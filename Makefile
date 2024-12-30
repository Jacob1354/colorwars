OBJDIRS = obj
SRCDIRS = src
INCDIRS = include
EXEC = colorwars


DIRS = $(OBJDIRS) $(SRCDIRS) $(INCDIRS)
OBJS = $(patsubst $(SRCDIRS)/%.c, $(OBJDIRS)/%.o, $(wildcard $(SRCDIRS)/*.c))


CC = gcc
CFLAGS = -Wall -Wextra -O0 -std=c99 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`
DEPFLAGS = -MT $@ -MMD -MP -MF $(INCDIRS)/$*.d


all: build


build: $(DIRS) $(EXEC)

$(DIRS):
	mkdir $(DIRS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC)

$(OBJDIRS)/%.o: $(SRCDIRS)/%.c 
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(wildcard $(INCDIRS)/*d)

clean:
	@rm -f $(wildcard $(INCDIRS)/*.d) $(wildcard $(OBJDIRS)/*.o) $(EXEC)
	

.PHONY: build clean
