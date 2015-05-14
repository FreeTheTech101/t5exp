#include "stdinc.h"

Com_Printf_t Com_Printf = (Com_Printf_t)0x4126C0;
DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x493A60;
DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x4359A0;
FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x4BEDA0;
FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x4B54D0;
SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x624C70;

unsigned int R_HashString(const char* string)
{
	unsigned int hash = 0;

	while (*string)
	{
		hash = (*string | 0x20) ^ (33 * hash);
		string++;
	}

	return hash;
}

#define VA_BUFFER_COUNT		4
#define VA_BUFFER_SIZE		32768

static char vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
static int vaNextBufferIndex = 0;
static CRITICAL_SECTION ThreadSafe;
static bool Initialized = false;

const char *va(const char *fmt, ...)
{
	va_list AP;
	size_t Length = 0;
	char *Destination = nullptr;

	if (!Initialized)
	{
		InitializeCriticalSection(&ThreadSafe);
		Initialized = true;
	}

	EnterCriticalSection(&ThreadSafe);
	Destination = &vaBuffer[vaNextBufferIndex][0];
	vaNextBufferIndex = (vaNextBufferIndex + 1) % VA_BUFFER_COUNT;

	va_start(AP, fmt);
	Length = _vsnprintf_s(Destination, VA_BUFFER_SIZE, _TRUNCATE, fmt, AP);
	Destination[VA_BUFFER_SIZE - 1] = '\0';
	va_end(AP);

	if (Length < 0 || Length >= VA_BUFFER_SIZE)
	{
		// This is pretty bad.
		MessageBoxA(NULL, "Attempted to overrun string in call to va()", "ZoneTool", NULL);
	}

	LeaveCriticalSection(&ThreadSafe);
	return Destination;
}