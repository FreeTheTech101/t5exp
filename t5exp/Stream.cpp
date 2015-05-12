#include "stdinc.h"

// Reserve 100MB by default.
// That's totally fine, as the dedi doesn't load images and therefore doesn't need much memory.
// That way we can be sure it won't need to reallocate memory.
// Side note: if you need a fastfile larger than 100MB, you're doing it wrong
Stream::Stream() : Stream(0x6400000) {}

Stream::Stream(size_t size)
{
	Stream::Buffer.reserve(size);
}

Stream::~Stream()
{
	Stream::Buffer.clear();
};

size_t Stream::Length()
{
	return Stream::Buffer.length();
}

size_t Stream::Size()
{
	return Stream::Buffer.size();
}

size_t Stream::Write(const void * _str, size_t size, size_t count)
{
	Stream::Buffer.append((char*)_str, size * count);
	return count;
}

size_t Stream::Write(int value, size_t count)
{
	size_t ret = 0;

	for (size_t i = 0; i < count; i++)
	{
		ret += Stream::Write(&value, 4, 1);
	}

	return ret;
}

size_t Stream::WriteString(const char* string)
{
	return Stream::WriteString(string, strlen(string));
}

size_t Stream::WriteString(const char* string, size_t len)
{
	size_t ret = 0;

	if (string)
	{
		ret += Stream::Write(string, len);
	}

	ret += Stream::WriteNull();

	return ret;
}

size_t Stream::WriteNull(size_t count)
{
	int _null = 0;

	size_t ret = 0;

	for (size_t i = 0; i < count; i++)
	{
		ret += Stream::Write(&_null, 1);
	}

	return ret;
}

size_t Stream::WriteMax(size_t count)
{
	int _max = -1;

	size_t ret = 0;

	for (size_t i = 0; i < count; i++)
	{
		ret += Stream::Write(&_max, 1);
	}

	return ret;
}

char* Stream::At()
{
	return (char*)(Stream::Data() + Stream::Length());
}

char* Stream::Data()
{
	return (char*)Stream::Buffer.data();
}

void Stream::ToBuffer(std::basic_string<uint8_t>& outBuffer)
{
	outBuffer.clear();
	outBuffer.append((const uint8_t*)Stream::Data(), Stream::Length());
}