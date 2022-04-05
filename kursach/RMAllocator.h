#pragma once
#include <cinttypes>

using namespace std;

class RMAllocator
{
private:
	struct RMNode
	{
		RMNode* next_block_ptr;
		size_t current_block_size;
	};

	const size_t mem_page_size = 1024;
	uint8_t* mem_page_ptr;
	RMNode* first_block_ptr;

public:
	RMAllocator();
	virtual ~RMAllocator();

	void* malloc(const size_t size);
	void free(const void* ptr, const size_t size);
	void print();
	void defragment();

};

