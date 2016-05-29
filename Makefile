
#######################
##                   ##
#  Makefile my_crypt  #
##                   ##
#######################

DEBUG = yes
OS_TYPE = $(shell uname -s)
TARGET = my_crypt
CC = clang
CFLAGS = -Wall -O2 -I /usr/local/include

ifeq ($(OS_TYPE), Linux)
	GCRYPT_FLAGS = 
	GCRYPT_LIBS = -lgcrypt
else ifeq ($(OS_TYPE), Darwin)
	GCRYPT_FLAGS = -I/usr/local/Cellar/libgcrypt/1.7.0_1/include -I/usr/local/Cellar/libgpg-error/1.22/include
	GCRYPT_LIBS = -L/usr/local/Cellar/libgcrypt/1.7.0_1/lib -lgcrypt -L/usr/local/Cellar/libgpg-error/1.22/lib -lgpg-error
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
