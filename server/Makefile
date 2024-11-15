CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=../bin/server
TEST:=../bin/gc_test

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

all: $(BIN) $(TEST)

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir obj

$(TEST): src/test/gc_test.c src/logger.c src/game_core.c src/mech.c
	@echo compiling unit test...
	@$(CC) $^ -o $@

.PHONY: clean
clean:
	@rm -rf obj
