CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic

OBJS := numbio.o calc.o

ifeq ($(mode), debug)
    CFLAGS += -g3
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

app.exe: $(OBJS) main.o
	$(CC) $^ -o $@
	
test.exe: $(OBJS) test.o
	$(CC) $^ -o $@
	
%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<
	
clean:
	$(RM) *.o *.exe