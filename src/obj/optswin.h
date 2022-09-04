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

#ifndef optswin_h
#define optswin_h

#include <riscos/vdu/types.h>
#include <riscos/wimp/types.h>
#include <riscos/file/types.h>
#include <riscos/mouse/types.h>

#include <yacitros/desktop/types.h>

#include "context.h"

void optswin_loaddata(program_data *pd);
void optswin_setfields(program_data *pd);

int optswin_load(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, const char *name);
event_rspcode optswin_open(void *vr, const wimp_winloc *loc);
event_rspcode optswin_close(void *vr);
const char *optswin_help(void *vr, const mouse_state *ms, wimp_iconhandle i);
event_rspcode optswin_click(void *vr,
                            const mouse_state *ms, wimp_iconhandle i);

#endif
