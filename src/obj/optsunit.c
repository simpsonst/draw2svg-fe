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

#include <riscos/vdu/types.h>
#include <riscos/wimp/types.h>

#include <yacitros/desktop/types.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/label.h>

#include "context.h"
#include "optsunit.h"
#include "icnums.h"

void optswin_unitop(void *vh, const wimp_menuchoice *c,
                    const vdu_point *pos)
{
}

menu_handle optswin_unitgen(void *vh, wimp_iconhandle ih, vdu_point *pos)
{
  program_data *h = vh;
  if (pos) {
    pos->x -= 64;
    pos->y += 40;
  }
  return h->unitmenu;
}

const char *optswin_unithelp(void *vh, const wimp_menuchoice *c)
{
  program_data *h = vh;
  switch (c[0]) {
  case 0:
    return label_default(&h->labels, "mh_unit_cm",
                         "\\Schoose centimetres as the preferred units.");
  case 1:
    return label_default(&h->labels, "mh_unit_mm",
                         "\\Schoose millimetres as the preferred units.");
  case 2:
    return label_default(&h->labels, "mh_unit_in",
                         "\\Schoose inches as the preferred units.");
  case 3:
    return label_default(&h->labels, "mh_unit_pt",
                         "\\Schoose points as the preferred units.");
  }
  return NULL;
}

void optswin_unitsel(void *vh, const wimp_menuchoice *c)
{
  program_data *h = vh;
  switch (c[0]) {
  case 0:
    win_setfield(h->optswin, UNITS_VALUE, "cm");
    break;
  case 1:
    win_setfield(h->optswin, UNITS_VALUE, "mm");
    break;
  case 2:
    win_setfield(h->optswin, UNITS_VALUE, "in");
    break;
  case 3:
    win_setfield(h->optswin, UNITS_VALUE, "pt");
    break;
  }
}
