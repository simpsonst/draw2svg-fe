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

#ifndef options_h
#define options_h

#include <stdbool.h>

enum { S_NONE, S_SCALE, S_FIT };
enum { PAR_NO, PAR_MEET, PAR_SLICE };
enum { PAR_MIN, PAR_MID, PAR_MAX };
enum { TXT_PATH, TXT_LATIN1, TXT_FONT };
enum { SZ_NONE, SZ_PERCENT, SZ_ABS };

typedef struct {
  unsigned setxy : 1, setg : 1, autosave : 1, bg : 1, scale : 2,
    margin : 1, partype : 2, parx : 2, pary : 2, abssized : 2;
  unsigned txtmode : 2;
  char units[10], fontcfg[256], savedir[256], thinness[10];
  char scaledata[40], fitdata[40], bgdata[16], mardata[16];
} options_cfg;

#define OPTIONS_DEFAULT { 0, 0, 0, 0, S_NONE, 0, \
  PAR_MEET, PAR_MID, PAR_MID, SZ_PERCENT, TXT_PATH, \
  "in", "", "", "1", "1", "*,*", "#fff", "1cm" }

#endif
