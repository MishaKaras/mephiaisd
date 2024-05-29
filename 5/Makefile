LIB = 
CC = gcc 

PREF_SRC = ./src/
PREF_OBJ = ./obj/

ifeq ($(LIB), stat)
	TARGET = S_gr
	LIBR = libgraph.a
	LFLAGS = -lgraph -lreadline -lcgraph -lgvc
else
	TARGET = D_gr
	LIBR = libgraph.so
	LFLAGS = -lgraph -lreadline -Wl,-rpath,. -lcgraph -lgvc
endif

SMAIN = $(PREF_SRC)main.c
SRC = $(filter-out $(PREF_SRC)main.c, $(wildcard $(PREF_SRC)*.c))
MAIN = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SMAIN))
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(LIBR) $(OBJ) $(MAIN)
	$(CC) -o $(TARGET) $(MAIN) -L. $(LFLAGS)
	rm $(PREF_OBJ)*.o
ifeq ($(LIB), stat)
	rm $(LIBR)
endif

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
ifeq ($(LIB), stat)
	$(CC) -c $< -o $@ -g -Wall -Wextra -Werror -lcgraph -lgvc
else
	$(CC) -c $< -o $@ -fPIC -g -Wall -Wextra -Werror -lcgraph -lgvc
endif

libgraph.a : $(OBJ)
	ar r libgraph.a $(OBJ)
	rm $(PREF_OBJ)graph.o $(PREF_OBJ)dialogs.o

libgraph.so : $(OBJ)
	$(CC) -shared -o libgraph.so $(OBJ)
	rm $(PREF_OBJ)graph.o $(PREF_OBJ)dialogs.o

info:
	clear && echo "Enter LIB (stat | din) to compile.\n"
