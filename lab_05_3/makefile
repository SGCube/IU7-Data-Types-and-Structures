CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic

OBJS := error.o qarray.o queue.o simul.o

ifeq ($(mode), debug)
    CFLAGS += -g3 -ggdb
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

app.exe: $(OBJS) test.o main.o
	$(CC) $^ -o $@
	
time.exe: $(OBJS) time.o
	$(CC) $^ -o $@
	
%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<
	
clean:
	$(RM) *.o *.exe *.txt