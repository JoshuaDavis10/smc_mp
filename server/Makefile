CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=../bin/server
LFLAGS:=

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

ifeq ($(OS),Windows_NT)
    # windows
    LFLAGS:=-lWs2_32
endif

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
