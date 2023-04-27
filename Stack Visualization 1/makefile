stack_test: stack_test.o stack.o
	gcc -o stack_test stack_test.o stack.o

stack_test.o: stack_test.c f1.h f2.h
	gcc -c stack_test.c

stack.o:  stack.c stack.h
	gcc -c stack.c
	
clean:
	rm stack_test stack_test.o stack.o
