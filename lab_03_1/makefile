CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic

OBJS := error.o matrix.o matrixio.o

ifeq ($(mode), debug)
    CFLAGS += -g3 -ggdb
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

app.exe: $(OBJS) fileio.o main.o
	$(CC) $^ -o $@
	
test.exe: $(OBJS) test.o
	$(CC) $^ -o $@
	
%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<
	
clean:
	$(RM) *.o *.exe