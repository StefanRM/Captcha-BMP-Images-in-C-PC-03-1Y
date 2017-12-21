build: build-task1 build-task2 build-task3 build-bonus

build-task1:
	gcc task1.c -o task1

build-task2:
	gcc task2.c -o task2

build-task3:
	gcc task3.c -o task3

build-bonus:
	gcc bonus.c -o bonus

run: run-task1 run-task2 run-task3 run-bonus

run-task1:
	./task1

run-task2:
	./task2

run-task3:
	./task3

run-bonus:
	./bonus

clean:
	rm -f task1 task2 task3 bonus *.bmp *.txt
