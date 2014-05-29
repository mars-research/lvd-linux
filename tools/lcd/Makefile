# LCD tools

#CFLAGS += -O2
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra


TOOLS = insmod4lcd inskern4lcd

all: $(TOOLS)

insmod4lcd: insmod4lcd.c
	gcc $(CFLAGS) $^ -o $@

inskern4lcd: inskern4lcd.c
	gcc $(CFLAGS) $^ -o $@

clean:
	$(RM) -f $(TOOLS)
