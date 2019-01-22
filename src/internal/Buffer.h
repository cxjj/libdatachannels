#ifndef LIBDATACHANNELS_INTERNAL_BUFFER_H_
#define	LIBDATACHANNELS_INTERNAL_BUFFER_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>

class Buffer
{
public:
	Buffer(const uint8_t* data, const size_t size)
	{
		//Set buffer size to a 
		capacity = ((size << 6) + 1) >> 6;
		//Allocate memory
		buffer = (uint8_t*) std::aligned_alloc(capacity, 64);
		//Copy
		memcpy(buffer,data,size);
		//Reset size
		this->size = size;
	}
	
	Buffer(size_t capacity = 0)
	{
		//Set buffer size
		this->capacity = capacity ? ((size << 6) + 1) >> 6 : 0;
		//Allocate memory
		buffer = capacity ? std::aligned_alloc(capacity, 64) : nullptr;
		//NO size
		this->size = 0;
	}
	
	Buffer(Buffer &&other)
	{
		this->capacity = other.capacity;
		this->buffer = other.buffer;
		this->size = other.size;
		other.buffer = nullptr;
		other.capacity = 0;
		other.size = 0;
	}
	
	Buffer& operator=(Buffer&& other) 
	{
		this->capacity = other.capacity;
		this->buffer = other.buffer;
		this->size = other.size;
		other.buffer = nullptr;
		other.capacity = 0;
		other.size = 0;
		return *this; 
	}
	
	//Not copiable
	Buffer(const Buffer &) = delete;
	Buffer& operator=(const Buffer&) = delete;
	
	~Buffer()
	{
		std::free(buffer);
	}
	
	uint8_t* GetData() const		{ return buffer;		}
	size_t GetCapacity() const		{ return capacity;		}
	size_t GetSize() const			{ return size;			}

	void SetSize(size_t size) 
	{ 
		//Check capacity
		if (size>capacity)
			//Allocate new size
			Alloc(size);
		//Set new size
		this->size = size;
	}

	void Alloc(size_t capacity)
	{
		//Calculate new size
		this->capacity = capacity;
		//Realloc
		buffer = ( uint8_t*) std::realloc(buffer,capacity);
		//Check new size
		if (size>capacity)
			//reduce size
			size = capacity;
	}

	void SetData(const uint8_t* data,const size_t size)
	{
		//Check size
		if (size>capacity)
			//Allocate new size
			Alloc(size);
		//Copy
		std::memcpy(buffer,data,size);
		//Reset size
		this->size = size;
	}
	
	void SetData(const Buffer& buffer)
	{
		SetData(buffer.GetData(),buffer.GetSize());
	}

	void AppendData(const uint8_t* data,const size_t size)
	{
		//Check size
		if (this->size+size>capacity)
			//Allocate new size
			Alloc(this->size+size);
		//Copy
		std::memcpy(buffer+size,data,size);
		//Increase size
		this->size += size;
	}
	
	static Buffer&& Wrap(uint8_t* buffer, size_t size)
	{
		Buffer buffer;
		
		buffer.buffer = buffer;
		buffer.capacity = size;
		buffer.size = size;
		
		return std::move(buffer);
	}
	
protected:
	uint8_t* buffer		= nullptr;
	size_t capacity		= 0;
	size_t size		= 0;
};

#endif

