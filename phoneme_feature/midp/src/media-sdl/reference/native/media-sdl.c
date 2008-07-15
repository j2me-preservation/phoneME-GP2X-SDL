#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <kni.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <math.h>
#include <pthread.h>
#include "timidity/timidity.h"

/*****************************************************************************/

struct MediaSDL_Channel
{ unsigned int   Assigned;
  void         (*Callback)(int);
  void          *Data;
};
static int                      MediaSDL_NumChannel;
static struct MediaSDL_Channel *MediaSDL_Channels;

#define SAMPLE_FREQ	22050

void AudioSubsystemCallback(int chan)
{ if ((chan>=0)&&(chan<MediaSDL_NumChannel))
     if (MediaSDL_Channels[chan].Assigned != 0)
        if (MediaSDL_Channels[chan].Callback != NULL)
           MediaSDL_Channels[chan].Callback(chan);
}

int InitAudioSubsystem()
{ int chan;
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) return(-1);
  if (Mix_OpenAudio(SAMPLE_FREQ, AUDIO_S16SYS, 2, 4096) != 0) return(-2);
  if (Timidity_Init() < 0) return(-3);
  MediaSDL_NumChannel = Mix_AllocateChannels(32);
  if (MediaSDL_NumChannel < 1) return(-4);
  MediaSDL_Channels = (struct MediaSDL_Channel *)malloc(sizeof(struct MediaSDL_Channel) * MediaSDL_NumChannel);
  if (MediaSDL_Channels == NULL) return(-5);
  for(chan=0; chan<MediaSDL_NumChannel; chan++)
     { MediaSDL_Channels[chan].Assigned = 0;
       MediaSDL_Channels[chan].Callback = NULL;
       MediaSDL_Channels[chan].Data = NULL;
     }
  Mix_ChannelFinished(AudioSubsystemCallback);
  return(0);
}

void FinalizeAudioSubsystem()
{ Timidity_Exit();
  SDL_CloseAudio();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

static int ReserveChannel()
{ int chan;
  for(chan=0; chan<MediaSDL_NumChannel; chan++)
     if (MediaSDL_Channels[chan].Assigned == 0)
        { MediaSDL_Channels[chan].Assigned = 1;
          return(chan);
        }
  return(-1);
}

static void FreeChannel(int chan)
{ if ((chan>=0)&&(chan<MediaSDL_NumChannel))
     { MediaSDL_Channels[chan].Assigned = 0;
       MediaSDL_Channels[chan].Callback = NULL;
       MediaSDL_Channels[chan].Data = NULL;
     }
}

/*****************************************************************************/

static unsigned short MidiNotes[128] = {0x0008,0x0008,0x0009,0x0009,0x000A,0x000A,0x000B,0x000C,
                                        0x000C,0x000D,0x000E,0x000F,0x0010,0x0011,0x0012,0x0013,
                                        0x0014,0x0015,0x0017,0x0018,0x0019,0x001B,0x001D,0x001E,
                                        0x0020,0x0022,0x0024,0x0026,0x0029,0x002B,0x002E,0x0031,
                                        0x0033,0x0037,0x003A,0x003D,0x0041,0x0045,0x0049,0x004D,
                                        0x0052,0x0057,0x005C,0x0062,0x0067,0x006E,0x0074,0x007B,
                                        0x0082,0x008A,0x0092,0x009B,0x00A4,0x00AE,0x00B9,0x00C4,
                                        0x00CF,0x00DC,0x00E9,0x00F6,0x0105,0x0115,0x0125,0x0137,
                                        0x0149,0x015D,0x0172,0x0188,0x019F,0x01B8,0x01D2,0x01ED,
                                        0x020B,0x022A,0x024B,0x026E,0x0293,0x02BA,0x02E4,0x0310,
                                        0x033E,0x0370,0x03A4,0x03DB,0x0416,0x0454,0x0496,0x04DC,
                                        0x0526,0x0574,0x05C8,0x0620,0x067D,0x06E0,0x0748,0x07B7,
                                        0x082D,0x08A9,0x092D,0x09B9,0x0A4D,0x0AE9,0x0B90,0x0C40,
                                        0x0CFA,0x0DC0,0x0E91,0x0F6F,0x105A,0x1153,0x125A,0x1372,
                                        0x149A,0x15D3,0x1720,0x1880,0x19F5,0x1B80,0x1D22,0x1EDE,
                                        0x20B4,0x22A6,0x24B5,0x26E4,0x2934,0x2BA7,0x2E40,0x3100};

struct NativeTonePlayer
{ Mix_Chunk MC;
  int       Chan;
};

struct NativeTonePlayer *CreateToneChunk(int Note, int Volume)
{ struct NativeTonePlayer *Ret;
  short Value, *Buf;
  unsigned int i, m;
  if ((Note<0)||(Note>127)) return(NULL);
  Ret = (struct NativeTonePlayer *)malloc(sizeof(struct NativeTonePlayer));
  if (Ret == NULL) return(NULL);
  Ret->MC.allocated = 0;
  Ret->MC.volume = MIX_MAX_VOLUME;
  Ret->MC.alen = SAMPLE_FREQ / MidiNotes[Note];
  Ret->MC.abuf = malloc(Ret->MC.alen * 2);
  if (Ret->MC.abuf == NULL)
     { free(Ret);
       return(NULL);
     }
  Value = (Volume * 32767) / 100;
  m = (Ret->MC.alen >> 1);
  Buf = (short *)Ret->MC.abuf;
  for(i=0; i<Ret->MC.alen; i++)
     Buf[i] = (i<m) ? Value : -Value;
  Ret->MC.alen <<= 1;
  Ret->Chan = -1;
  return(Ret);
}

void FreeToneChunk(struct NativeTonePlayer *NTP)
{ if (NTP != NULL)
     { if (NTP->MC.abuf != NULL) free(NTP->MC.abuf);
       free(NTP);
     }  
}

void TonePlayerCallback(int chan)
{ struct NativeTonePlayer *NTP;
  NTP = (struct NativeTonePlayer *)MediaSDL_Channels[chan].Data;
  FreeChannel(chan);
  FreeToneChunk(NTP);
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_Manager_nPlayTone()
{ struct NativeTonePlayer *NTP;
  jint Note = KNI_GetParameterAsInt(1);
  jint Duration = KNI_GetParameterAsInt(2);
  jint Volume = KNI_GetParameterAsInt(3);
  printf("nPlayTone(%d, %d, %d)\n", Note, Duration, Volume); 
  NTP = CreateToneChunk(Note, Volume);
  if (NTP != NULL)
     { NTP->Chan = ReserveChannel();
       if (NTP->Chan == -1) FreeToneChunk(NTP);
       else { MediaSDL_Channels[NTP->Chan].Data = NTP;
              MediaSDL_Channels[NTP->Chan].Callback = TonePlayerCallback;
              Mix_PlayChannelTimed(NTP->Chan, &NTP->MC, -1, Duration);
            }
     }
  KNI_ReturnVoid();
}

/*****************************************************************************/

#define MIDI_CHUNK_SIZE	16384

struct NativeMIDIPlayer
{ MidiSong      *Song;
  Mix_Chunk      MC;
  int            Chan;
  long           MediaSample;
  long long      LastTime;
  int            CheckEOM;
  int            VolumeLevel;
  int            Stopped;
};

long long GetTimeMillis()
{ struct timeval TV;
  long long Ret;
  gettimeofday(&TV, NULL);
  Ret = (long long)TV.tv_sec * 1000;
  Ret += (long long)TV.tv_usec / 1000;
  return(Ret);
}

void MidiPlayerCallback(int chan)
{ struct NativeMIDIPlayer *NMP = (struct NativeMIDIPlayer *)MediaSDL_Channels[chan].Data;
  if (NMP->Stopped) return;
  NMP->LastTime = GetTimeMillis();
  NMP->MediaSample += (NMP->MC.alen >> 2);
  NMP->MC.alen = Timidity_PlaySome(NMP->Song, NMP->MC.abuf, MIDI_CHUNK_SIZE);
  if (NMP->MC.alen <= 0)
     { NMP->CheckEOM = 1;
       return;
     }
  Mix_PlayChannel(NMP->Chan, &NMP->MC, 0);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_MIDIPlayer_nMidiPlayerInit()
{ struct NativeMIDIPlayer *Ret;
  Ret = malloc(sizeof(struct NativeMIDIPlayer));
  if (Ret != NULL) 
     { Ret->CheckEOM = 0;
       Ret->VolumeLevel = 100;
       Ret->Chan = -1;
       Ret->Song = NULL;
       Ret->Stopped = 1;
       Ret->MediaSample = 0;
       Ret->MC.allocated = 0;
       Ret->MC.volume = MIX_MAX_VOLUME;
       Ret->MC.alen = MIDI_CHUNK_SIZE;
       Ret->MC.abuf = malloc(Ret->MC.alen);
       if (Ret->MC.abuf == NULL)
          { free(Ret);
            Ret = NULL;
          }
     }
  KNI_ReturnInt((int)Ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_MIDIPlayer_nMidiPlayerRealize()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  SDL_AudioSpec audio;
  SDL_RWops *RW;
  unsigned char *Buffer;
  unsigned int BufferSize;
  int ret=0;
  KNI_StartHandles(1);
  KNI_DeclareHandle(buf); 
  KNI_GetParameterAsObject(2, buf);
  NMP = (struct NativeMIDIPlayer *)id;
  BufferSize = KNI_GetArrayLength(buf);  
  Buffer = malloc(BufferSize);
  if (Buffer == NULL) ret = -1;
  else { KNI_GetRawArrayRegion(buf, 0, (jsize)BufferSize, (jbyte*)Buffer);
         RW = SDL_RWFromMem(Buffer, BufferSize);
         if (RW == NULL) ret = -2;
         else { audio.freq = 22050;
                audio.format = AUDIO_S16SYS;
                audio.channels = 2;
                audio.samples = MIDI_CHUNK_SIZE >> 2;
                audio.callback = NULL;
                NMP->Song = Timidity_LoadSong(RW, &audio);
                SDL_RWclose(RW);
                free(Buffer);
                if (NMP->Song == NULL) ret = -3;
              }
       }
  KNI_EndHandles();
  KNI_ReturnInt(ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_MIDIPlayer_nMidiPlayerPrefetch()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  int ret=0;
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->Chan = ReserveChannel();
  if (NMP->Chan == -1) ret = -1;
  else { MediaSDL_Channels[NMP->Chan].Data = NMP;
         MediaSDL_Channels[NMP->Chan].Callback = MidiPlayerCallback;
       }
  KNI_ReturnInt(ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_MIDIPlayer_nMidiPlayerStart()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  int ret=0;
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->LastTime = GetTimeMillis();
  NMP->CheckEOM = 0;
  NMP->Stopped = 0;
  Timidity_Start(NMP->Song);
  NMP->MC.alen = Timidity_PlaySome(NMP->Song, NMP->MC.abuf, MIDI_CHUNK_SIZE);
  if (NMP->MC.alen == 0)
     { FreeChannel(NMP->Chan);
       NMP->Chan = -1;
       ret = -1;
     }
  else Mix_PlayChannel(NMP->Chan, &NMP->MC, 0);
  KNI_ReturnInt(ret);
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_MIDIPlayer_nMidiPlayerStop()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->Stopped = 1;
  Mix_HaltChannel(NMP->Chan);
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_MIDIPlayer_nMidiPlayerDeallocate()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->Stopped = 1;
  Mix_HaltChannel(NMP->Chan);
  FreeChannel(NMP->Chan);
  NMP->Chan = -1;
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_MIDIPlayer_nMidiPlayerClose()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->Stopped = 1;
  if (NMP->Chan != -1)
     { Mix_HaltChannel(NMP->Chan);
       FreeChannel(NMP->Chan);
       NMP->Chan = -1;
     }
  Timidity_FreeSong(NMP->Song);
  free(NMP->MC.abuf);
  free(NMP);
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_LONG Java_javax_microedition_media_MIDIPlayer_nGetMediaTime()
{ struct NativeMIDIPlayer *NMP;
  long Ret;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeMIDIPlayer *)id;
  Ret = (NMP->MediaSample * 1000) / SAMPLE_FREQ;
  Ret += (long)(GetTimeMillis() - NMP->LastTime);
  KNI_ReturnLong(Ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_MIDIPlayer_nCheckEOM()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeMIDIPlayer *)id;
  KNI_ReturnInt(NMP->CheckEOM);
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_MIDIPlayer_nSetVolumeLevel()
{ struct NativeMIDIPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  jint value = KNI_GetParameterAsInt(2);
  NMP = (struct NativeMIDIPlayer *)id;
  NMP->VolumeLevel = value;
  NMP->MC.volume = (MIX_MAX_VOLUME * value) / 100;
  KNI_ReturnVoid();
}

/*****************************************************************************/

struct ToneSequenceEvent
{ signed char Type;
  signed char Value;
};

struct ToneSequenceBlock
{ int Start;
  int Stop;
  int Size;
};

struct ToneSequenceLoaded
{ char                      Tempo;
  char                      Resolution;
  struct ToneSequenceBlock  TS_Blocks[256];
  struct ToneSequenceBlock  TS_Main;
  char                     *Buffer;
  int                       BufSize;
  struct ToneSequenceEvent *Sequence;
  int                       SeqSize;
};

int EvalBlockSize(struct ToneSequenceEvent *Events, struct ToneSequenceBlock *Block, struct ToneSequenceBlock *Blocks, struct ToneSequenceEvent *Result)
{ struct ToneSequenceEvent *evt;
  int i, start, stop, ret, pw, j;
  start = Block->Start;
  stop = Block->Stop + 1;
  for(ret=0,pw=0,i=start; i<stop; i++)
     { evt = &Events[i];
       if (evt->Type >= -1) // PLAY_NOTE or SILENCE
          { ret++;
            if (Result != NULL) 
               { Result[pw].Type = evt->Type;
                 Result[pw].Value = evt->Value;
                 pw++;  
               }
          }
       else switch(evt->Type)
                  { case -7: // PLAY_BLOCK
                             ret += Blocks[(unsigned char)evt->Value].Size;
                             if (Result != NULL) 
                                { j = EvalBlockSize(Events, &Blocks[(unsigned char)evt->Value], Blocks, &Result[pw]);
                                  if (j < 0) return(-1);
                                  pw += j;
                                }
                             break;
                    case -8: // SET_VOLUME
                             ret++;
                             if (Result != NULL) 
                                { Result[pw].Type = evt->Type;
                                  Result[pw].Value = evt->Value;
                                  pw++;  
                                }
                             break;
                    case -9: // REPEAT
                             ret += evt->Value;
                             i++;
                             if (Result != NULL)
                                { for(j=0; j<evt->Value; j++,pw++)
                                     { Result[pw].Type = Events[i].Type;
                                       Result[pw].Value = Events[i].Value;
                                     }
                                }
                             break;
                    
                  }
     }
  return(ret);
}

int EvalBlocksSize(struct ToneSequenceLoaded *TSL)
{ int i, ret;
  for(i=0; i<256; i++)
     if (TSL->TS_Blocks[i].Start > -1)
        { ret = EvalBlockSize((struct ToneSequenceEvent *)TSL->Buffer, &TSL->TS_Blocks[i], TSL->TS_Blocks, NULL);
          if (ret < 0) return(-1);
          TSL->TS_Blocks[i].Size = ret;
        } 
  for(i=0; i<256; i++)
     if (TSL->TS_Blocks[i].Start > -1)
        { ret = EvalBlockSize((struct ToneSequenceEvent *)TSL->Buffer, &TSL->TS_Blocks[i], TSL->TS_Blocks, NULL);
          if (ret < 0) return(-1);
          TSL->TS_Blocks[i].Size = ret;
        } 
  return(0);
}

struct ToneSequenceLoaded *InitToneSequence(char *Buffer, unsigned int BufSize)
{ struct ToneSequenceLoaded *Ret;
  struct ToneSequenceEvent *Events;
  int i, ss, inBlock, inMain;
  if ((BufSize & 0x01) != 0) return(NULL);
  Ret = (struct ToneSequenceLoaded *)malloc(sizeof(struct ToneSequenceLoaded));
  if (Ret == NULL) return(NULL);
  Ret->Tempo = 30;
  Ret->Resolution = 64;
  Ret->Buffer = Buffer;
  Ret->BufSize = BufSize;
  Events = (struct ToneSequenceEvent *)Buffer;
  if ((Events[0].Type != -2) || (Events[0].Value != 1)) // VERSION = 1
     { free(Ret);
       return(NULL);
     }
  for(i=0; i<256; i++) 
     { Ret->TS_Blocks[i].Start = -1;
       Ret->TS_Blocks[i].Stop = -1;
       Ret->TS_Blocks[i].Size = 0;
     }
  Ret->TS_Main.Start = -1;
  Ret->TS_Main.Stop = -1;
  Ret->TS_Main.Size = 0;
  ss = Ret->BufSize >> 1;
  inBlock = inMain = 0;
  for(i=1; i<ss; i++)
     { switch(Events[i].Type)
             { case -3: // TEMPO
                        Ret->Tempo = Events[i].Value;
                        break;
               case -4: // RESOLUTION
                        Ret->Resolution = Events[i].Value;
                        break;
               case -5: // BLOCK_START
                        if ((Ret->TS_Blocks[(unsigned char)Events[i].Value].Start != -1) || (inBlock == 1) || (inMain == 1))
                           { free(Ret);
                             return(NULL);
                           }
                        Ret->TS_Blocks[(unsigned char)Events[i].Value].Start = i+1;
                        inBlock = 1;
                        break;
               case -6: // BLOCK_END
                        if ((Ret->TS_Blocks[(unsigned char)Events[i].Value].Start == -1) || (Ret->TS_Blocks[(unsigned char)Events[i].Value].Stop != -1) || (inBlock == 0))
                           { free(Ret);
                             return(NULL);
                           }
                        Ret->TS_Blocks[(unsigned char)Events[i].Value].Stop = i-1;
                        inBlock = 0;
                        break;
               case -7: // PLAY_BLOCK
                        if ((inBlock == 0) && (inMain == 0)) 
                           { inMain = 1;
                             Ret->TS_Main.Start = i;
                           }
                        break;
               case -8: // SET_VOLUME
                        if ((inBlock == 0) && (inMain == 0)) 
                           { inMain = 1;
                             Ret->TS_Main.Start = i;
                           }
                        break;
               case -9: // REPEAT
                        if ((inBlock == 0) && (inMain == 0)) 
                           { inMain = 1;
                             Ret->TS_Main.Start = i;
                           }
                        break;
               default: if (Events[i].Type >= -1) // PLAY_NOTE or SILENCE
                           { if ((inBlock == 0) && (inMain == 0)) 
                              { inMain = 1;
                                Ret->TS_Main.Start = i;
                              }
                           }
                        else { // Invalid
                               free(Ret);
                               return(NULL);
                             }
             }
     }
  Ret->TS_Main.Stop = ss-1;
  if (EvalBlocksSize(Ret) != 0)
     { free(Ret);
       return(NULL);
     }
  Ret->SeqSize = EvalBlockSize(Events, &Ret->TS_Main, Ret->TS_Blocks, NULL);
  if (Ret->SeqSize == -1)
     { free(Ret);
       return(NULL);
     }
  Ret->Sequence = (struct ToneSequenceEvent *)malloc(sizeof(struct ToneSequenceEvent) * Ret->SeqSize);
  if (Ret->Sequence == NULL)
     { free(Ret);
       return(NULL);
     }
  EvalBlockSize(Events, &Ret->TS_Main, Ret->TS_Blocks, Ret->Sequence);
  return(Ret);
}

#define TS_CHUNK_SIZE	16384

struct NativeTSPlayer
{ struct ToneSequenceLoaded *Loaded;
  struct NativeTonePlayer   *NTP;
  int                        Chan;
  long                       TimeSampled;
  long long                  LastTime;
  int                        CheckEOM;
  int                        TimeMult;
  int                        ActualNote;
  int                        ActualDuration;
  int                        ActualVolume;
  int                        Stopped;
};

int PlayNextTone(struct NativeTSPlayer *NMP)
{ int Type=0;
  if (NMP->NTP != NULL) 
     { FreeToneChunk(NMP->NTP);
       NMP->NTP = NULL;
     }
  do { NMP->ActualNote++;
       if (NMP->ActualNote >= NMP->Loaded->SeqSize) return(-1);
       Type = NMP->Loaded->Sequence[NMP->ActualNote].Type;
       if (Type == -8) NMP->ActualVolume = NMP->Loaded->Sequence[NMP->ActualNote].Value;
     } while(Type == -8); 
  if (Type == -1) NMP->NTP = CreateToneChunk(0x40, 0);
  else NMP->NTP = CreateToneChunk(Type, NMP->ActualVolume);
  if (NMP->NTP == NULL) return(-1);
  NMP->ActualDuration = NMP->Loaded->Sequence[NMP->ActualNote].Value * NMP->TimeMult;
  NMP->LastTime = GetTimeMillis();
  Mix_PlayChannelTimed(NMP->Chan, &NMP->NTP->MC, -1, NMP->ActualDuration);
  return(0);
}

void TSPlayerCallback(int chan)
{ struct NativeTSPlayer *NMP = (struct NativeTSPlayer *)MediaSDL_Channels[chan].Data;
  NMP->TimeSampled += NMP->ActualDuration;
  if (NMP->Stopped) return;
  if (PlayNextTone(NMP) != 0) NMP->CheckEOM = 1;
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerInit()
{ struct NativeTSPlayer *Ret;
  Ret = malloc(sizeof(struct NativeTSPlayer));
  if (Ret != NULL) 
     { Ret->CheckEOM = 0;
       Ret->Chan = -1;
       Ret->TimeSampled = 0;
       Ret->NTP = NULL;
       Ret->Stopped = 1;
     }
  KNI_ReturnInt((int)Ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerRealize()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  int ret=0, seqsize;
  char *sequence;
  KNI_StartHandles(1);
  KNI_DeclareHandle(buf); 
  KNI_GetParameterAsObject(2, buf);
  NMP = (struct NativeTSPlayer *)id;
  seqsize = KNI_GetArrayLength(buf);  
  sequence = malloc(seqsize);
  if (sequence == NULL) ret = -1;
  else { KNI_GetRawArrayRegion(buf, 0, (jsize)seqsize, (jbyte*)sequence);
         NMP->Loaded = InitToneSequence(sequence, seqsize);
         if (NMP->Loaded == NULL) { free(sequence); ret = -1;}
         else { NMP->TimeMult = 240000 / (NMP->Loaded->Resolution * NMP->Loaded->Tempo * 4);
                printf("\nToneSequence: Load OK! (Tempo=%d  Resolution=%d  TimeMult=%d)\n", NMP->Loaded->Tempo, NMP->Loaded->Resolution, NMP->TimeMult);
              }   
       }
  KNI_EndHandles();
  KNI_ReturnInt(ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerStart()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  int ret=0;
  NMP = (struct NativeTSPlayer *)id;
  NMP->Chan = ReserveChannel();
  if (NMP->Chan == -1) ret = -1;
  MediaSDL_Channels[NMP->Chan].Data = NMP;
  MediaSDL_Channels[NMP->Chan].Callback = TSPlayerCallback;
  NMP->ActualNote = 0;
  NMP->ActualVolume = 100;
  NMP->LastTime = GetTimeMillis();
  NMP->CheckEOM = 0;
  NMP->TimeSampled = 0;
  NMP->Stopped = 0;
  if (PlayNextTone(NMP) != 0) ret=-1;
  KNI_ReturnInt(ret);
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerStop()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeTSPlayer *)id;
  NMP->Stopped = 1;
  if (NMP->Chan >= 0) Mix_HaltChannel(NMP->Chan);
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerDeallocate()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeTSPlayer *)id;
  NMP->Stopped = 1;
  if (NMP->Chan >= 0) 
     { Mix_HaltChannel(NMP->Chan);
       FreeChannel(NMP->Chan);
       NMP->Chan = -1;
     }
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID Java_javax_microedition_media_ToneSequencePlayer_nTSPlayerClose()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeTSPlayer *)id;
  NMP->Stopped = 1;
  if (NMP->Chan != -1)
     { Mix_HaltChannel(NMP->Chan);
       FreeChannel(NMP->Chan);
       NMP->Chan = -1;
     }
  free(NMP->Loaded->Buffer);
  free(NMP->Loaded->Sequence);
  free(NMP->Loaded);
  free(NMP);
  KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_LONG Java_javax_microedition_media_ToneSequencePlayer_nTSGetMediaTime()
{ struct NativeTSPlayer *NMP;
  long Ret;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeTSPlayer *)id;
  Ret = NMP->TimeSampled;
  Ret += (long)(GetTimeMillis() - NMP->LastTime);
  KNI_ReturnLong(Ret);
}

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_media_ToneSequencePlayer_nTSCheckEOM()
{ struct NativeTSPlayer *NMP;
  jint id = KNI_GetParameterAsInt(1);
  NMP = (struct NativeTSPlayer *)id;
  KNI_ReturnInt(NMP->CheckEOM);
}

