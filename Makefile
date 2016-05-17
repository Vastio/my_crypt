
#######################
##                   ##
#  Makefile my_crypt  #
##                   ##
#######################

DEBUG = yes
TARGET = my_crypt
CC = clang
CFLAGS = -Wall -O2 -I /usr/local/include
LDFLAGS += -L/usr/local/lib
GCRYPT = `libgcrypt-config --cflags`

ifeq ($(DEBUG), yes)
	CFLAGS += -ggdb -DDEBUG
endif

# Sources list
SRCS = my_crypt.c


all:	.depend $(TARGET)


.depend:
	$(CC) $(CFLAGS) $(GCRYPT) -M $(SRCS) >> .depend


ifeq ($wildcard .depend,.depend)
include .depend
endif


$(TARGET):	$(SRCS:.c=.o)

clean:
		@rm -f *.o $(TARGET) .depend
		@echo "Sources cleaned"

cleanall:
		rm -fv *.o *~ $(TARGET) .depend
		@echo "All cleaned"
