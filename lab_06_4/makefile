CC := gcc
CFLAGS := -std=c99 -Wall -Werror
SRCDIR := src
LIBDIR := lib

LIBS := btree.dll file.dll
BTREE_OBJ := $(SRCDIR)/queue.o $(SRCDIR)/btree.o $(SRCDIR)/avltree.o
FILE_OBJ := $(SRCDIR)/file.o

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
	
libs: $(LIBS)

btree.dll: $(BTREE_OBJ) $(LIBDIR)
	gcc -shared $(BTREE_OBJ) -Wl,--subsystem,windows -o $(LIBDIR)/$@
	
file.dll: $(FILE_OBJ) $(LIBDIR)
	gcc -shared $(FILE_OBJ) -Wl,--subsystem,windows -o $(LIBDIR)/$@
	
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
	
clean_app:
	$(RM) $(SRCDIR)/main.o $(SRCDIR)/test.o *.exe 