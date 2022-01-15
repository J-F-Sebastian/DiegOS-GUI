.PHONY: all clean

OBJDIR := build

OBJS := event.o eventqueue.o view.o viewexec.o frame.o geometry.o palette.o palettegroup.o
OBJS += palettegroupfactory.o viewrenderfactory.o viewrenderhw.o vieweventfactory.o vieweventsdl.o
OBJS += titlebar.o window.o background.o progressbar.o button.o window_icon.o desktopapp.o
OBJS += testdesktopapp.o
CXXFLAGS += -Wall -Wextra -Winline -Wcast-align -I.\SDL\include\SDL2 -g -O1 -march=core2

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(addprefix $(OBJDIR)/, $(OBJS))
	$(CXX) -L.\SDL\lib -o testsys_debug.exe $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	$(CXX) -L.\SDL\lib -s -o testsys.exe $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
#	$(CXX) -g -o testsys_debug.exe testlist.cpp

clean:
	del /Q $(OBJDIR)\*.*
	del testsys*.exe
