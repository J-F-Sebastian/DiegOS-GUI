.PHONY: all clean builddir build

OBJDIR := build

# Geometry classes, Points and Rectangles
OBJS := geometry.o

# The Z-Buffer
OBJS += viewzbuffer.o

# Events related objects
OBJS += event.o
OBJS += eventqueue.o
OBJS += vieweventfactory.o
OBJS += vieweventsdl.o

# Color Palettes
OBJS += palette.o
OBJS += palettegroup.o
OBJS += palettegroupfactory.o

# Rendering
OBJS += viewrenderfactory.o
OBJS += viewrenderhw.o

# View and ViewGroup
OBJS += view.o
OBJS += viewgroup.o

# Specialized ViewGroups
OBJS += viewexec.o
OBJS += viewapplication.o
OBJS += window.o
OBJS += desktopapp.o

# Specialized Views
OBJS += frame.o
OBJS += titlebar.o
OBJS += background.o
OBJS += progressbar.o
OBJS += scrollbar.o
OBJS += abstract_button.o
OBJS += button.o
OBJS += window_icon.o
OBJS += desktop.o
OBJS += resizetab.o
OBJS += palettetab.o

# Singletons
OBJS += palettegroupinstance.o
OBJS += viewrenderinstance.o
OBJS += systempaletteinstance.o

# Test application
OBJS += testdesktopapp.o

CXXFLAGS += -Wall -Wextra -Winline -Wcast-align -g3 -O1

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: builddir build

build: $(addprefix $(OBJDIR)/, $(OBJS)) *.h
	$(CXX) $(LFLAGS) -g3 -o guidbg $(OBJDIR)/* -lSDL2main -lSDL2 -lSDL2_ttf
	$(CXX) $(LFLAGS) -s -o gui $(OBJDIR)/* -lSDL2main -lSDL2 -lSDL2_ttf

clean:
	rm -rf $(OBJDIR)/
	rm -f gui
	rm -f guidbg

builddir:
	mkdir -p build
