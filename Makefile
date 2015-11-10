CC := clang++
SRCDIR := src
TESTDIR := tests
BUILDDIR := build

TARGET:=bin/speck
TEST_TARGET:=bin/speck_test

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS :=	$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TEST_SOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
TEST_OBJECTS :=	$(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TEST_SOURCES:.$(SRCEXT)=.o))
TEST_LD_LIBRARIES = -L/usr/local/lib -lCppUTest -lCppUTestExt
CFLAGS := -O3 -g -std=c++11# -Wall
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester: $(OBJECTS) $(TEST_OBJECTS)
	@echo " Linking..."
	$(CC) $(TEST_LD_LIBRARIES) build/speck.o build/tester.o -o $(TEST_TARGET) $(LIB)

test: tester
	@bin/speck_test

.PHONY: clean
