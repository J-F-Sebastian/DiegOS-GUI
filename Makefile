.PHONY: all clean

OBJDIR := build

OBJS := event.o eventqueue.o view.o viewgroup.o viewexec.o frame.o geometry.o palette.o palettegroup.o
OBJS += palettegroupfactory.o viewrenderfactory.o viewrenderhw.o vieweventfactory.o vieweventsdl.o
OBJS += titlebar.o window.o background.o progressbar.o button.o window_icon.o desktopapp.o scrollbar.o
OBJS += testdesktopapp.o viewgroup.o viewapplication.o palettegroupinstance.o viewrenderinstance.o
OBJS += viewzbuffer.o desktop.o abstract_button.o resizetab.o systempaletteinstance.o palettetab.o

CXXFLAGS += -Wall -Wextra -Winline -Wcast-align -g3 -O1

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(addprefix $(OBJDIR)/, $(OBJS)) *.h
	$(CXX) $(LFLAGS) -g3 -o guidbg $^ -lSDL2main -lSDL2 -lSDL2_ttf
	$(CXX) $(LFLAGS) -s -o gui $^ -lSDL2main -lSDL2 -lSDL2_ttf

clean:
	rm -rf $(OBJDIR)/
	rm -f gui
	rm -f guidbg
