#pragma once
class MKKAllocator
{
private:
	size_t mem_size;
	void* mem_ptr;

public:
	MKKAllocator();
	virtual ~MKKAllocator();

	void* malloc(const size_t size);
	void free(const void* ptr);
	void print();
};

