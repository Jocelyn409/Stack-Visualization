all: test test-inst stackviz

test: test.c test.o
	gcc test.c -o test

test-inst: inst.o
	gcc -finstrument-functions test.c inst.o -o test-inst

inst.o: inst.c
	gcc -c inst.c

stackviz: stackviz.c stackviz.o
	gcc stackviz.c -o stackviz

clean:
	rm test test-inst stackviz *.o
