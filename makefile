compile: func.o
	@gcc -o shell  func.o
func.o: func.c 
	@gcc -c func.c
clean: 
	rm *o 
	rm program 