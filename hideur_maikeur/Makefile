# Customizable datas (language, plattform related datas...)

CC=gcc
FLAGS= -Wall -ansi -pedantic
TMPFILES = *.o *~
DIR=Hideur2.0
FILES=INSTALL README hideur
ARCHIVE=hideur-linux.eng

LG= -DENG # Choose your Language

EXE=hideur # choose executable name

CLEANCMD=rm -f # the clean command (Unix, Linux...)

# Datas which should'nt be edited

#DEBUG=-DDBG

hideur : header.o main.o
	$(CC) -o $(EXE) header.o main.o $(FLAGS) $(LG) $(DEBUG)
	@echo Ok...
	@hideur

header.o : header.c header.h messages.h debug.h aff.h
	$(CC) -c header.c  $(LG) $(DEBUG) $(OS)

main.o : main.c header.h header.c messages.h debug.h
	$(CC) -c main.c  $(LG) $(DEBUG) $(OS)

clean :
	@$(CLEANCMD) $(TMPFILES)
	@echo Ok...

superclean :
	@$(CLEANCMD) $(TMPFILES) $(EXE)

install :
	mkdir $(DIR)
	cp $(FILES) $(DIR)
	tar xvfz $(ARCHIVE) $(DIR)
	rm -fr DIR

idiot :
	@echo Agaaaa...gagaga.

tar :
	@echo "Creating archive" $(ARCHIVE).tar.gz ...
	@mkdir $(DIR)
	@cp $(FILES) $(DIR)
	@tar cvfz $(ARCHIVE).tar.gz  $(DIR) > /dev/null
	@rm -fr $(DIR)
