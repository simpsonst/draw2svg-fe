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

#include <stdlib.h>

#include <swis.h>

#include <riscos/swi/URI.h>

#include <riscos/wimp/types.h>
#include <riscos/mouse/types.h>

#include <yacitros/desktop/types.h>
#include <yacitros/desktop/task.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/label.h>
#include <yacitros/desktop/event.h>

#include "infowin.h"
#include "context.h"

event_rspcode infowin_open(void *vr, const wimp_winloc *loc)
{
  program_data *r = vr;
  if (win_openat(r->infowin, loc) < 0)
    task_complain(task_err());
  return event_CLAIM;
}

event_rspcode infowin_close(void *vr)
{
  program_data *r = vr;
  win_close(r->infowin);
  return event_CLAIM;
}

const char *infowin_help(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;
  if (i == 8)
    return label_default(&r->labels, "ih_info_website",
                         "\\Svisit the website.");
  return label_default(&r->labels, "wh_info",
                       "This \\w displays information "
                       "about the Draw-to-SVG converter.");
}
#define DEFAULT_WEBSITE \
"https://www.lancaster.ac.uk/~simpsons/software/pkg-draw2svg"

event_rspcode infowin_click(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;

  if (i == 8) {
    const char *location =
      label_default(&r->labels, "str_website", DEFAULT_WEBSITE);
    task_complain(_swix(URI_Dispatch, _INR(0,2), 0, location, 0));
    return event_CLAIM;
  }

  return event_SHIRK;
}
