CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=../bin/server

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
