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
#include <stdio.h>

#include <kernel.h>
#include <swis.h>
#include <riscos/swi/OS.h>

#ifdef __TARGET_UNIXLIB__
#define __UNIXLIB_INTERNALS 1
#include <unixlib/local.h>
int __riscosify_control = __RISCOSIFY_NO_PROCESS;
#endif

#include <yacitros/desktop/task.h>
#include <yacitros/desktop/event.h>
#include <yacitros/desktop/template.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/iconbar.h>
#include <yacitros/desktop/menu.h>
#include <yacitros/desktop/label.h>
#include <yacitros/desktop/transmit.h>

#include "context.h"
#include "options.h"
#include "config.h"
#include "baricon.h"
#include "infowin.h"
#include "optswin.h"
#include "savewin.h"

extern const char *const linkversion;
extern const char *const linkdate;
extern const char *const linktime;

#if 0
static void autoquit(void *vpd)
{
  program_data_t *pd = vpd;
  pd->quit = true;
}
#endif

options_cfg opts = OPTIONS_DEFAULT;
program_data app;

int main(void)
{
  template_handle tmp;

  app.nulls_eph = NULL;
  app.curfile[0] = '\0';
  app.opts = &opts;
  if (label_open(&app.labels, "<Draw2SVG$Dir>.Messages") < 0) {
    fprintf(stderr, "Failed to open messages\n");
    if (task_err())
      fprintf(stderr, "%s\n", task_err()->errmess);
    exit(EXIT_FAILURE);
  }

  app.quit = false;
  if (!task_init(label_default(&app.labels, "tn_draw2svg", "Draw-to-SVG"),
                 &app.quit)) {
    int exec_rc;
    _kernel_oserror *exec_err;

    /* check for back-end */
    if ((exec_err = _swix(OS_File, _INR(0,1)|_IN(4)|_OUT(0),
                          15, "draw2svg", "Run$Path", &exec_rc))) {
      task_complain(exec_err);
      app.quit = true;
    } else if (exec_rc != 1) {
      task_error(false, label_default(&app.labels, "em_noexec",
                 "Can't find the draw2svg program in <Run$Path>"));
      app.quit = true;
    } else {
      char versionstring[140];
      template_init("<Draw2SVG$Dir>.Templates");

      /* initialise iconbar */
      iconbar_init(&app);
      win_conf_help(iconbar, &baricon_help);
      win_conf_load(iconbar, &baricon_load);
      win_conf_save(iconbar, &baricon_save);
      app.ih = iconbar_appicon("!Draw2SVG", sprite_WIMPAREA);

      /* initialise Info window */
      tmp = template_load("info", NULL, 20, 2000);
      app.infowin = win_create(tmp, &app);
      app.infowin_work = template_release(tmp);
      win_conf_open(app.infowin, &infowin_open);
      win_conf_close(app.infowin, &infowin_close);
      win_conf_help(app.infowin, &infowin_help);
      win_conf_click(app.infowin, &infowin_click);
      sprintf(versionstring, "%s (%s)", linkversion, linkdate);
      win_setfield(app.infowin, 3, versionstring);

      /* initialise Save window */
      tmp = template_load("xfer", NULL, 20, 2000);
      app.savewin = win_create(tmp, &app);
      app.savewin_work = template_release(tmp);
      win_conf_open(app.savewin, &savewin_open);
      win_conf_close(app.savewin, &savewin_close);
      win_conf_help(app.savewin, &savewin_help);
      win_conf_click(app.savewin, &savewin_click);
      transmit_init(&app.xferdata);

      /* initialise Options window */
      tmp = template_load("Options", NULL, 50, 2000);
      app.optswin = win_create(tmp, &app);
      app.optswin_work = template_release(tmp);
      win_conf_open(app.optswin, &optswin_open);
      win_conf_close(app.optswin, &optswin_close);
      win_conf_help(app.optswin, &optswin_help);
      win_conf_click(app.optswin, &optswin_click);
      win_conf_load(app.optswin, &optswin_load);
      optswin_loaddata(&app);
      optswin_setfields(&app);

      /* create icon bar menu */
      app.menu = menu_create(label_default(&app.labels,
                                           "mt_ibar", "Draw2SVG"),
                             label_default(&app.labels, "mi_ibar",
                                           ">Info,Options...|Quit"));

      app.matt = win_attachmenu(iconbar, &baricon_menugen, &baricon_menusel,
                                &baricon_menuop, &baricon_menuhelp, &app);

      /* create units menu */
      app.unitmenu =
        menu_create(label_default(&app.labels, "mt_unit", "Units"),
                    label_default(&app.labels, "mi_unit",
                                  "Centimetres,Millimetres,Inches,Points"));

#if 0
      task_error(false, "Opening once...");
      task_complain(wimp_createmenuwindow(win_rawhandle(app.savewin),
                                          640, 512));
      task_error(false, "Opening twice...");
      task_complain(wimp_closemenu());
      task_complain(wimp_createmenuwindow(win_rawhandle(app.savewin),
                                          640, 512));
      task_error(false, "Done!");
#endif
    }

    /* event loop */
    while (!app.quit) {
      if (event_process() < 0) {
        task_complain(task_err());
        break;
      }
    }

    /* close down */

    // Fix bug when remotely operating application by sending a
    // Message_Quit while the menu and info window happen to be open.
    menu_close();

    win_destroy(app.optswin), free(app.optswin_work);
#if 1
    win_destroy(app.savewin), free(app.savewin_work);
#endif
    win_destroy(app.infowin), free(app.infowin_work);
    menu_destroy(app.unitmenu);
    win_detachmenu(app.matt);
    menu_destroy(app.menu);
    iconbar_term();
    template_term();
    task_term();
    label_close(&app.labels);
  }

  return 0;
}
