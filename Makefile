TARGET=rsakey-cracker
SOURCE=$(TARGET).c

ifeq ($(shell uname),Darwin)
CFLAGS=-I/usr/local/opt/openssl/include
LIBS=-L/usr/local/opt/openssl/lib
endif
CFLAGS+= -Wall -Wextra
LIBS+= -lssl -lcrypto -lpthread

all:
	$(CC) $(CFLAGS) $(SOURCE) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
