OBJDIRS = obj
DBG_OBJDIRS = dbg_objs
SRCDIRS = src
INCDIRS = include
EXEC = colorwars
DBG_EXEC = dbg_colorwars


DIRS = $(OBJDIRS) $(SRCDIRS) $(INCDIRS) $(DBG_OBJDIRS)
OBJS = $(patsubst $(SRCDIRS)/%.c, $(OBJDIRS)/%.o, $(wildcard $(SRCDIRS)/*.c))
DBG_OBJS = $(patsubst $(SRCDIRS)/%.c, $(DBG_OBJDIRS)/%.o, $(wildcard $(SRCDIRS)/*.c))


CC = gcc
CFLAGS = -Wall -Wextra -O0 -std=c99 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image
DEPFLAGS = -MT $@ -MMD -MP -MF $(INCDIRS)/$*.d


all: build


build: $(DIRS) $(EXEC)

mem_leak: debug
mem_leak:
	valgrind ./$(DBG_EXEC)

debug: CFLAGS += -g 
debug: $(DIRS) $(DBG_EXEC)


$(DIRS):
	mkdir -p $(DIRS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(EXEC)

$(OBJDIRS)/%.o: $(SRCDIRS)/%.c 
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(DBG_EXEC): $(DBG_OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(DBG_EXEC)	

$(DBG_OBJDIRS)/%.o: $(SRCDIRS)/%.c 
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@


-include $(wildcard $(INCDIRS)/*d)

clean:
	@rm -f $(wildcard $(INCDIRS)/*.d) $(wildcard $(OBJDIRS)/*.o) $(wildcard $(DBG_OBJDIRS)/*.o) $(EXEC)
	

.PHONY: build debug clean mem_leak
