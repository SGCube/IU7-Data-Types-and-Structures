CC := gcc
CFLAGS := -std=c99 -Wall -Werror
SRCDIR := src

OBJS := $(SRCDIR)/matrix.o $(SRCDIR)/graph.o $(SRCDIR)/list.o $(SRCDIR)/graph_list.o

ifeq ($(mode), debug)
    CFLAGS += -g3 -ggdb
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

vpath %.c $(SRCDIR)
	
app.exe: $(OBJS) $(SRCDIR)/main.o
	$(CC) $^ -o $@
	
test.exe: $(OBJS) $(SRCDIR)/test.o
	$(CC) $^ -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	$(RM) *.o *.exe $(SRCDIR)/*.o *.exe *.gv *.png