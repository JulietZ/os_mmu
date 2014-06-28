CC=g++
all: mmu

mmu: mmu.o Helper.o PageOperations.o
	$(CC) mmu.o Helper.o PageOperations.o -o mmu -std=c++0x

PageOperations.o: PageOperations.cpp
	$(CC) -c PageOperations.cpp -std=c++0x

Helper.o: Helper.cpp
	$(CC) -c Helper.cpp -std=c++0x

mmu.o: mmu.cpp
	$(CC) -c mmu.cpp -std=c++0x

clean:
	rm -rf *o mmu
