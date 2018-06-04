CC      = gcc
CCFLAGS = -c $(LDFLAGS)
LD      = $(CC) $(LDFLAGS)
LDFLAGS = -Wall

ROOTDIR := .
OBJDIR  := obj
BINDIR  := bin
SRCDIR  := src
INCDIR  := inc

INC  := -I$(ROOTDIR) -I$(INCDIR)
SRC  := $(wildcard $(SRCDIR)/*.c)
OBJ  := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
PRE  := `pkg-config --cflags gtk+-3.0`
POST := `pkg-config --libs gtk+-3.0`
LIBS := -lepoxy -lm -lpng
EXEC := asteroids

all: $(EXEC)

$(EXEC): $(OBJ) | dir
	cd DashGL; make; cd ..;
	$(LD) $(PRE) -o $(BINDIR)/$@ $(OBJ) DashGL/dashgl.o $(POST) $(LIBS)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	if [ ! -d "DashGL" ]; then \
		git clone https://github.com/kion-dgl/DashGL-Library.git DashGL; \
	fi

clean:
	rm -f $(OBJDIR)/* 2>/dev/null || true
	rm -f $(BINDIR)/* 2>/dev/null || true
	rm DashGL/*.o     2>/dev/null || true

diff:
	git diff --stat

rebuild: clean $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | dir
	$(CC) $(PRE) $(CCFLAGS) $(INC) -c $< -o $@ $(POST) $(LIBS)
