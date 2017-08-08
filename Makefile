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
		 ItemFactory.o \
         Item.o \
		 File.o \
		 Directory.o \
		 LinuxFile.o \
		 LinuxDirectory.o \
		 LinuxItemFactory.o \
		 LinuxFileInfo.o \
		 LinuxFileComparisonStrategy.o \
		 LinuxDirectoryComparisonStrategy.o \
		 LinuxSimpleFileComparison.o \
		 LinuxSimpleDirectoryComparison.o \
		 LinuxComparisonFactory.o \
		 ComparisonContext.o \
		 ComparisonStrategyFactory.o \
		 FileComparisonStrategy.o \
		 DirectoryComparisonStrategy.o \
		 SystemParameters.o \
		 Token.o \
		 Value.o \
		 Keyword.o \
		 Commandline.o \
		 platform.o

TESTS  = TEST_LEAKCHECK_MATCH \
		 TEST_LEAKCHECK_DIFFER \
		 TEST_MATCH \
		 TEST_DIFFER

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

.PHONY: TEST_LEAKCHECK_MATCH
TEST_LEAKCHECK_MATCH: $(TARGET:%=$(OBJDIR)/%)
	$(VALGRIND) --leak-check=full --error-exitcode=1 $< \
		--dir1 testDir1 --dir2 testDir2 --fileStrategy simple --dirStrategy simple \
		>/dev/null

.PHONY: TEST_LEAKCHECK_DIFFER
TEST_LEAKCHECK_DIFFER: $(TARGET:%=$(OBJDIR)/%)
	$(VALGRIND) --leak-check=full --error-exitcode=100 $< \
		--dir1 testDir1 --dir2 testDir3 --fileStrategy simple --dirStrategy simple \
		>/dev/null; \
		test $$? -ne 100

.PHONY: TEST_MATCH
TEST_MATCH: $(TARGET:%=$(OBJDIR)/%)
	$< --dir1 testDir1 --dir2 testDir2 --fileStrategy simple --dirStrategy simple \
	> /dev/null; \
	test $$? -eq 0

.PHONY: TEST_DIFFER
TEST_DIFFER: $(TARGET:%=$(OBJDIR)/%)
	$< --dir1 testDir1 --dir2 testDir3 --fileStrategy simple --dirStrategy simple \
	> /dev/null; \
	test $$? -ne 0

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

# include compiler generated dependency files for headers
-include $(OBJDIR)/*.d
