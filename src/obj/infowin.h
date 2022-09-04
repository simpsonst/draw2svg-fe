// -*- c-basic-offset: 2; indent-tabs-mode: nil -*-

/*
   Draw2SVG front-end: RISC OS WIMP program convert Drawfiles into SVG
   Copyright (C) 2001,2003,2005-7,2012,2019  Steven Simpson

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


   Author contact: Email to s.simpson@lancaster.ac.uk
*/

#ifndef infowin_h
#define infowin_h

#include <riscos/wimp/types.h>
#include <riscos/mouse/types.h>

#include <yacitros/desktop/types.h>

event_rspcode infowin_open(void *vr, const wimp_winloc *loc);
event_rspcode infowin_close(void *vr);
const char *infowin_help(void *vr, const mouse_state *ms, wimp_iconhandle i);
event_rspcode infowin_click(void *, const mouse_state *, wimp_iconhandle);

#endif
