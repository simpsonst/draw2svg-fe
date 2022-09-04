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

#include <kernel.h>
#include <swis.h>
#include <riscos/swi/OS.h>

#include <riscos/wimp/types.h>
#include <riscos/wimp/event.h>

#include <yacitros/desktop/label.h>
#include <yacitros/desktop/task.h>
#include <yacitros/desktop/event.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/menu.h>

#include "context.h"
#include "baricon.h"
#include "convert.h"

static event_rspcode nullevents(void *vr, wimp_eventtype t, void *data)
{
  program_data *r = vr;
  if (t == wimp_Null_Reason_Code) {
    if (r->nulls > 0)
      r->nulls--;
    if (r->nulls == 0) {
      event_deselect(1<<wimp_Null_Reason_Code);
      event_cancel(r->nulls_eph);
      r->nulls_eph = NULL;
    }
  }
  return event_SHIRK;
}

const char *baricon_help(void *vr, const mouse_state *m, wimp_iconhandle ih)
{
  program_data *r = vr;
  return label_default(&r->labels, "ih_ibar", "\\TDraw-to-SVG converter.|M"
                       "\\Sto see the options \\w.|M"
                       "\\Dconvert it to SVG \\w.");
}

int baricon_load(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, const char *name)
{
  program_data *r = vr;

  if (type != 0xaff)
    return false;

  if (r->opts->autosave) {
    char oname[300], *ptr;
    const char *tail, *ext;

    if (r->opts->autosave) {
      int dt;
      if (!task_complain(_swix(OS_File, _INR(0,1)|_OUT(0),
                               17, r->opts->savedir, &dt))) {
        switch (dt) {
        case 2:
        case 3:
          break;
        case 0:
        case 1:
          if (r->nulls == 0) {
            task_error(false, dt ? "\"%s\" is not a directory." :
                       "The save directory \"%s\" does not exist.",
                       r->opts->savedir);
            if (r->nulls_eph == NULL) {
              r->nulls += 1;
              r->nulls_eph = event_setpre(&nullevents, r);
              event_select(1<<wimp_Null_Reason_Code);
            }
          }
          return false;
        }
      }
    }

    /* try to guess the output file name from the input leaf
       and the configured field */
    strncpy(oname, r->opts->savedir, sizeof oname);
    ptr = oname + strlen(oname);
    *ptr++ = '.';
    tail = strrchr(name, '.');
    tail = tail ? tail + 1 : name;
    ext = strrchr(tail, '/');
    if (!ext) ext = tail + strlen(tail);
    for (; ptr + 5 < oname + sizeof oname && tail < ext; )
      *ptr++ = *tail++;
    strcpy(ptr, "/svg");
    convert(r->opts, name, oname);
    return true;
  } else {
    char tmp[100], *ptr;
    const char *tail, *ext;
    /* record the input file name */
    strncpy(r->curfile, name, sizeof r->curfile);

    /* guess a sensible output name */
    tail = strrchr(name, '.');
    tail = tail ? tail + 1 : name;
    ext = strrchr(tail, '/');
    if (!ext) ext = tail + strlen(tail);
    for (ptr = tmp; ptr + 5 < tmp + sizeof tmp && tail < ext; )
      *ptr++ = *tail++;
    strcpy(ptr, "/svg");
    win_setfield(r->savewin, 2, tmp);

    /* open the save window */
    if (win_openmenu(r->savewin, pos->x - 64, pos->y - 64) < 0)
      task_complain(task_err());
    return true;
  }
  return false;
}

int baricon_save(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, char *name, int len)
{
  strncpy(name, "<Wimp$Scrap>", len);
  return false; /* unsafe */
}

void baricon_menusel(void *vh, const wimp_menuchoice *c)
{
  program_data *h = vh;
  switch (c[0]) {
  case 0:
    break;
  case 1:
    if (win_open(h->optswin) < 0)
      task_complain(task_err());
    break;
  case 2:
    h->quit = true;
    break;
  }
}

void baricon_menuop(void *vh, const wimp_menuchoice *c, const vdu_point *pos)
{
  program_data *h = vh;
  switch (c[0]) {
  case 0:
    if (win_opensubmenu(h->infowin, pos->x, pos->y) < 0)
      task_complain(task_err());
    break;
  }
}

menu_handle baricon_menugen(void *vh, wimp_iconhandle ih, vdu_point *pos)
{
  program_data *h = vh;
  if (pos) {
    pos->x -= 64;
    pos->y = 96 + menu_height(h->menu);
  }
  return h->menu;
}

const char *baricon_menuhelp(void *vh, const wimp_menuchoice *c)
{
  program_data *h = vh;
  switch (c[0]) {
  case 0:
    return label_default(&h->labels, "mh_ibar_info",
                         "\\Rget information about "
                         "this version of the Draw-to-SVG converter.");
  case 1:
    return label_default(&h->labels, "mh_ibar_new",
                         "\\Sopen see the options.");
  case 2:
    return label_default(&h->labels, "mh_ibar_quit",
                         "\\Squit the Draw-to-SVG converter.");
  }
  return NULL;
}
