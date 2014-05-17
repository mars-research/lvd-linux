# Make sure early boot has no stackprotector
nostackp := $(call cc-option, -fno-stack-protector)
CFLAGS_enlighten.o		:= $(nostackp)

obj-y		:= enlighten.o

