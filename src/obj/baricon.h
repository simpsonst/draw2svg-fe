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


   Author contact: <https://github.com/simpsonst>
*/

#ifndef baricon_h
#define baricon_h

#include <riscos/vdu/types.h>
#include <riscos/wimp/types.h>
#include <riscos/mouse/types.h>
#include <riscos/file/types.h>

#include <yacitros/desktop/types.h>

#include "context.h"

const char *baricon_help(void *vr, const mouse_state *m, wimp_iconhandle ih);
int baricon_load(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, const char *name);
int baricon_save(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, char *name, int len);
void baricon_menusel(void *vh, const wimp_menuchoice *c);
void baricon_menuop(void *vh, const wimp_menuchoice *c,
                    const vdu_point *pos);
menu_handle baricon_menugen(void *vh, wimp_iconhandle ih, vdu_point *pos);
const char *baricon_menuhelp(void *vh, const wimp_menuchoice *c);

#endif
