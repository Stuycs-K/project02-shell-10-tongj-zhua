compile: func.o cwd.o
	@gcc -o shell func.o cwd.o
func.o: func.c cwd.c
	@gcc -c func.c
cwd.o: cwd.c
	@gcc -c cwd.c
clean: 
	rm *o 
	rm program 