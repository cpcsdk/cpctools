SUBDIRS = src


IMGS = logo.png pause.png

EXTRA_DIST = rom $(IMGS) cap32.cfg.tpl Keymap.cfg

reloadeddir = $(pkgdatadir)
reloaded_DATA = $(IMGS) cap32.cfg Keymap.cfg

reloadedromdir = $(pkgdatadir)/rom
reloadedrom_DATA = rom/*


cap32.cfg: cap32.cfg.tpl
	@regex_cmd@ $< -e 's;@DATA_DIR@;$(pkgdatadir);' > cap32.cfg	
