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

#include <stdio.h>

#include <riscos/vdu/hourglass.h>
#include <riscos/wimp/op.h>

#include <yacitros/desktop/task.h>

#include "options.h"
#include "convert.h"

void convert(options_cfg *r, const char *iname, const char *oname)
{
  _kernel_oserror *oe;
  char cmd[800];
  char *ptr = cmd;
  char *end = cmd + sizeof cmd - 1;
  /* int rc; */

  ptr += sprintf(ptr, "draw2svg");
  ptr += sprintf(ptr, " -u %s", r->units);
  ptr += sprintf(ptr, " --thin %s", r->thinness);
  ptr += sprintf(ptr, " %cxy", r->setxy ? '-' : '+');
  ptr += sprintf(ptr, " %cg", r->setg ? '-' : '+');
  switch (r->abssized) {
  case SZ_NONE:
    ptr += sprintf(ptr, " +z");
    break;
  case SZ_ABS:
    ptr += sprintf(ptr, " -z");
    break;
  case SZ_PERCENT:
    ptr += sprintf(ptr, " --pcsize");
    break;
  }
  switch (r->txtmode) {
  case TXT_PATH:
    ptr += sprintf(ptr, " --text-to-path");
    break;
  case TXT_LATIN1:
    break;
  }
  ptr += sprintf(ptr, " -it");
  ptr += sprintf(ptr, " -ot");
  if (r->bg)
    ptr += sprintf(ptr, " -bg %s", r->bgdata);
  if (r->margin)
    ptr += sprintf(ptr, " --margin %s", r->mardata);
  switch (r->scale) {
  case S_SCALE:
    ptr += sprintf(ptr, " --scale %s", r->scaledata);
    break;
  case S_FIT:
    ptr += sprintf(ptr, " --fit %s", r->fitdata);
    break;
  }
  switch (r->partype) {
  case PAR_NO:
    ptr += sprintf(ptr, " --noar");
    break;
  case PAR_SLICE:
    ptr += sprintf(ptr, " --slice");
    break;
  }
  switch (r->parx) {
  case PAR_MIN:
    ptr += sprintf(ptr, " --xmin");
    break;
  case PAR_MAX:
    ptr += sprintf(ptr, " --xmax");
    break;
  }
  switch (r->pary) {
  case PAR_MIN:
    ptr += sprintf(ptr, " --ymin");
    break;
  case PAR_MAX:
    ptr += sprintf(ptr, " --ymax");
    break;
  }
  ptr += sprintf(ptr, " %.*s", (int) (end - ptr) - 1, iname);
  ptr += sprintf(ptr, " %.*s", (int) (end - ptr) - 1, oname);
  (void) vdu_hourglasson();
  oe = wimp_starttask(cmd);
  (void) vdu_hourglassoff();
  task_complain(oe);
#if 0
  switch (rc) {
  case 0:
    break;
  case -2:
    task_error(false, "Possibly short of memory to run draw2svg?");
    break;
  default:
    task_error(false, "Command failed with %d, errno = %d", rc, errno);
    break;
  }
#endif
}
