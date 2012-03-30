/*
 *   
 *
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

#include <midp_logging.h>
#include <midp_mastermode_port.h>
#include <keymap_input.h>

#include "SDL.h"

enum
{
     BTN_A = 0,     //       A /             1
     BTN_X = 1,     //       X /             2
     BTN_B = 2,     //       B /             3
     BTN_Y = 3,     //       Y /             4
     BTN_L = 4,     //       L /         5, L1
     BTN_R = 5,     //       R /         6, L2
     BTN_HOME = 6,  //    Home /         7, R1
     BTN_HOLD = 7,  //    Hold /         8, R2
     BTN_HELP1 = 8, //  Help I /        Select
     BTN_HELP2 = 9, // Help II /         Start
     BTN_TACT = 10, //    Tact / L Thumb Stick
     BTN_UP = 11, 
     BTN_DOWN = 12,
     BTN_LEFT = 13,
     BTN_RIGHT = 14,
     BTN_UP_LEFT = 15,
     BTN_UP_RIGHT = 16,
     BTN_DOWN_LEFT = 17,
     BTN_DOWN_RIGHT = 18
};


static int GP2X_Default_keys[19];

void InitGP2XKeys()
{ char *Value;
/*
  GP2X_Default_keys[0] = KEYMAP_KEY_UP;
  GP2X_Default_keys[1] = KEYMAP_KEY_1;
  GP2X_Default_keys[2] = KEYMAP_KEY_LEFT;
  GP2X_Default_keys[3] = KEYMAP_KEY_7;
  GP2X_Default_keys[4] = KEYMAP_KEY_DOWN;
  GP2X_Default_keys[5] = KEYMAP_KEY_9;
  GP2X_Default_keys[6] = KEYMAP_KEY_RIGHT;
  GP2X_Default_keys[7] = KEYMAP_KEY_3;
  GP2X_Default_keys[8] = KEYMAP_KEY_SOFT2;
  GP2X_Default_keys[9] = KEYMAP_KEY_SOFT1;
  GP2X_Default_keys[10] = KEYMAP_KEY_POUND;
  GP2X_Default_keys[11] = KEYMAP_KEY_ASTERISK;
  GP2X_Default_keys[12] = KEYMAP_KEY_GAMEA;
  GP2X_Default_keys[13] = KEYMAP_KEY_GAMEB;
  GP2X_Default_keys[14] = KEYMAP_KEY_GAMEC;
  GP2X_Default_keys[15] = KEYMAP_KEY_GAMED;
  GP2X_Default_keys[16] = KEYMAP_KEY_CLEAR;
  GP2X_Default_keys[17] = KEYMAP_KEY_SELECT;
  GP2X_Default_keys[18] = KEYMAP_KEY_5;
*/
  GP2X_Default_keys[11] = KEYMAP_KEY_UP;
  GP2X_Default_keys[17] = KEYMAP_KEY_1;
  GP2X_Default_keys[13] = KEYMAP_KEY_LEFT;
  GP2X_Default_keys[15] = KEYMAP_KEY_7;
  GP2X_Default_keys[12] = KEYMAP_KEY_DOWN;
  GP2X_Default_keys[16] = KEYMAP_KEY_9;
  GP2X_Default_keys[14] = KEYMAP_KEY_RIGHT;
  GP2X_Default_keys[18] = KEYMAP_KEY_3;

  GP2X_Default_keys[9] = KEYMAP_KEY_SOFT2;
  GP2X_Default_keys[8] = KEYMAP_KEY_SOFT1;
  GP2X_Default_keys[4] = KEYMAP_KEY_POUND;
  GP2X_Default_keys[5] = KEYMAP_KEY_ASTERISK;
  GP2X_Default_keys[0] = KEYMAP_KEY_GAMEA;
  GP2X_Default_keys[2] = KEYMAP_KEY_GAMEB;
  GP2X_Default_keys[1] = KEYMAP_KEY_GAMEC;
  GP2X_Default_keys[3] = KEYMAP_KEY_GAMED;
  GP2X_Default_keys[6] = KEYMAP_KEY_CLEAR;
  GP2X_Default_keys[7] = KEYMAP_KEY_SELECT;
  GP2X_Default_keys[10] = KEYMAP_KEY_5;
/*
  if ((Value = getenv("J2ME_GP2X_JOYU")) != NULL) GP2X_Default_keys[0] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYUL")) != NULL) GP2X_Default_keys[1] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYL")) != NULL) GP2X_Default_keys[2] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYDL")) != NULL) GP2X_Default_keys[3] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYD")) != NULL) GP2X_Default_keys[4] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYDR")) != NULL) GP2X_Default_keys[5] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYR")) != NULL) GP2X_Default_keys[6] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYUR")) != NULL) GP2X_Default_keys[7] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_START")) != NULL) GP2X_Default_keys[8] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_SELECT")) != NULL) GP2X_Default_keys[9] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_LEFT")) != NULL) GP2X_Default_keys[10] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_RIGHT")) != NULL) GP2X_Default_keys[11] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTA")) != NULL) GP2X_Default_keys[12] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTB")) != NULL) GP2X_Default_keys[13] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTX")) != NULL) GP2X_Default_keys[14] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTY")) != NULL) GP2X_Default_keys[15] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_VOLU")) != NULL) GP2X_Default_keys[16] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_VOLD")) != NULL) GP2X_Default_keys[17] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYC")) != NULL) GP2X_Default_keys[18] = atoi(Value);
*/
  if ((Value = getenv("J2ME_GP2X_JOYU")) != NULL) GP2X_Default_keys[11] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYUL")) != NULL) GP2X_Default_keys[15] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYL")) != NULL) GP2X_Default_keys[13] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYDL")) != NULL) GP2X_Default_keys[17] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYD")) != NULL) GP2X_Default_keys[12] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYDR")) != NULL) GP2X_Default_keys[18] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYR")) != NULL) GP2X_Default_keys[14] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYUR")) != NULL) GP2X_Default_keys[16] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_START")) != NULL) GP2X_Default_keys[9] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_SELECT")) != NULL) GP2X_Default_keys[8] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_LEFT")) != NULL) GP2X_Default_keys[4] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_RIGHT")) != NULL) GP2X_Default_keys[5] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTA")) != NULL) GP2X_Default_keys[0] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTB")) != NULL) GP2X_Default_keys[2] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTX")) != NULL) GP2X_Default_keys[1] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_BUTY")) != NULL) GP2X_Default_keys[3] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_VOLU")) != NULL) GP2X_Default_keys[6] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_VOLD")) != NULL) GP2X_Default_keys[7] = atoi(Value);
  if ((Value = getenv("J2ME_GP2X_JOYC")) != NULL) GP2X_Default_keys[10] = atoi(Value);
}

void KeyboardCheck(SDL_Event *event, MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent)
{ int Key = KEYMAP_KEY_INVALID;
  switch(event->key.keysym.sym)
        { case SDLK_LSUPER: Key = KEYMAP_KEY_SOFT1; break;
          case SDLK_MENU:   Key = KEYMAP_KEY_SOFT2; break;
          case SDLK_UP:     Key = KEYMAP_KEY_UP; break;
          case SDLK_DOWN:   Key = KEYMAP_KEY_DOWN; break;
          case SDLK_LEFT:   Key = KEYMAP_KEY_LEFT; break;
          case SDLK_RIGHT:  Key = KEYMAP_KEY_RIGHT; break;
          case SDLK_RETURN: Key = KEYMAP_KEY_SELECT; break;
          case SDLK_CLEAR:  Key = KEYMAP_KEY_CLEAR; break;
          case SDLK_KP0:    Key = KEYMAP_KEY_0; break;
          case SDLK_KP1:    Key = KEYMAP_KEY_7; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP2:    Key = KEYMAP_KEY_8; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP3:    Key = KEYMAP_KEY_9; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP4:    Key = KEYMAP_KEY_4; break;
          case SDLK_KP5:    Key = KEYMAP_KEY_5; break;
          case SDLK_KP6:    Key = KEYMAP_KEY_6; break;
          case SDLK_KP7:    Key = KEYMAP_KEY_1; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP8:    Key = KEYMAP_KEY_2; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP9:    Key = KEYMAP_KEY_3; break; // KEYPADNUM reverse KEYPHONE
          case SDLK_KP_MULTIPLY: Key = KEYMAP_KEY_ASTERISK; break;
          case SDLK_KP_PLUS:     Key = KEYMAP_KEY_POUND; break;
          case SDLK_HOME:   Key = KEYMAP_KEY_SEND; break;
          case SDLK_END:    Key = KEYMAP_KEY_END; break;
          case SDLK_PAGEUP: Key = -9; break;
          default: Key = KEYMAP_KEY_INVALID;
        }
  pNewSignal->waitingFor = UI_SIGNAL;
  pNewMidpEvent->type = MIDP_KEY_EVENT;
  pNewMidpEvent->CHR = Key;
  pNewMidpEvent->ACTION = (event->key.state == SDL_PRESSED) ? KEYMAP_STATE_PRESSED : KEYMAP_STATE_RELEASED;
}

void JoystickCheck(SDL_Event *event, MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent)
{ int Key = event->jbutton.button;
  if ((Key>=0) && (Key<=18)) {
    Key = GP2X_Default_keys[Key];
//    fprintf(stdout,"button %d - key: %d\n",event->jbutton.button,GP2X_Default_keys[event->jbutton.button]);
  }
  else Key = KEYMAP_KEY_INVALID;
  pNewSignal->waitingFor = UI_SIGNAL;
  pNewMidpEvent->type = MIDP_KEY_EVENT;
  pNewMidpEvent->CHR = Key;
  pNewMidpEvent->ACTION = (event->jbutton.state == SDL_PRESSED) ? KEYMAP_STATE_PRESSED : KEYMAP_STATE_RELEASED;
}

void Caanoo_PushAnalogEvent(int btn, int pressed){
    SDL_Event event;
    event.type = (pressed)?SDL_JOYBUTTONDOWN:SDL_JOYBUTTONUP;
    event.jbutton.button = btn;
    event.jbutton.state = (pressed)?SDL_PRESSED:SDL_RELEASED;
    event.jbutton.which = 0;
    SDL_PushEvent(&event);
}


//void JoystickCheckAxis(SDL_Event *event, MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent)
void Caanoo_UpdateAnalog(void)
{ static int buttonsPrev = 0;
  static SDL_Joystick *joy0 = NULL;
  if ( joy0 == NULL ) joy0=SDL_JoystickOpen(0);
  int buttonsNow = 0;
  SDL_JoystickUpdate();

    if (SDL_JoystickGetAxis( joy0, 0 ) < -16384) buttonsNow |= (1 << BTN_LEFT);
    if (SDL_JoystickGetAxis( joy0, 0 ) >  16384) buttonsNow |= (1 << BTN_RIGHT);
    if (SDL_JoystickGetAxis( joy0, 1 ) < -16384) buttonsNow |= (1 << BTN_UP);
    if (SDL_JoystickGetAxis( joy0, 1 ) >  16384) buttonsNow |= (1 << BTN_DOWN);

    // DIAGONALS
    if (SDL_JoystickGetAxis(joy0, 0) > 11585 && SDL_JoystickGetAxis(joy0, 1) < -11585)
      buttonsNow |= (1 << BTN_UP_RIGHT);
    if (SDL_JoystickGetAxis(joy0, 0) > 11585 && SDL_JoystickGetAxis(joy0, 1) > 11585)
      buttonsNow |= (1 << BTN_DOWN_RIGHT);
    if (SDL_JoystickGetAxis(joy0, 0) < -11585 && SDL_JoystickGetAxis(joy0, 1) > 11585)
      buttonsNow |= (1 << BTN_DOWN_LEFT);
    if (SDL_JoystickGetAxis(joy0, 0) < -11585 && SDL_JoystickGetAxis(joy0, 1) < -11585)
      buttonsNow |= (1 << BTN_UP_LEFT);

    if (buttonsNow != buttonsPrev)
    {
        if ((buttonsNow & (1 << BTN_LEFT)) != (buttonsPrev & (1 << BTN_LEFT)))
        {
                Caanoo_PushAnalogEvent(BTN_LEFT, (buttonsNow & (1 << BTN_LEFT)));
        }
        if ((buttonsNow & (1 << BTN_RIGHT)) != (buttonsPrev & (1 << BTN_RIGHT)))
        {
                Caanoo_PushAnalogEvent(BTN_RIGHT, (buttonsNow & (1 << BTN_RIGHT)));
        }
        if ((buttonsNow & (1 << BTN_UP)) != (buttonsPrev & (1 << BTN_UP)))
        {
                Caanoo_PushAnalogEvent(BTN_UP, (buttonsNow & (1 << BTN_UP)));
        }
        if ((buttonsNow & (1 << BTN_DOWN)) != (buttonsPrev & (1 << BTN_DOWN)))
        {
                Caanoo_PushAnalogEvent(BTN_DOWN, (buttonsNow & (1 << BTN_DOWN)));
        }
      // DIAGONALS
      if ((buttonsNow & (1 << BTN_UP_RIGHT)) != (buttonsPrev & (1<< BTN_UP_RIGHT)))
        {
          Caanoo_PushAnalogEvent(BTN_UP_RIGHT, (buttonsNow & (1 << BTN_UP_RIGHT)));
        }
      if ((buttonsNow & (1 << BTN_DOWN_RIGHT)) != (buttonsPrev & (1<< BTN_DOWN_RIGHT)))
        {
          Caanoo_PushAnalogEvent(BTN_DOWN_RIGHT, (buttonsNow & (1 << BTN_DOWN_RIGHT)));
        }
      if ((buttonsNow & (1 << BTN_DOWN_LEFT)) != (buttonsPrev & (1<< BTN_DOWN_LEFT)))
        {
          Caanoo_PushAnalogEvent(BTN_DOWN_LEFT, (buttonsNow & (1 << BTN_DOWN_LEFT)));
        }
      if ((buttonsNow & (1 << BTN_UP_LEFT)) != (buttonsPrev & (1<< BTN_UP_LEFT)))
        {
          Caanoo_PushAnalogEvent(BTN_UP_LEFT, (buttonsNow & (1 << BTN_UP_LEFT)));
        }
    }
    buttonsPrev = buttonsNow;
}

void CheckEvent(SDL_Event *event, MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent)
{ 

  if (event->type == SDL_QUIT)
     { SDL_Quit();
       exit(0);
     }
  if ((event->type == SDL_KEYDOWN) || (event->type == SDL_KEYUP))
     { KeyboardCheck(event, pNewSignal, pNewMidpEvent);
       return;
     }
  if ((event->type == SDL_JOYBUTTONDOWN) || (event->type == SDL_JOYBUTTONUP))
     { JoystickCheck(event, pNewSignal, pNewMidpEvent);
       return;
     }
}

/*
 * This function is called by the VM periodically. It has to check if
 * system has sent a signal to MIDP and return the result in the
 * structs given.
 *
 * Values for the <timeout> paramater:
 *  >0 = Block until a signal sent to MIDP, or until <timeout> milliseconds
 *       has elapsed.
 *   0 = Check the system for a signal but do not block. Return to the
 *       caller immediately regardless of the if a signal was sent.
 *  -1 = Do not timeout. Block until a signal is sent to MIDP.
 */
void checkForSystemSignal(MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent, jlong timeout) 
{ SDL_Event event;
  Caanoo_UpdateAnalog();
  jlong currentTime = JVM_JavaMilliSeconds(), stopTime;
  if (timeout == -1)
     { if (SDL_WaitEvent(&event))
          CheckEvent(&event, pNewSignal, pNewMidpEvent);
       return;
     }
  do { if (SDL_PollEvent(&event))
          { CheckEvent(&event, pNewSignal, pNewMidpEvent);
            return;
          }
       stopTime = JVM_JavaMilliSeconds();
     } while((stopTime - currentTime) < timeout);
}

