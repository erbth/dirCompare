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
		 DirectoryComparisonStrategy.o \
		 SystemParameters.o \
		 Token.o \
		 Value.o \
		 Keyword.o

TESTS = LEAKCHECK

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%)

.PHONY: test
test: $(TESTS)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) -MMD -o $@ $<

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
