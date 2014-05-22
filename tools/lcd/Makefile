# Userspace tools

CFLAGS=-O2

TOOLS = insmod4lcd

all: $(TOOLS)

insmod4lcd: insmod4lcd.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f $(TOOLS)
