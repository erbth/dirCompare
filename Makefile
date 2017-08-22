SHELL := /bin/bash
CXX := g++
LD := g++
VALGRIND := valgrind
TEE := tee
PDFLATEX := pdflatex

OBJDIR := bin
DOCDIR := doc

CXXFLAGS += -std=c++11 -gdwarf-2 -DWITH_TESTING
LDFLAGS  += $(CXXFLAGS)
TEXFLAGS += -synctex=1

include platform.make
TARGET_PLATFORM := $(LINUX)
CXXFLAGS += -DTARGET_PLATFORM=$(TARGET_PLATFORM)

TARGET = dirCompare
LIBOBJS= ItemFactory.o \
         Item.o \
		 File.o \
		 Directory.o \
		 InvalidItem.o \
		 InvalidFile.o \
		 InvalidDirectory.o \
		 LinuxFile.o \
		 LinuxDirectory.o \
		 LinuxItemFactory.o \
		 LinuxFileInfo.o \
		 LinuxFileComparisonStrategy.o \
		 LinuxDirectoryComparisonStrategy.o \
		 LinuxSimpleFileComparison.o \
		 LinuxComparisonFactory.o \
		 SimpleDirectoryComparison.o \
		 ComparisonContext.o \
		 ComparisonStrategyFactory.o \
		 FileComparisonStrategy.o \
		 DirectoryComparisonStrategy.o \
		 SystemParameters.o \
		 Token.o \
		 Value.o \
		 Keyword.o \
		 Commandline.o \
		 ignoring.o \
		 platform.o \
		 log.o

OBJS =   $(LIBOBJS) main.o

TESTS  = TEST_CPPUNIT #\
		 TEST_LEAKCHECK_MATCH \
		 TEST_LEAKCHECK_DIFFER \
		 TEST_MATCH \
		 TEST_DIFFER \
		 TEST_LEAKCHECK_IGNORE

DOCS   = documentation.pdf

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%)

.PHONY: test
test: $(TESTS)

.PHONY: doc
doc: $(DOCS:%=$(DOCDIR)/%)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/testing: $(OBJDIR)/testing.o $(LIBOBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) `pkg-config --libs cppunit` -o $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -MMD -o $@ $<

$(DOCDIR)/%.pdf: $(DOCDIR)/%.tex
	$(PDFLATEX) $(TEXFLAGS) -output-directory=$(DOCDIR) $<

$(OBJS:%=$(OBJDIR)/%): | $(OBJDIR)
$(TARGET:%=$(OBJDIR)/%): | $(OBJDIR)

.PHONY: TEST_LEAKCHECK_MATCH
TEST_LEAKCHECK_MATCH: $(TARGET:%=$(OBJDIR)/%) TEST_CPPUNIT
	$(VALGRIND) --leak-check=full --error-exitcode=1 $< \
		--dir1 testDir1 --dir2 testDir2 --fileStrategy simple --dirStrategy simple \
		>/dev/null

.PHONY: TEST_LEAKCHECK_DIFFER
TEST_LEAKCHECK_DIFFER: $(TARGET:%=$(OBJDIR)/%) TEST_CPPUNIT
	$(VALGRIND) --leak-check=full --error-exitcode=100 $< \
		--dir1 testDir1 --dir2 testDir3 --fileStrategy simple --dirStrategy simple \
		>/dev/null; \
		test $$? -ne 100

.PHONY: TEST_MATCH
TEST_MATCH: $(TARGET:%=$(OBJDIR)/%) TEST_CPPUNIT
	$< --dir1 testDir1 --dir2 testDir2 --fileStrategy simple --dirStrategy simple \
	> /dev/null; \
	test $$? -eq 0

.PHONY: TEST_DIFFER
TEST_DIFFER: $(TARGET:%=$(OBJDIR)/%) TEST_CPPUNIT
	$< --dir1 testDir1 --dir2 testDir3 --fileStrategy simple --dirStrategy simple \
	> /dev/null; \
	test $$? -ne 0

.PHONY: TEST_LEAKCHECK_IGNORE
TEST_LEAKCHECK_IGNORE: $(TARGET:%=$(OBJDIR)/%) TEST_CPPUNIT
	$(VALGRIND) --leak-check=full --error-exitcode=100 $< \
		--dir1 testDir1 --dir2 testDir3 --fileStrategy simple --dirStrategy simple \
		--ignoreFile "test1.5" --ignoreDir "testDir2" >/dev/null; \
		test $$? -ne 100

.PHONY: TEST_CPPUNIT
TEST_CPPUNIT: $(OBJDIR)/testing
	$(OBJDIR)/testing

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

# include compiler generated dependency files for headers
-include $(OBJDIR)/*.d
