CC := gcc
CFLAGS := -std=c99 -Wall -Werror
OBJS := btree.o hasht.o file.o
LIB := btree.dll hasht.dll file.dll

ifeq ($(mode), debug)
    CFLAGS += -g3 -ggdb
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

app.exe: main.o
	$(CC) $^ -o $@
	
test.exe: test.o
	$(CC) $^ -o $@

%.dll: %.o
	mkdir dll
	gcc -shared $^ -Wl,--subsystem,windows -o dll/$<
	
main.o: main.c *.h
	$(CC) $(CFLAGS) -c $<
	
test.o: main.c *.h
	$(CC) $(CFLAGS) -c $<
	
%.o: %.c *.h
	$(CC) $(CFLAGS) -D ARR_EXPORTS -c $<
	
clean:
	$(RM) *.o *.exe *.a *.dll