all: mysort counter

mysort: Driver_mysort.o Bucket_Sort.o Locks.o Barriers.o array.o
	g++  Driver_mysort.o Bucket_Sort.o Locks.o Barriers.o array.o -pthread -O0 -g -o mysort

counter: Driver_counter.o Counter.o Locks.o Barriers.o
	g++  Driver_counter.o Counter.o Locks.o Barriers.o -pthread -O0 -g -o counter

Driver_mysort.o: Driver_mysort.cpp
	g++ -c -g Driver_mysort.cpp

Driver_counter.o: Driver_counter.cpp
	g++ -c -g Driver_counter.cpp

Counter.o: Counter.cpp
	g++ -c -g Counter.cpp

Bucket_Sort.o: Bucket_Sort.cpp
	g++ -c -g Bucket_Sort.cpp

Locks.o: Locks.cpp
	g++ -c -g Locks.cpp

Barriers.o: Barriers.cpp
	g++ -c -g Barriers.cpp

array.o: array.cpp
	g++ -c -g array.cpp

clean:
	rm *.o mysort