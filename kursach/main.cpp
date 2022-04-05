#include <iostream>
#include "RMAllocator.h"
#include "MKKAllocator.h"


using namespace std;

// списки свободных блоков (первое подходящее) и алгоритм Мак-Кьюзи-Кэрелса
int main()
{
	cout << "OS Kursach (c) Diana Kolpakova" << endl;
	cout << "Memory allocators:" << endl;
	cout << "- Resource Map allocator (first fit policy)" << endl;
	cout << "- McKusick-Karels allocator" << endl;
	cout << endl;

	RMAllocator a1;
	//MKKAllocator mkkallocator;

	a1.print();

	void* p1 = a1.malloc(100);
	cout << "p1 = " << p1 << endl << endl;
	a1.print();

	void* p2 = a1.malloc(100);
	cout << "p2 = " << p2 << endl << endl;
	a1.print();

	void* p3 = a1.malloc(100);
	cout << "p3 = " << p3 << endl << endl;
	a1.print();

	a1.free(p2, 100);
	cout << "p2 freed" << endl << endl;
	a1.print();

	a1.free(p1, 100);
	cout << "p1 freed" << endl << endl;
	a1.print();

	a1.free(p3, 100);
	cout << "p3 freed" << endl << endl;
	a1.print();

	a1.defragment();
	cout << "defragmented" << endl << endl;
	a1.print();
}
