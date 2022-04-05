#include <iostream>
#include "MKKAllocator.h"

using namespace std;

MKKAllocator::MKKAllocator()
{
    // request initial memory from OS
    mem_size = 1024;
    mem_ptr = ::malloc(mem_size);
}

MKKAllocator::~MKKAllocator()
{
    // return memory to OS
    if (mem_ptr != nullptr)
        ::free(mem_ptr);
}

void* MKKAllocator::malloc(const size_t size)
{
    if (size == 0)
        return nullptr;
    return nullptr;
}

void MKKAllocator::free(const void* ptr)
{
    if (ptr == nullptr)
        return;
}

void MKKAllocator::print()
{
}
