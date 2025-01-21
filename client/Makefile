CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=../bin/client
LFLAGS:=
CFLAGS:=-O1 -Wall -std=c99 -Wno-missing-braces

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

ifeq ($(OS),Windows_NT)
    # windows
    LFLAGS:=-lWs2_32 -lm -lraylib -lopengl32 -lgdi32 -lwinmm
endif

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@ -I include/ -L lib/ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	@echo compiling $<...
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
