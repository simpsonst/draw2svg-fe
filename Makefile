all::

FIND=find
XARGS=xargs
SED=sed
CAT=cat
MKDIR=mkdir -p
CP=cp
CMP=cmp -s
HTML2TXT=lynx -dump
MARKDOWN=markdown

VWORDS:=$(shell src/getversion.sh --prefix=v MAJOR MINOR)
VERSION:=$(word 1,$(VWORDS))
BUILD:=$(word 2,$(VWORDS))

## Provide a version of $(abspath) that can cope with spaces in the
## current directory.
myblank:=
myspace:=$(myblank) $(myblank)
MYCURDIR:=$(subst $(myspace),\$(myspace),$(CURDIR)/)
MYABSPATH=$(foreach f,$1,$(if $(patsubst /%,,$f),$(MYCURDIR)$f,$f))

-include $(call MYABSPATH,config.mk)
-include draw2svg-fe-env.mk

CPPFLAGS += -DVERSION='"$(file <VERSION)"'

test_binaries.c += draw2svg
draw2svg_obj += main
draw2svg_obj += baricon
draw2svg_obj += convert
draw2svg_obj += infowin
draw2svg_obj += optsunit
draw2svg_obj += optswin
draw2svg_obj += savewin
draw2svg_obj += version
draw2svg_lib += -lyac_dsktop
draw2svg_lib += -lyac_wimp
draw2svg_lib += -lyac_vdu
draw2svg_lib += -lyac_os
draw2svg_lib += -lyac_sprite

#TEXTFILES += HISTORY
TEXTFILES += VERSION
TEXTFILES += COPYING
TEXTFILES += Manual

SOURCES:=$(filter-out $(headers),$(shell $(FIND) src/obj \( -name "*.c" -o -name "*.h" -o -name "*.hh" \) -printf '%P\n'))

riscos_zips += draw2svg-fe
draw2svg-fe_zrof += !ReadMe,fff
draw2svg-fe_appname=!Draw2SVG
draw2svg-fe_rof += !Boot,feb
draw2svg-fe_rof += !Help,feb
draw2svg-fe_rof += !Run,feb
draw2svg-fe_rof += !Sprites,ff9
draw2svg-fe_rof += !Sprites22,ff9
draw2svg-fe_rof += Messages,fff
draw2svg-fe_rof += Templates,fec
draw2svg-fe_rof += $(TEXTFILES:%=Docs/%,fff)
draw2svg-fe_rof += $(call riscos_src,$(SOURCES))
draw2svg-fe_runimage += draw2svg

include binodeps.mk

all:: VERSION BUILD riscos-zips

install:: install-riscos

tidy::
	$(FIND) . -name "*~" -delete

distclean: blank
	$(RM) VERSION BUILD

tmp/obj/version.o: VERSION

$(BINODEPS_OUTDIR)/riscos/!Draw2SVG/Docs/COPYING,fff: LICENSE.txt
	$(MKDIR) "$(@D)"
	$(CP) --reflink=auto "$<" "$@"

$(BINODEPS_OUTDIR)/riscos/!Draw2SVG/Docs/VERSION,fff: VERSION
	$(MKDIR) "$(@D)"
	$(CP) --reflink=auto  "$<" "$@"


MYCMPCP=$(CMP) -s '$1' '$2' || $(CP) '$1' '$2'
.PHONY: prepare-version
mktmp:
	@$(MKDIR) tmp/
prepare-version: mktmp
	$(file >tmp/BUILD,$(BUILD))
	$(file >tmp/VERSION,$(VERSION))
BUILD: prepare-version
	@$(call MYCMPCP,tmp/BUILD,$@)
VERSION: prepare-version
	@$(call MYCMPCP,tmp/VERSION,$@)

# Set this to the comma-separated list of years that should appear in
# the licence.  Do not use characters other than [0-9,] - no spaces.
YEARS=2001,2003,2005-7,2012,2019

update-licence:
	$(FIND) . -name ".git" -prune -or -type f -print0 | $(XARGS) -0 \
	$(SED) -i 's/Copyright (C) [0-9,-]\+  Steven Simpson/Copyright (C) $(YEARS)  Steven Simpson/g'
