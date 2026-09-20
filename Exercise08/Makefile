CC = mpicc
CFLAGS = -O2

all:
	$(CC) $(CFLAGS) -o ../Exercise01/ex01 ../Exercise01/exercise01.c
	$(CC) $(CFLAGS) -o ../Exercise02/ex02 ../Exercise02/exercise02.c
	$(CC) $(CFLAGS) -o ../Exercise03/ex03 ../Exercise03/exercise03.c
	$(CC) $(CFLAGS) -o ../Exercise04/ex04 ../Exercise04/exercise04.c
	$(CC) $(CFLAGS) -o ../Exercise05/ex05 ../Exercise05/exercise05.c
	$(CC) $(CFLAGS) -o ../Exercise06/ex06 ../Exercise06/exercise06.c

run: all
	mpirun --oversubscribe -np 4 ../Exercise01/ex01
	mpirun --oversubscribe -np 4 ../Exercise02/ex02
	mpirun --oversubscribe -np 4 ../Exercise03/ex03
	mpirun --oversubscribe -np 4 ../Exercise04/ex04
	mpirun --oversubscribe -np 4 ../Exercise05/ex05
	mpirun --oversubscribe -np 4 ../Exercise06/ex06

clean:
	rm -f ../Exercise01/ex01
	rm -f ../Exercise02/ex02
	rm -f ../Exercise03/ex03
	rm -f ../Exercise04/ex04
	rm -f ../Exercise05/ex05
	rm -f ../Exercise06/ex06
