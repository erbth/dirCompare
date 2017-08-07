SHELL := /bin/bash
CXX := g++
LD := g++
VALGRIND := valgrind
TEE := tee
PDFLATEX := pdflatex

OBJDIR := bin
DOCDIR := doc

CXXFLAGS += -std=c++11 -gdwarf-2
LDFLAGS  += $(CXXFLAGS)
TEXFLAGS += -synctex=1

include platform.make
TARGET_PLATFORM := $(LINUX)
CXXFLAGS += -DTARGET_PLATFORM=$(TARGET_PLATFORM)

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
		 DirectoryComparisonStrategy.o \
		 SystemParameters.o \
		 Token.o \
		 Value.o \
		 Keyword.o \
		 Commandline.o \
		 platform.o

TESTS  = LEAKCHECK

DOCS   = documentation.pdf

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%)

.PHONY: test
test: $(TESTS)

.PHONY: doc
doc: $(DOCS:%=$(DOCDIR)/%)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -MMD -o $@ $<

$(DOCDIR)/%.pdf: $(DOCDIR)/%.tex
	$(PDFLATEX) $(TEXFLAGS) -output-directory=$(DOCDIR) $<

$(OBJS:%=$(OBJDIR)/%): | $(OBJDIR)
$(TARGET:%=$(OBJDIR)/%): | $(OBJDIR)

.PHONY: LEAKCHECK
LEAKCHECK: $(TARGET:%=$(OBJDIR)/%)
	$(VALGRIND) --leak-check=full --error-exitcode=1 $< >/dev/null

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

# include compiler generated dependency files for headers
-include $(OBJDIR)/*.d
