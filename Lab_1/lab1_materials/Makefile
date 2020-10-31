mysort: Driver.o Merge_Sort.o Bucket_Sort.o
	g++  Driver.o Merge_Sort.o Bucket_Sort.o -pthread -O0 -g -o mysort

Driver.o: Driver.cpp
	g++ -c -g Driver.cpp

Merge_Sort.o: Merge_Sort.cpp
	g++ -c -g Merge_Sort.cpp

Bucket_Sort.o: Bucket_Sort.cpp
	g++ -c -g Bucket_Sort.cpp

clean:
	rm *.o mysort