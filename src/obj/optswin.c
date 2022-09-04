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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <kernel.h>
#include <swis.h>

#include <riscos/types.h>
#include <riscos/wimp/types.h>
#include <riscos/file/types.h>
#include <riscos/mouse/types.h>

#include <yacitros/desktop/types.h>
#include <yacitros/desktop/win.h>
#include <yacitros/desktop/event.h>
#include <yacitros/desktop/task.h>
#include <yacitros/desktop/label.h>

#include "context.h"
#include "options.h"
#include "optswin.h"
#include "optsunit.h"
#include "config.h"
#include "icnums.h"

static int ensurewrite(const char *name)
{
  char nextname[300];
  const char *ptr = name;
  const char *end = strrchr(name, '.');
  if (!end) end = strrchr(name, ':');
  if (!end) end = name + strlen(name);

  while ((ptr = strchr(ptr, '.')) != NULL && ptr <= end) {
    int type;
    memcpy(nextname, name, ptr - name);
    nextname[ptr - name] = '\0';
    _swix(OS_File, _INR(0,1)|_OUT(0), 17, nextname, &type);
    switch (type) {
    case 1:
      return -1;
    case 0:
      if (_swix(OS_File, _INR(0,1)|_IN(4), 8, nextname, 4))
        return -1;
      break;
    }
    ptr++;
  }
  return 0;
}

int optswin_load(void *vr, wimp_iconhandle ih, const vdu_point *pos,
                 long sz, file_type type, const char *name)
{
  program_data *r = vr;
  size_t len;
  const char *dot;
  int ot;

  task_complain(_swix(OS_File, _INR(0,1)|_OUT(0), 17, (int) name, &ot));

  switch (ot) {
    /* directory or image: use supplied name */
  case 2: /* directory */
  case 3: /* image */
    switch (ih) {
    case TEXT_FONTOPT:
    case TEXT_FONTCFG:
      win_setfield(r->optswin, TEXT_FONTCFG, name);
      return true;
    case AUTOSAVE_FLAG:
    case AUTOSAVE_VALUE:
      win_setfield(r->optswin, AUTOSAVE_VALUE, name);
      return true;
    }
    return false;

    /* other files: use parent directory */
  case 1:
    dot = strrchr(name, '.');
    if (!dot) dot = strrchr(name, ':');
    if (!dot) return false;
    len = dot - name;

    switch (ih) {
    case TEXT_FONTOPT:
    case TEXT_FONTCFG:
      win_setfieldn(r->optswin, TEXT_FONTCFG, name, len);
      return true;
    case AUTOSAVE_FLAG:
    case AUTOSAVE_VALUE:
      win_setfieldn(r->optswin, AUTOSAVE_VALUE, name, len);
      return true;
    }
    return false;
  }
  return false;
}

void optswin_setfields(program_data *pd)
{
  win_selecticon(pd->optswin, POS_FLAG, pd->opts->setxy);
  win_selecticon(pd->optswin, GROUP_FLAG, pd->opts->setg);
  win_selecticon(pd->optswin, BACK_FLAG, pd->opts->bg);
  win_selecticon(pd->optswin, MARGIN_FLAG, pd->opts->margin);
  win_selecticon(pd->optswin, SCALE_FLAG, pd->opts->scale == S_SCALE);
  win_selecticon(pd->optswin, FIT_FLAG, pd->opts->scale == S_FIT);
  win_selecticon(pd->optswin, NOSCALE_FLAG, pd->opts->scale == S_NONE);

  win_selecticon(pd->optswin, SIZE_ABSOLUTE, pd->opts->abssized == SZ_ABS);
  win_selecticon(pd->optswin, SIZE_NONE, pd->opts->abssized == SZ_NONE);
  win_selecticon(pd->optswin, SIZE_PERCENT, pd->opts->abssized == SZ_PERCENT);

  win_selecticon(pd->optswin, ASPRAT_NO, pd->opts->partype == PAR_NO);
  win_selecticon(pd->optswin, ASPRAT_SLICE, pd->opts->partype == PAR_SLICE);
  win_selecticon(pd->optswin, ASPRAT_MEET, pd->opts->partype == PAR_MEET);

  win_selecticon(pd->optswin, TEXT_LATIN1OPT,
                 pd->opts->txtmode == TXT_LATIN1);
  win_selecticon(pd->optswin, TEXT_PATHOPT, pd->opts->txtmode == TXT_PATH);
  win_selecticon(pd->optswin, TEXT_FONTOPT, pd->opts->txtmode == TXT_FONT);

  win_selecticon(pd->optswin, ASPRAT_XMIN, pd->opts->parx == PAR_MIN);
  win_selecticon(pd->optswin, ASPRAT_XMID, pd->opts->parx == PAR_MID);
  win_selecticon(pd->optswin, ASPRAT_XMAX, pd->opts->parx == PAR_MAX);

  win_selecticon(pd->optswin, ASPRAT_YMIN, pd->opts->pary == PAR_MIN);
  win_selecticon(pd->optswin, ASPRAT_YMID, pd->opts->pary == PAR_MID);
  win_selecticon(pd->optswin, ASPRAT_YMAX, pd->opts->pary == PAR_MAX);

  win_selecticon(pd->optswin, AUTOSAVE_FLAG, pd->opts->autosave);
  win_setfield(pd->optswin, THIN_VALUE, pd->opts->thinness);
  win_setfield(pd->optswin, TEXT_FONTCFG, pd->opts->fontcfg);
  win_setfield(pd->optswin, AUTOSAVE_VALUE, pd->opts->savedir);
  win_setfield(pd->optswin, UNITS_VALUE, pd->opts->units);
  win_setfield(pd->optswin, SCALE_VALUE, pd->opts->scaledata);
  win_setfield(pd->optswin, FIT_VALUE, pd->opts->fitdata);
  win_setfield(pd->optswin, BACK_VALUE, pd->opts->bgdata);
  win_setfield(pd->optswin, MARGIN_VALUE, pd->opts->mardata);
  win_enableicon(pd->optswin, TEXT_FONTCFG, false);
  win_enableicon(pd->optswin, TEXT_FONTOPT, false);
}

void optswin_getfields(program_data *pd)
{
  int tmp;

  if (win_iconselected(pd->optswin, POS_FLAG, &tmp) == 0)
    pd->opts->setxy = tmp;
  if (win_iconselected(pd->optswin, GROUP_FLAG, &tmp) == 0)
    pd->opts->setg = tmp;
  if (win_iconselected(pd->optswin, BACK_FLAG, &tmp) == 0)
    pd->opts->bg = tmp;
  if (win_iconselected(pd->optswin, MARGIN_FLAG, &tmp) == 0)
    pd->opts->margin = tmp;
  if (win_iconselected(pd->optswin, SIZE_ABSOLUTE, &tmp) == 0 && tmp)
    pd->opts->abssized = SZ_ABS;
  else if (win_iconselected(pd->optswin, SIZE_PERCENT, &tmp) == 0 && tmp)
    pd->opts->abssized = SZ_PERCENT;
  else if (win_iconselected(pd->optswin, SIZE_NONE, &tmp) == 0 && tmp)
    pd->opts->abssized = SZ_NONE;
  if (win_iconselected(pd->optswin, SCALE_FLAG, &tmp) == 0 && tmp)
    pd->opts->scale = S_SCALE;
  else if (win_iconselected(pd->optswin, NOSCALE_FLAG, &tmp) == 0 && tmp)
    pd->opts->scale = S_NONE;
  else if (win_iconselected(pd->optswin, FIT_FLAG, &tmp) == 0 && tmp)
    pd->opts->scale = S_FIT;
  if (win_iconselected(pd->optswin, ASPRAT_NO, &tmp) == 0 && tmp)
    pd->opts->partype = PAR_NO;
  else if (win_iconselected(pd->optswin, ASPRAT_MEET, &tmp) == 0 && tmp)
    pd->opts->partype = PAR_MEET;
  else if (win_iconselected(pd->optswin, ASPRAT_SLICE, &tmp) == 0 && tmp)
    pd->opts->partype = PAR_SLICE;

  if (win_iconselected(pd->optswin, TEXT_LATIN1OPT, &tmp) == 0 && tmp)
    pd->opts->txtmode = TXT_LATIN1;
  else if (win_iconselected(pd->optswin, TEXT_PATHOPT, &tmp) == 0 && tmp)
    pd->opts->txtmode = TXT_PATH;
  else if (win_iconselected(pd->optswin, TEXT_FONTOPT, &tmp) == 0 && tmp)
    pd->opts->txtmode = TXT_FONT;

  if (win_iconselected(pd->optswin, ASPRAT_XMIN, &tmp) == 0 && tmp)
    pd->opts->parx = PAR_MIN;
  else if (win_iconselected(pd->optswin, ASPRAT_XMID, &tmp) == 0 && tmp)
    pd->opts->parx = PAR_MID;
  else if (win_iconselected(pd->optswin, ASPRAT_XMAX, &tmp) == 0 && tmp)
    pd->opts->parx = PAR_MAX;

  if (win_iconselected(pd->optswin, ASPRAT_YMIN, &tmp) == 0 && tmp)
    pd->opts->pary = PAR_MIN;
  else if (win_iconselected(pd->optswin, ASPRAT_YMID, &tmp) == 0 && tmp)
    pd->opts->pary = PAR_MID;
  else if (win_iconselected(pd->optswin, ASPRAT_YMAX, &tmp) == 0 && tmp)
    pd->opts->pary = PAR_MAX;

  if (win_iconselected(pd->optswin, AUTOSAVE_FLAG, &tmp) == 0)
    pd->opts->autosave = tmp;
  win_getfield(pd->optswin, THIN_VALUE, pd->opts->thinness,
               sizeof pd->opts->thinness);
  win_getfield(pd->optswin, TEXT_FONTCFG,
               pd->opts->fontcfg, sizeof pd->opts->fontcfg);
  win_getfield(pd->optswin, AUTOSAVE_VALUE,
               pd->opts->savedir, sizeof pd->opts->savedir);
  win_getfield(pd->optswin, UNITS_VALUE,
               pd->opts->units, sizeof pd->opts->units);
  win_getfield(pd->optswin, SCALE_VALUE,
               pd->opts->scaledata, sizeof pd->opts->scaledata);
  win_getfield(pd->optswin, FIT_VALUE,
               pd->opts->fitdata, sizeof pd->opts->fitdata);
  win_getfield(pd->optswin, BACK_VALUE,
               pd->opts->bgdata, sizeof pd->opts->bgdata);
  win_getfield(pd->optswin, MARGIN_VALUE,
               pd->opts->mardata, sizeof pd->opts->mardata);
}

event_rspcode optswin_open(void *vr, const wimp_winloc *loc)
{
  program_data *r = vr;
  if (win_openat(r->optswin, loc) < 0)
    task_complain(task_err());
  return event_CLAIM;
}

event_rspcode optswin_close(void *vr)
{
  program_data *r = vr;
  optswin_setfields(r);
  win_close(r->optswin);
  return event_CLAIM;
}

const char *optswin_help(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;
  switch (i) {
  case OKAY_BUTTON:
    return label_default(&r->labels, "ih_opts_okay",
                         "\\Sconfirm your changes.");
  case RESTORE_BUTTON:
    return label_default(&r->labels, "ih_opts_restore",
                         "\\Srestore previous options.");
  case CANCEL_BUTTON:
    return label_default(&r->labels, "ih_opts_cancel",
                         "\\Scancel any changes.");
  case SAVE_BUTTON:
    return label_default(&r->labels, "ih_opts_save",
                         "\\Ssave the options.");

  case TEXT_LATIN1OPT:
    return label_default(&r->labels, "ih_opts_txtlatin1",
                         "This selects conversion of text assuming"
                         " Latin-1 encoding.");
  case TEXT_PATHOPT:
    return label_default(&r->labels, "ih_opts_txtpath",
                         "This selects conversion of text to paths.");
  case TEXT_FONTOPT:
    return label_default(&r->labels, "ih_opts_txtfont",
                         "This selects conversion of text using a"
                         " font-configuration file.");

  case POS_FLAG:
    return label_default(&r->labels, "ih_opts_pos",
                         "This selects whether the output is positioned.");
  case SIZE_ABSOLUTE:
    return label_default(&r->labels, "ih_opts_abssize",
                         "This makes the output have an absolute size.");
  case SIZE_PERCENT:
    return label_default(&r->labels, "ih_opts_pcsize",
                         "This makes the output have a size expressed"
                         " as percentages.");
  case SIZE_NONE:
    return label_default(&r->labels, "ih_opts_nosize",
                         "This makes the output have no size.");
  case GROUP_FLAG:
    return label_default(&r->labels, "ih_opts_group",
                         "This selects whether groups are converted.");

  case BACK_FLAG:
    return label_default(&r->labels, "ih_opts_bg",
                         "This selects whether a background is added.");
  case MARGIN_FLAG:
    return label_default(&r->labels, "ih_opts_margin", "This selects "
                        "whether a margin is added after scaling.");

  case ASPRAT_NO:
    return label_default(&r->labels, "ih_opts_arno", "This selects "
                        "no aspect-ratio preservation.");
  case ASPRAT_MEET:
    return label_default(&r->labels, "ih_opts_armeet", "This selects "
                        "aspect-ratio preservation by meeting.");
  case ASPRAT_SLICE:
    return label_default(&r->labels, "ih_opts_arslice", "This selects "
                        "no aspect-ratio preservation by slicing.");

  case ASPRAT_XMIN:
    return label_default(&r->labels, "ih_opts_xmin", "This selects "
                        "left alignment.");
  case ASPRAT_XMID:
    return label_default(&r->labels, "ih_opts_xmid", "This selects "
                        "centre alignment.");
  case ASPRAT_XMAX:
    return label_default(&r->labels, "ih_opts_xmax", "This selects "
                        "right alignment.");

  case ASPRAT_YMIN:
    return label_default(&r->labels, "ih_opts_ymin", "This selects "
                        "top alignment.");
  case ASPRAT_YMID:
    return label_default(&r->labels, "ih_opts_ymid", "This selects "
                        "middle alignment.");
  case ASPRAT_YMAX:
    return label_default(&r->labels, "ih_opts_ymax", "This selects "
                        "bottom alignment.");

  case FIT_FLAG:
    return label_default(&r->labels, "ih_opts_fit", "This selects "
                        "scaling-to-fit.");
  case SCALE_FLAG:
    return label_default(&r->labels, "ih_opts_scale", "This selects "
                        "scaling-by-factor.");
  case NOSCALE_FLAG:
    return label_default(&r->labels, "ih_opts_sfnone", "This deselects "
                        "scaling.");

  case THIN_VALUE:
    return label_default(&r->labels, "ih_opts_thin",
                         "This is the width of thin lines.");
  case UNITS_VALUE:
    return label_default(&r->labels, "ih_opts_units",
                         "This is the preferred unit.");
  case TEXT_FONTCFG:
    return label_default(&r->labels, "ih_opts_fontcfg",
                         "This is the font-configuration file.");
  case AUTOSAVE_FLAG:
    return label_default(&r->labels, "ih_opts_autosave", "This selects "
                         "automatic saving to the autosave directory.");
  case AUTOSAVE_VALUE:
    return label_default(&r->labels, "ih_opts_savedir",
                         "This is the autosave directory.");
  case UNITS_MENU:
    return label_default(&r->labels, "ih_opts_unitmenu",
                         "\\Schange the preferred units.");
  case BACK_VALUE:
    return label_default(&r->labels, "ih_opts_bgdata",
                         "This is the background colour (\"#rrggbb\").");
  case MARGIN_VALUE:
    return label_default(&r->labels, "ih_opts_mardata", "This is the "
                         "margin size (\"size\" or \"width,height\").");
  case FIT_VALUE:
    return label_default(&r->labels, "ih_opts_fitdata", "Fit to "
                         "width (\"width,*\"), height (\"*,height\") "
                         "or both (\"width,height\").");
  case SCALE_VALUE:
    return label_default(&r->labels, "ih_opts_scaledata",
                         "This is the scale factor (\"x[,y]\").");
  }
  return label_default(&r->labels, "wr_opts_bg",
                       "These are the Draw-to-SVG conversion options.");
}

/* Test whether the new configuration system is in use.  Return true
   if it is (i.e. <Choices$Write> is set). */
static bool newchoices(void)
{
  return !!getenv(CFGVARNAME);
#if 0
  int len;
  _kernel_oserror *er =
    _swix(OS_ReadVarVal, _INR(0,4)|_OUT(2),
          CFGVARNAME, 0, 0x80000000, 0, 0,
          &len);
  if (er)
    task_error(false, "Len: %d Error %d: %s", len, er->errnum, er->errmess);
  return er == NULL && len != 0;
#endif
}

void optswin_loaddata(program_data *pd)
{
  FILE *fin;
  const char *name;

  if (newchoices()) {
    /* using new boot sequence */
    name = BOOTCFGFILE;
    if (_swix(OS_File, _INR(0,1), 5, LOCALCFGFILE)) {
      /* old configuration file exists, so move it */
      if (_swix(OS_FSControl, _INR(0,3), 26, LOCALCFGFILE, WRITECFGFILE,
                (1<<7)|(1<<1)))
        task_error(false, "Could not auto-upgrade choices.");
    }
  } else {
    /* using old boot sequence */
    name = LOCALCFGFILE;
  }
  fin = fopen(name, "r");
  if (fin) {
    char line[500];
    char *name, *value, *ptr;

    while (fgets(line, sizeof line, fin)) {
      for (name = line; *name && isspace(*name); name++)
        ;
      for (ptr = name; *ptr && !isspace(*ptr); ptr++)
        ;
      *ptr++ = '\0';
      for (value = ptr; *value && isspace(*value); value++)
        ;
      for (ptr = value; *ptr && *ptr != '\n'; ptr++)
        ;
      *ptr++ = '\0';
      for ( ; ptr > value && isspace(ptr[-1]); ptr--)
        ;
      *ptr = '\0';
      if (!strcmp(name, "setxy")) {
        pd->opts->setxy = !strcmp(value, "true");
      } else if (!strcmp(name, "setg")) {
        pd->opts->setg = !strcmp(value, "true");
      } else if (!strcmp(name, "margin")) {
        pd->opts->margin = !strcmp(value, "true");
      } else if (!strcmp(name, "bg")) {
        pd->opts->bg = !strcmp(value, "true");
      } else if (!strcmp(name, "autosave")) {
        pd->opts->autosave = !strcmp(value, "true");
      } else if (!strcmp(name, "abssize")) {
        if (!strcmp(value, "false") || !strcmp(value, "none"))
          pd->opts->abssized = SZ_NONE;
        else if (!strcmp(value, "true") || !strcmp(value, "abs"))
          pd->opts->abssized = SZ_ABS;
        else
          pd->opts->abssized = SZ_PERCENT;
      } else if (!strcmp(name, "scale")) {
        if (!strcmp(value, "scale"))
          pd->opts->scale = S_SCALE;
        else if (!strcmp(value, "fit"))
          pd->opts->scale = S_FIT;
        else
          pd->opts->scale = S_NONE;
      } else if (!strcmp(name, "text")) {
        if (!strcmp(value, "font"))
          pd->opts->txtmode = TXT_FONT;
        else if (!strcmp(value, "latin1"))
          pd->opts->txtmode = TXT_LATIN1;
        else
          pd->opts->txtmode = TXT_PATH;
      } else if (!strcmp(name, "xalign")) {
        if (!strcmp(value, "xmin"))
          pd->opts->parx = PAR_MIN;
        else if (!strcmp(value, "xmax"))
          pd->opts->parx = PAR_MAX;
        else
          pd->opts->parx = PAR_MID;
      } else if (!strcmp(name, "yalign")) {
        if (!strcmp(value, "ymin"))
          pd->opts->pary = PAR_MIN;
        else if (!strcmp(value, "ymax"))
          pd->opts->pary = PAR_MAX;
        else
          pd->opts->pary = PAR_MID;
      } else if (!strcmp(name, "asprat")) {
        if (!strcmp(value, "slice"))
          pd->opts->partype = PAR_SLICE;
        else if (!strcmp(value, "none"))
          pd->opts->partype = PAR_NO;
        else
          pd->opts->partype = PAR_MEET;
      } else if (!strcmp(name, "units")) {
        strncpy(pd->opts->units, value, sizeof pd->opts->units);
      } else if (!strcmp(name, "savedir")) {
        strncpy(pd->opts->savedir, value, sizeof pd->opts->savedir);
      } else if (!strcmp(name, "fontcfg")) {
        strncpy(pd->opts->fontcfg, value, sizeof pd->opts->fontcfg);
      } else if (!strcmp(name, "thinness")) {
        strncpy(pd->opts->thinness, value, sizeof pd->opts->thinness);
      } else if (!strcmp(name, "mardata")) {
        strncpy(pd->opts->mardata, value, sizeof pd->opts->mardata);
      } else if (!strcmp(name, "scaledata")) {
        strncpy(pd->opts->scaledata, value, sizeof pd->opts->scaledata);
      } else if (!strcmp(name, "fitdata")) {
        strncpy(pd->opts->fitdata, value, sizeof pd->opts->fitdata);
      } else if (!strcmp(name, "bgdata")) {
        strncpy(pd->opts->bgdata, value, sizeof pd->opts->bgdata);
      } else {
      }
    }
    fclose(fin);
  }
}

void optswin_savedata(program_data *pd)
{
  FILE *fout;
  const char *name = newchoices() ? WRITECFGFILE : LOCALCFGFILE;

  if (ensurewrite(name) < 0)
    task_error(false, "Can't create choices directory %s", name);
  fout = fopen(name, "w");
  if (fout) {
    fprintf(fout, "setxy %s\n", pd->opts->setxy ? "true" : "false");
    fprintf(fout, "setg %s\n", pd->opts->setg ? "true" : "false");
    fprintf(fout, "abssize %s\n", pd->opts->abssized == SZ_NONE ? "none" :
            pd->opts->abssized == SZ_ABS ? "abs" : "percent");
    fprintf(fout, "scale %s\n", pd->opts->scale == S_SCALE ? "scale" :
            (pd->opts->scale == S_FIT ? "fit" : "none"));
    fprintf(fout, "text %s\n", pd->opts->txtmode == TXT_FONT ? "font" :
            (pd->opts->txtmode == TXT_LATIN1 ? "latin1" : "path"));
    fprintf(fout, "asprat %s\n", pd->opts->partype == PAR_SLICE ? "slice" :
            (pd->opts->partype == PAR_NO ? "none" : "meet"));
    fprintf(fout, "xalign %s\n", pd->opts->parx == PAR_MIN ? "xmin" :
            (pd->opts->parx == PAR_MAX ? "xmax" : "xmid"));
    fprintf(fout, "yalign %s\n", pd->opts->pary == PAR_MIN ? "ymin" :
            (pd->opts->pary == PAR_MAX ? "ymax" : "ymid"));
    fprintf(fout, "bg %s\n", pd->opts->bg ? "true" : "false");
    fprintf(fout, "margin %s\n", pd->opts->margin ? "true" : "false");
    fprintf(fout, "autosave %s\n", pd->opts->autosave ? "true" : "false");
    fprintf(fout, "savedir %.*s\n",
            (int) sizeof pd->opts->savedir, pd->opts->savedir);
    fprintf(fout, "units %.*s\n",
            (int) sizeof pd->opts->units, pd->opts->units);
    fprintf(fout, "thinness %.*s\n",
            (int) sizeof pd->opts->thinness, pd->opts->thinness);
    fprintf(fout, "fontcfg %.*s\n",
            (int) sizeof pd->opts->fontcfg, pd->opts->fontcfg);
    fprintf(fout, "scaledata %.*s\n",
            (int) sizeof pd->opts->scaledata, pd->opts->scaledata);
    fprintf(fout, "fitdata %.*s\n",
            (int) sizeof pd->opts->fitdata, pd->opts->fitdata);
    fprintf(fout, "bgdata %.*s\n",
            (int) sizeof pd->opts->bgdata, pd->opts->bgdata);
    fprintf(fout, "mardata %.*s\n",
            (int) sizeof pd->opts->mardata, pd->opts->mardata);
    fclose(fout);
  }
}

event_rspcode optswin_click(void *vr, const mouse_state *ms, wimp_iconhandle i)
{
  program_data *r = vr;

  switch (i) {
  case OKAY_BUTTON:
    /* get the options and close the window */
    optswin_getfields(r);
    if (!ms->buttons.bits.adjust0)
      win_close(r->optswin);
    return event_CLAIM;
  case RESTORE_BUTTON:
    /* reload the options */
    optswin_loaddata(r);
    /* reflect the reload */
    optswin_setfields(r);
    return event_CLAIM;
  case CANCEL_BUTTON:
    /* cancel any changes */
    optswin_setfields(r);
    win_close(r->optswin);
    return event_CLAIM;
  case SAVE_BUTTON:
    /* get and save the options */
    optswin_getfields(r);
    optswin_savedata(r);
    return event_CLAIM;
  case UNITS_MENU:
    /* open the units menu -64+40 */
#if 1
    if (win_openiconmenu(r->optswin, i, &ms->pos,
                         &optswin_unitgen,
                         &optswin_unitsel,
                         &optswin_unitop,
                         &optswin_unithelp,
                         r) < 0)
      task_complain(task_err());
#else
    if (win_openiconmenu(r->optswin, i, &ms->pos,
                         &optswin_unitgen,
                         NULL, NULL, NULL,
                         r) < 0)
      task_complain(task_err());
#endif
    return event_CLAIM;
  }
  return event_SHIRK;
}
