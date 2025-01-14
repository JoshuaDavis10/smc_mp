CC:=gcc
SRCDIR:=src
OBJDIR:=obj

ifeq ($(OS),Windows_NT)
	# windows
	BIN:=../bin/client.exe
else
	UNAME_S := $(shell uname -s)
	ifeq($(UNAME_S),Linux)
	    # Linux
	    BIN:=../bin/client
endif

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
