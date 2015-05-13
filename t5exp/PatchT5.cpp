#include "stdinc.h"

void XModelExport(const char* name);

void DumpStuff()
{
	XModelExport("t5_weapon_ak74u_viewmodel");
	XModelExport("t5_weapon_galil_viewmodel");
}

void RunStuff()
{
	static bool run = false;

	if (!run)
	{
		run = true;
		DumpStuff();
	}
}

void Sys_RunInit()
{
	// No improper quit popup
	nop(0x533F1A, 2);

	// Nop optimal settings
	nop(0x86D114, 5);

	// Fix steam start
	nop(0x86CFE3, 5);
	call(0x53E8D0, 0x53E8DF, PATCH_JUMP);

	// Custom command line
	*(char**)0x403683 = "+set dedicated 1";

	// Hook random frame function. FastFiles will all be loaded then.
	call(0x86C785, RunStuff, PATCH_CALL);

	// Don't initialize network stuff
	*(BYTE*)0x46EA60 = 0xC3;

	// Ignore dedi authentication
	nop(0x4ED3E8, 5);
	*(BYTE*)0x5AFD60 = 0xC3;
	*(BYTE*)0x5D4500 = 0xC3;
}