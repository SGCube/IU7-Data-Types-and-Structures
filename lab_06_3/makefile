CC := gcc
CFLAGS := -std=c99 -Wall -Werror
SRCDIR := src
LIBDIR := lib

BTREE_OBJ := $(SRCDIR)/btree.o $(SRCDIR)/avltree.o

ifeq ($(mode), debug)
    CFLAGS += -g3 -ggdb
endif

ifeq ($(mode), release)
    CFLAGS += -DNDEBUG -g0
endif

vpath %.c $(SRCDIR)
	
app.exe: $(SRCDIR)/main.o
	$(CC) $^ -o $@
	
test.exe: $(SRCDIR)/test.o
	$(CC) $^ -o $@

btree.dll: $(BTREE_OBJ) $(LIBDIR)
	gcc -shared $(BTREE_OBJ) -Wl,--subsystem,windows -o $(LIBDIR)/$@
	
%.dll: $(SRCDIR)/%.o $(LIBDIR)
	gcc -shared $< -Wl,--subsystem,windows -o $(LIBDIR)/$@
	
$(SRCDIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(SRCDIR)/test.o: test.c
	$(CC) $(CFLAGS) -c $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) -D ARR_EXPORTS -c $< -o $@
	
$(LIBDIR):
	mkdir $(LIBDIR)
	
clean:
	$(RM) *.o *.exe $(LIBDIR)/*.dll $(SRCDIR)/*.o