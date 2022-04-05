#include <iostream>
#include <cinttypes>
#include "RMAllocator.h"

using namespace std;

RMAllocator::RMAllocator()
{
	// request initial memory from OS
	// request one memory page and then never ask more
	mem_page_ptr = (uint8_t*)::malloc(mem_page_size);

	if (mem_page_ptr == nullptr)
		throw runtime_error("Error: Cannot allocate a memory page");

	// initialize the first and only memory block
	first_block_ptr = (RMNode*)mem_page_ptr;
	first_block_ptr->next_block_ptr = nullptr;
	first_block_ptr->current_block_size = mem_page_size;
}

RMAllocator::~RMAllocator()
{
	// return memory to OS
	if (mem_page_ptr != nullptr)
		::free(mem_page_ptr);
}

void* RMAllocator::malloc(const size_t size)
{
	// special case
	if (size == 0)
		return nullptr;

	// requested memory size cannot be less than size of struct that keeps pointer + block size
	const size_t requested_size = max(size, sizeof(RMNode));

	// start with the first block;
	RMNode* block_ptr = first_block_ptr;

	// iterate through all the blocks until the first fit
	while (block_ptr != nullptr)
	{
		// the first fit:  if the current block is big enough, then cut the current block
		if (block_ptr->current_block_size >= requested_size + sizeof(RMNode))
		{
			// calculate pointer of memory block to allocate
			uint8_t* busy_block_ptr = (uint8_t*)block_ptr + block_ptr->current_block_size - requested_size;
			// reduce size of current free block;
			block_ptr->current_block_size -= requested_size;
			return busy_block_ptr;
		}

		// go to the next block
		block_ptr = block_ptr->next_block_ptr;
	}

	// no fit blocks found
	throw runtime_error("Error: Cannot allocate memory of requested size");
}

void RMAllocator::free(const void* ptr, const size_t size)
{
	// special case
	if (ptr == nullptr)
		return;

	// freed memory size cannot be less than size of struct that keeps pointer + block size
	const size_t freed_size = max(size, sizeof(RMNode));

	// initialize the freed memory block
	RMNode* freed_block_ptr = (RMNode*)ptr;
	freed_block_ptr->next_block_ptr = nullptr;
	freed_block_ptr->current_block_size = size;

	// insert the new memory block into the memory block chain (ordered by pointer values)
	RMNode** previous_ptr_ptr = &first_block_ptr;
	RMNode* block_ptr = first_block_ptr;
	// iterate through all the blocks
	while (block_ptr != nullptr)
	{
		// is it correct place to insert the free block
		if (ptr < block_ptr)
		{
			freed_block_ptr->next_block_ptr = block_ptr;
			break;
		}

		// go to the next block
		previous_ptr_ptr = &block_ptr->next_block_ptr;
		block_ptr = block_ptr->next_block_ptr;
	}
	*previous_ptr_ptr = freed_block_ptr;

    defragment();
}

void RMAllocator::print()
{
	cout << "RMAllocator statistics:" << endl;
	cout << "memory page pointer = " << (void*)mem_page_ptr << endl;
	cout << "memory page size = " << mem_page_size << endl;

	int num = 0;
	RMNode* block_ptr = first_block_ptr;
	while (block_ptr != nullptr)
	{
		cout << "free block #" << num << " pointer = " << (void*)block_ptr << endl;
		cout << "free block #" << num << " size = " << block_ptr->current_block_size << endl;
		num++;
		block_ptr = block_ptr->next_block_ptr;
	}
	cout << "free blocks total = " << num << endl;
	cout << endl;
}

void RMAllocator::defragment()
{
	//to do: join the memory blocks when possible
	RMNode* block_ptr = first_block_ptr;
	// iterate through all the blocks
	while (block_ptr != nullptr)
	{
		// repeat while we can join several subsequent blocks 
		while (block_ptr != nullptr)
		{
			// is next block is joint to the current block
			uint8_t* end_of_block_ptr = (uint8_t*)block_ptr + block_ptr->current_block_size;
			// break if not
			if (end_of_block_ptr != (uint8_t*)block_ptr->next_block_ptr)
				break;

			// increase the current block size
			block_ptr->current_block_size += block_ptr->next_block_ptr->current_block_size;
			// ajust the next block pointer
			block_ptr->next_block_ptr = block_ptr->next_block_ptr->next_block_ptr;
		}

		// go to the next block
		block_ptr = block_ptr->next_block_ptr;
	}
}
