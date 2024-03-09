TARGET = Main
STRUCT =
LIB = 

CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/
PREF_LIB = ./lib/


ifeq ($(STRUCT), vec)
ifeq ($(LIB), stat)
	TARGET = Vecs
	LIBR = libvecs.a
	LFLAGS = -lvecs -lreadline
else
	TARGET = Vecd
	LIBR = libvecd.so
	LFLAGS = -lvecd -lreadline -Wl,-rpath,.
endif
else ifeq ($(STRUCT), list)
ifeq ($(LIB), stat)
	TARGET = Lists
	LIBR = liblsts.a
	LFLAGS = -llsts -lreadline
else
	TARGET = Listd
	LIBR = liblstd.so
	LFLAGS = -llstd -lreadline -Wl,-rpath,.
endif
endif

SRC = $(PREF_SRC)main.c
SRCV = $(PREF_SRC)f1.c $(PREF_SRC)inputfuncs.c
SRCL = $(PREF_SRC)f2.c $(PREF_SRC)inputfuncs.c

OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))
OBJ1 = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRCV))
OBJ2 = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRCL))

$(TARGET) : $(LIBR) $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)  -L. $(LFLAGS)
	rm $(PREF_OBJ)*.o
ifeq ($(LIB), stat)
	rm $(LIBR)
endif

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
ifeq ($(LIB), stat)
	$(CC) -c $< -o $@ -g -Wall -Wextra -Werror
else
	$(CC) -c $< -o $@ -fPIC -g -Wall -Wextra -Werror
endif

libvecs.a : $(OBJ1)
	ar r libvecs.a $(OBJ1)
	rm $(PREF_OBJ)f1.o

liblsts.a : $(OBJ2)
	ar r liblsts.a $(OBJ2)
	rm $(PREF_OBJ)f2.o

libvecd.so : $(OBJ1)
	$(CC) -shared -o libvecd.so $(OBJ1)
	rm $(PREF_OBJ)f1.o

liblstd.so : $(OBJ2)
	$(CC) -shared -o liblstd.so $(OBJ2)
	rm $(PREF_OBJ)f2.o

info:
	clear && echo "Enter STRUCT (vec | list) and LIB (stat | din) to compile\n"
