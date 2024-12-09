compile: func.o cwd.o redirection.o
	@gcc -o shell func.o cwd.o redirection.o
func.o: func.c cwd.h redirection.h
	@gcc -c func.c
cwd.o: cwd.c cwd.h
	@gcc -c cwd.c
redirection.o: redirection.c redirection.h
	@gcc -c redirection.c
clean:
	rm -f *o
	rm -f program