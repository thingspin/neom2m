
###############################################################################
# Path

#DESTDIR		:= $(CURDIR)/out
SRCROOT		= $(PWD)
PREFIX		:= /system
BINDIR		:= $(PREFIX)/sbin

CROSS_COMPILE := arm-linux-
###############################################################################
# Compile

CC	:= $(CROSS_COMPILE)gcc

#CFLAGS	:= $(EXTRA_CFLAGS)
#LDFLAGS	:= $(EXTRA_LDFLAGS)

###############################################################################
# Options

#AUTOSTART	:= y

###############################################################################
# Target rules

CFLAGS	+= -Wall
LDFLAGS	+= -lpthread  -lm

OBJS	:= src/netool.o src/netctrl.o 
APP	:= netctrl

all:		$(APP) 

$(APP):		$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(Q)rm -vrf $(APP) $(OBJS) \
		$(CONFIG_FILE)
	$(MAKE) clean -C mond_src

