CXX := g++
LD := g++

OBJDIR := bin

CXXFLAGS += -std=c++11 -gdwarf-2
LDFLAGS  += $(CXXFLAGS)

TARGET = dirCompare
OBJS   = main.o \
         Item.o \
		 File.o \
		 Directory.o \
		 LinuxFile.o \
		 LinuxDirectory.o \
		 LinuxItemFactory.o \
		 LinuxFileInfo.o

HEADERS = Item.h \
		  File.h \
		  Directory.h \
		  LinuxFile.h \
		  LinuxDirectory.h \
		  ItemFactory.h \
		  LinuxItemFactory.h \
		  errno_exception.h \
		  gp_exception.h \
		  LinuxFileInfo.h

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJS:%=$(OBJDIR)/%): | $(OBJDIR)
$(TARGET:%=$(OBJDIR)/%): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
