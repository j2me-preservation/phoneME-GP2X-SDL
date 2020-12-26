/*
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */
package javax.microedition.media;

import java.util.Vector;
import java.util.Enumeration;
import java.util.Hashtable;

import java.io.*;

import javax.microedition.media.*;
import javax.microedition.media.control.*;

public class MIDIPlayer extends ABBBasicPlayer implements Runnable, VolumeControl
       { private native int  nMidiPlayerInit();
         private native int  nMidiPlayerRealize(int id, byte[] buf);
         private native int  nMidiPlayerPrefetch(int id);
         private native int  nMidiPlayerStart(int id);
         private native void nMidiPlayerStop(int id);
         private native void nMidiPlayerDeallocate(int id);
         private native void nMidiPlayerClose(int id);
         private native long nGetMediaTime(int id);
         private native int  nCheckEOM(int id);
         private native void nSetVolumeLevel(int id, int value);
         private Thread      CheckThread;
         private int         MIDIPlayer_ID;
         private int         VolumeLevel;
         
         public MIDIPlayer() throws Exception
                { super();
                  this.MIDIPlayer_ID = nMidiPlayerInit();
                  if (this.MIDIPlayer_ID == 0) throw new Exception("Out Of Memory Error!!!"); 
                  this.CheckThread = new Thread(this);
                }
    
         public int getAudioType() 
                { return AUDIO_MIDI;
                }

         protected void doRealize() throws MediaException
                   { try { int MidiFileSize = this.source.available();
                           byte[] Buffer = new byte[MidiFileSize];
                           if (this.source.read(Buffer) != MidiFileSize) throw new MediaException("I/O Error!"); 
                           else if (nMidiPlayerRealize(this.MIDIPlayer_ID, Buffer) != 0) throw new MediaException("Out Of Memory Error!");
                         } catch(Exception E) { System.out.println("Exception:"+E.getMessage()); }
                   }

         protected void doPrefetch() throws MediaException
                   { if (nMidiPlayerPrefetch(this.MIDIPlayer_ID) != 0) throw new MediaException("MIDI Prefetch Error!");
                   }

         protected boolean doStart()
                   { if (nMidiPlayerStart(this.MIDIPlayer_ID) == 0) return true;
                     return false;
                   }

         protected void doPostStart() 
                   { this.CheckThread.start();
                   }
                   
         protected void doStop() throws MediaException
                   { nMidiPlayerStop(this.MIDIPlayer_ID);
                   }

         protected void doDeallocate()
                   { nMidiPlayerDeallocate(this.MIDIPlayer_ID);
                   }

         protected void doClose()
                   { nMidiPlayerClose(this.MIDIPlayer_ID);
                     this.MIDIPlayer_ID = 0;
                   }

         protected long doSetMediaTime(long now) throws MediaException
                   { return now;
                   }

         protected long doGetMediaTime()
                   { return nGetMediaTime(this.MIDIPlayer_ID);
                   }

         protected long doGetDuration()
                   { return TIME_UNKNOWN;
                   }

         protected Control doGetControl(String type)
                   { if (type.compareTo("javax.microedition.media.control.VolumeControl") == 0) return this;
                     System.out.println("MIDIPlayer: doGetControl("+type+")");
                     return null;
                   }

         public String getContentType() 
                { chkClosed(true);
                  return "audio/midi";
                }

         public void run()
                { while(this.state == Player.STARTED)
                       { if (nCheckEOM(this.MIDIPlayer_ID) != 0) 
                            { sendEvent(PlayerListener.END_OF_MEDIA, new Long(this.doGetMediaTime()));
                              return;
                            }
                         Thread.yield();
                       }
                }

         public void setMute(boolean mute)
                { if (mute) 
                     { this.VolumeLevel = 0;
                       nSetVolumeLevel(this.MIDIPlayer_ID, this.VolumeLevel);
                     }
                }

         public boolean isMuted()
                { if (this.VolumeLevel == 0) return true;
                  return false;
                }

         public int setLevel(int level)
                { if (level < 0) level = 0;
                  if (level > 100) level = 100;
                  this.VolumeLevel = level;
                  nSetVolumeLevel(this.MIDIPlayer_ID, this.VolumeLevel);
                  return this.VolumeLevel;
                }

         public int getLevel()
                { return this.VolumeLevel;
                }

}


