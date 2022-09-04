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

#include <string.h>

#include <riscos/vdu/types.h>
#include <riscos/wimp/types.h>
#include <riscos/wimp/op.h>
#include <riscos/mouse/types.h>

#include <yacitros/desktop/types.h>
#include <yacitros/desktop/label.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/transmit.h>
#include <yacitros/desktop/menu.h>
#include <yacitros/desktop/event.h>
#include <yacitros/desktop/task.h>

#include "context.h"
#include "savewin.h"
#include "convert.h"

static const char *get_leafname(const char *s)
{
  const char *leafname = strrchr(s, '.');
  if (leafname) {
    return ++leafname;
  } else {
    leafname = strchr(s, ':');
    if (leafname) {
      return ++leafname;
    } else {
      return s;
    }
  }
}

int savewin_deposit(void *vc, const char *n, int safe)
{
  program_data *c = vc;
  if (!strchr(n, ':')) {
    task_error(false, "%s",
               label_default(&c->labels, "em_dragsave",
                             "To save, drag the icon to"
                             " a directory display"));
    return false;
  }
  convert(c->opts, c->curfile, n);
  c->curfile[0] = '\0';
  menu_close();
  return true;
}

void savewin_drag(void *vc, const vdu_box *a)
{
  program_data *c = vc;
  wimp_mouseevent me;
  char newname[300];
  const char *leafname;
  _kernel_oserror *oe;

  win_getfield(c->savewin, 2, newname, sizeof newname);
  leafname = get_leafname(newname);
  if ((oe = wimp_getpointerinfo(&me)) != NULL) {
    task_complain(oe);
    return;
  }

  /* initiate the data-save protocol to find out where the
     user wants to save the data */
  transmit_open(&c->xferdata, leafname, 100, 0xaad,
                &me.mouse.pos, me.wh, me.ih, c,
                &savewin_deposit, NULL);
}

event_rspcode savewin_click(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;
  if (i == 0 && (ms->buttons.bits.select0 ||
                 ms->buttons.bits.adjust0)) {
    char tmp[300];
    win_getfield(r->savewin, 2, tmp, sizeof tmp);
    savewin_deposit(r, tmp, true);
    return event_CLAIM;
  } else if (i == 3 && (ms->buttons.bits.select1 ||
                        ms->buttons.bits.adjust1)) {
    win_dragiconpos(r->savewin, &savewin_drag, r, 3);
    return event_CLAIM;
  }
  return event_SHIRK;
}

const char *savewin_help(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;

  switch (i) {
  case 0: /* okay */
    return label_default(&r->labels, "ih_save_okay",
                         "Click here to save the data with the current"
                         " filename. If it is not a full pathname, you"
                         " must drag the icon to a directory display "
                         "first.");
  case 2: /* filename */
    return label_default(&r->labels, "ih_save_filename",
                         "This shows the filename for this data. If it"
                         " is not a full pathname, drag "
                         "the icon into a directory display.");
  case 3: /* icon */
    return label_default(&r->labels, "ih_save_file",
                         "Drag the icon to the directory in which you want"
                         " to save the file|MOr, drag it to the program"
                         " into which you want to transfer the data.");
  }
  return label_default(&r->labels, "wa_save", "The converter wants to"
                       " know where you want to store the SVG file.");
}

event_rspcode savewin_open(void *vr, const wimp_winloc *loc)
{
  program_data *r = vr;
  if (win_openat(r->savewin, loc) < 0)
    task_complain(task_err());
  return event_CLAIM;
}

event_rspcode savewin_close(void *vr)
{
  program_data *r = vr;
  win_close(r->savewin);
  r->curfile[0] = '\0';
  return event_CLAIM;
}
