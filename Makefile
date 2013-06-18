# FreeBSD makefile for topic parser
# Topcat Software LLC.

CC?=     cc
CFLAGS+= -Wall -Werror
BUILD=   v1.02

all: prep compile

prep: ./bin2c.c ./syntax.txt ./markup.txt ./license.txt
	$(CC) $(CFLAGS) ./bin2c.c -o ./bin2c
	./bin2c -z ./syntax.txt ./syntax.c
	./bin2c -z ./markup.txt ./markup.c
	./bin2c -z ./license.txt ./license.c

compile: ./topic.c ./syntax.c ./markup.c ./license.c ./topic.1
	$(CC) $(CFLAGS) -DBUILD="\"$(BUILD)\"" ./topic.c -o ./topic

clean:
	rm -f ./topic ./topic.1.gz ./bin2c ./syntax.c ./markup.c ./license.c

# eof
