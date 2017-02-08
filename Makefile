
#######################
##                   ##
#  Makefile my_crypt  #
##                   ##
#######################

DEBUG = yes
OS_TYPE = $(shell uname -s)
TARGET = my_crypt
CC = gcc
CFLAGS = -Wpedantic -Werror -Wshadow -Wstrict-overflow -fno-strict-aliasing -std=c99 -O2 -Os -I /usr/local/include


ifeq ($(OS_TYPE), Linux)
	GCRYPT_FLAGS =
	GCRYPT_LIBS = -lgcrypt
else ifeq ($(OS_TYPE), Darwin)
	GCRYPT_FLAGS = -I/usr/local/opt/libgcrypt/include -I/usr/local/opt/libgpg-error/include
	GCRYPT_LIBS = -L/usr/local/opt/libgcrypt/lib -lgcrypt -L/usr/local/opt/libgpg-error/lib -lgpg-error
endif

ifeq ($(DEBUG), yes)
	CFLAGS += -ggdb -DDEBUG
endif

# Sources list
SRCS = my_crypt.c


all:	$(TARGET)

$(TARGET):	my_crypt.o
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(GCRYPT_FLAGS) $(GCRYPT_LIBS)

clean:
		@rm -f *.o $(TARGET) .depend
		@echo "Sources cleaned"

cleanall:
		rm -fv *.o *~ $(TARGET) .depend
		@echo "All cleaned"
