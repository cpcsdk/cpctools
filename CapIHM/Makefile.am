SUBDIRS = src 


EXTRA_DIST = rom logo.png cap32.cfg

capricedir = $(pkgdatadir)
caprice_DATA = logo.png cap32.cfg

capriceromdir = $(pkgdatadir)/rom
capricerom_DATA = rom/*


cap32.cfg: cap32.cfg.tpl
	@regex_cmd@ $< -e 's;@DATA_DIR@;$(pkgdatadir);' > cap32.cfg	
