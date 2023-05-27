.PHONY: all clean

OBJDIR := build

OBJS := event.o eventqueue.o view.o viewgroup.o viewexec.o frame.o geometry.o palette.o palettegroup.o
OBJS += palettegroupfactory.o viewrenderfactory.o viewrenderhw.o vieweventfactory.o vieweventsdl.o
OBJS += titlebar.o window.o background.o progressbar.o button.o window_icon.o desktopapp.o scrollbar.o
OBJS += testdesktopapp.o viewapplication.o palettegroupinstance.o viewrenderinstance.o
OBJS += viewzbuffer.o desktop.o
CXXFLAGS += -Wall -Wextra -Winline -Wcast-align -g -O4
CXXFLAGS += -I"E:\sviluppo\SDL2_ttf-2.20.1\i686-w64-mingw32\include\SDL2"
CXXFLAGS += -I"E:\sviluppo\SDL2-2.24.0\i686-w64-mingw32\include\SDL2"
LFLAGS = -L"E:\sviluppo\SDL2-2.24.0\i686-w64-mingw32\lib"
LFLAGS += -L"E:\sviluppo\SDL2_ttf-2.20.1\i686-w64-mingw32\lib"

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(addprefix $(OBJDIR)/, $(OBJS)) *.h
	$(CXX) $(LFLAGS) -g -o testsys_debug.exe $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	$(CXX) $(LFLAGS) -s -o testsys.exe $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

clean:
	del /Q $(OBJDIR)\*.*
	del testsys*.exe
