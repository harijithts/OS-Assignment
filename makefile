run:
	gcc ./priority.c -Wextra -Wall -fsanitize=undefined -o main
	./main 
