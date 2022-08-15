CFLAGS= -Wall -pedantic-errors

compile:
	gcc $(CFLAGS) -o env src/env.c
	gcc $(CFLAGS) -o true src/true.c
	gcc $(CFLAGS) -o false src/false.c
	gcc $(CFLAGS) -o head src/head.c
	gcc $(CFLAGS) -o tail src/tail.c
	gcc $(CFLAGS) -o wc src/wc.c
	gcc $(CFLAGS) -o cat src/cat.c
	gcc $(CFLAGS) -o rm src/rm.c
	
clean:
	rm env
	rm true
	rm false
	rm head
	rm tail
	rm wc
	rm cat
	rm -f rm
