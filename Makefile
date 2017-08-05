SHELL := /bin/bash
CXX := g++
LD := g++
VALGRIND := valgrind
TEE := tee

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
		 LinuxFileInfo.o \
		 LinuxSimpleFileComparison.o \
		 LinuxSimpleDirectoryComparison.o \
		 LinuxComparisonFactory.o \
		 ComparisonContext.o \
		 DirectoryComparisonStrategy.o

HEADERS = Item.h \
		  File.h \
		  Directory.h \
		  LinuxFile.h \
		  LinuxDirectory.h \
		  ItemFactory.h \
		  LinuxItemFactory.h \
		  errno_exception.h \
		  gp_exception.h \
		  LinuxFileInfo.h \
		  FileComparisonStrategy.h \
		  LinuxFileComparisonStrategy.h \
		  LinuxSimpleFileComparison.h \
		  DirectoryComparisonStrategy.h \
		  LinuxDirectoryComparisonStrategy.h \
		  LinuxSimpleDirectoryComparison.h \
		  ComparisonStrategyFactory.h \
		  LinuxComparisonFactory.h \
		  ComparisonContext.h

TESTS = #LEAKCHECK

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%) $(TESTS:%=$(OBJDIR)/%)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJS:%=$(OBJDIR)/%): | $(OBJDIR)
$(TARGET:%=$(OBJDIR)/%): | $(OBJDIR)

$(OBJDIR)/LEAKCHECK: $(TARGET:%=$(OBJDIR)/%)
	$(VALGRIND) --leak-check=full --error-exitcode=1 $< 2>&1 >/dev/null | $(TEE) $@; \
	test $${PIPESTATUS[0]} -eq 0

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
