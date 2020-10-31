mysort: Driver.o Merge_Sort.o Quick_Sort.o
	g++  Driver.o Merge_Sort.o Quick_Sort.o -o mysort

Driver.o: Driver.cpp
	g++ -c Driver.cpp

Merge_Sort.o: Merge_Sort.cpp
	g++ -c Merge_Sort.cpp

Quick_Sort.o: Quick_Sort.cpp
	g++ -c Quick_Sort.cpp

clean:
	rm *.o mysort