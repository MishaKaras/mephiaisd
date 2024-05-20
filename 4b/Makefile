LIB = 
CC = gcc 

PREF_SRC = ./src/
PREF_OBJ = ./obj/

ifeq ($(LIB), stat)
	TARGET = S_bst
	LIBR = libbst.a
	LFLAGS = -lbst -lreadline -lcgraph -lgvc -lm
else
	TARGET = D_bst
	LIBR = libbst.so
	LFLAGS = -lbst -lreadline -Wl,-rpath,. -lcgraph -lgvc -lm
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
	$(CC) -c $< -o $@ -g -Wall -Wextra -Werror -lcgraph -lgvc -lm
else
	$(CC) -c $< -o $@ -fPIC -g -Wall -Wextra -Werror -lcgraph -lgvc -lm
endif

libbst.a : $(OBJ)
	ar r libbst.a $(OBJ)
	rm $(PREF_OBJ)bintree.o $(PREF_OBJ)dialogs.o

libbst.so : $(OBJ)
	$(CC) -shared -o libbst.so $(OBJ)
	rm $(PREF_OBJ)bintree.o $(PREF_OBJ)dialogs.o

info:
	clear && echo "Enter LIB (stat | din) to compile.\n"

time:
	$(CC) timing.c $(PREF_SRC)bintree.c $(PREF_SRC)inputfuncs.c -o time -lcgraph -lgvc -g -lm
