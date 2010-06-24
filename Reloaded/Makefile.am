SUBDIRS = src


IMGS = logo.png pause.png

EXTRA_DIST = rom $(IMGS) cap32.cfg.tpl Keymap.cfg

reloadeddir = $(pkgdatadir)
reloaded_DATA = $(IMGS) cap32.cfg Keymap.cfg

reloadedromdir = $(pkgdatadir)/rom
reloadedrom_DATA = rom/amsdos.rom rom/cpc464.rom rom/cpc6128.rom rom/cpc664.rom


if DARWIN
cap32.cfg: cap32.cfg.tpl
	@regex_cmd@ $< -e 's;@DATA_DIR@;$(pkgdatadir);' > cap32.cfg	
else
cap32.cfg: cap32.cfg.tpl
	@regex_cmd@  's;@DATA_DIR@;$(pkgdatadir);' $< > cap32.cfg	
endif
