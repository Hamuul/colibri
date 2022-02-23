# Declaratiile de variabile
CC = gcc
CFLAGS = -g -std=gnu11 -Wall -lm

# Regula de compilare
build: task1 task2 task3 task4 task5

task1: task1.c
	$(CC) -o task1 task1.c $(CFLAGS)

task2: task2.c
	$(CC) -o task2 task2.c $(CFLAGS)

task3: task3.c
	$(CC) -o task3 task3.c $(CFLAGS)

task4: task4.c
	$(CC) -o task4 task4.c $(CFLAGS)

task5: browser.c
	$(CC) -o task5 browser.c -lncurses $(CFLAGS)

# Regulile de "curatenie" (sterge executabile)
.PHONY : clean
clean :
	rm -f task1 task2 task3 task4 task5