/*

    TiMidity -- Experimental MIDI to WAVE converter
    Copyright (C) 1995 Tuukka Toivonen <toivonen@clinet.fi>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


   common.h
*/

#include "SDL.h"

typedef struct {
  char *path;
  void *next;
} PathList;

// #define SNDDBG(x)	printf x
#define SNDDBG(x)	

extern SDL_RWops *open_file(char *name);
extern void add_to_pathlist(char *s);
extern void *safe_malloc(size_t count);
extern void free_pathlist(void);
