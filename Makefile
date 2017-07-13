CXX := g++
LD := g++

OBJDIR := bin

CXXFLAGS += -std=c++11
LDFLAGS  += $(CXXFLAGS)

TARGET = dirCompare
OBJS   = main.o \
         Item.o

.PHONY: all
all: $(TARGET:%=$(OBJDIR)/%)

$(TARGET:%=$(OBJDIR)/%): $(OBJS:%=$(OBJDIR)/%)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJS:%=$(OBJDIR)/%): | $(OBJDIR)
$(TARGET:%=$(OBJDIR)/%): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
