#include <string>
#include <stdint.h>

class Stream
{
private:
	std::string Buffer;

public:

	Stream();
	Stream(size_t size);
	~Stream();

	size_t Length();
	size_t Size();
	size_t Write(const void * _str, size_t size, size_t count = 1);
	size_t Write(int value, size_t count);

	size_t WriteString(const char* string);
	size_t WriteString(const char* string, size_t len);
	size_t WriteNull(size_t count = 1);
	size_t WriteMax(size_t count = 1);

	char* At();
	char* Data();

	void ToBuffer(std::basic_string<uint8_t>& outBuffer);
}; 
