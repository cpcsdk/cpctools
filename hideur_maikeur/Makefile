# Customizable datas (language, plattform related datas...)

CC=gcc
FLAGS= -Wall -ansi -pedantic
TMPFILES = *.o *~

OS=-DDOS # ascii fix if dos is the plattforms

LG= -DENG # Choose your Language
#LG=-DFRENCH

EXE=hideur # Choose the executable name
#EXE=hideur.exe

CLEANCMD=rm -f# the clean command (MS-DOS)
#CLEANCMD=del

# Datas which should'nt be edited

#DEBUG=-DDBG

hideur : header.o main.o
	$(CC) -o $(EXE) header.o main.o $(FLAGS) $(LG) $(OS) $(DEBUG)
	@echo Ok...
	@./hideur

header.o : header.c header.h messages.h debug.h aff.h
	$(CC) -c header.c  $(LG) $(DEBUG) $(OS)

main.o : main.c header.h header.c messages.h debug.h
	$(CC) -c main.c  $(LG) $(DEBUG) $(OS)

clean :
	@$(CLEANCMD) $(TMPFILES)
	@echo Ok...

superclean :
	@$(CLEANCMD) $(TMPFILES) $(EXE)

idiot :
	@echo Agaaaa...gagaga.


