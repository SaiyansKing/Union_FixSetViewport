#include <Windows.h>
#include <algorithm>

int __fastcall FixSetViewPort_G1(DWORD zCRnd_D3D, DWORD _EDX, int x0, int y0, int width, int height)
{
	int gWidth = *reinterpret_cast<int*>(zCRnd_D3D + 0x984);
	int gHeight = *reinterpret_cast<int*>(zCRnd_D3D + 0x988);
	int x1 = x0 + width;
	int y1 = y0 + height;
	if(x0 > gWidth || y0 > gHeight || x1 < 0 || y1 < 0)
	{
		x1 = 0;
		y1 = 0;
		x0 = 0;
		y0 = 0;
	}
	else
	{
		x1 = std::min<int>(x1, gWidth);
		y1 = std::min<int>(y1, gHeight);
		x0 = std::max<int>(x0, 0);
		y0 = std::max<int>(y0, 0);
	}

	DWORD viewPort[6] = {static_cast<DWORD>(x0), static_cast<DWORD>(y0), static_cast<DWORD>(x1 - x0), static_cast<DWORD>(y1 - y0), 0x00000000, 0x3F800000};
	DWORD device = *reinterpret_cast<DWORD*>(0x929D5C);
	if(reinterpret_cast<int(__stdcall*)(DWORD, DWORD*)>(*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(device) + 0x34))(device, viewPort) < 0) return 0;
	return 1;
}

int __fastcall FixSetViewPort_G2(DWORD zCRnd_D3D, DWORD _EDX, int x0, int y0, int width, int height)
{
	int gWidth = *reinterpret_cast<int*>(zCRnd_D3D + 0x98C);
	int gHeight = *reinterpret_cast<int*>(zCRnd_D3D + 0x990);
	int x1 = x0 + width;
	int y1 = y0 + height;
	if(x0 > gWidth || y0 > gHeight || x1 < 0 || y1 < 0)
	{
		x1 = 0;
		y1 = 0;
		x0 = 0;
		y0 = 0;
	}
	else
	{
		x1 = std::min<int>(x1, gWidth);
		y1 = std::min<int>(y1, gHeight);
		x0 = std::max<int>(x0, 0);
		y0 = std::max<int>(y0, 0);
	}

	DWORD viewPort[6] = {static_cast<DWORD>(x0), static_cast<DWORD>(y0), static_cast<DWORD>(x1 - x0), static_cast<DWORD>(y1 - y0), 0x00000000, 0x3F800000};
	DWORD device = *reinterpret_cast<DWORD*>(0x9FC9F4);
	if(reinterpret_cast<int(__stdcall*)(DWORD, DWORD*)>(*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(device) + 0x34))(device, viewPort) < 0) return 0;
	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
    if(reason == DLL_PROCESS_ATTACH)
	{
		DWORD functionToOverWrite = 0;
		DWORD newFunction = 0;
        DWORD baseAddr = reinterpret_cast<DWORD>(GetModuleHandleA(nullptr));
		if(*reinterpret_cast<DWORD*>(baseAddr + 0x160) == 0x37A8D8 && *reinterpret_cast<DWORD*>(baseAddr + 0x37A960) == 0x7D01E4 && *reinterpret_cast<DWORD*>(baseAddr + 0x37A98B) == 0x7D01E8)
		{
			functionToOverWrite = baseAddr + 0x3DF088;
			newFunction = reinterpret_cast<DWORD>(&FixSetViewPort_G1);
		}
		else if(*reinterpret_cast<DWORD*>(baseAddr + 0x168) == 0x3D4318 && *reinterpret_cast<DWORD*>(baseAddr + 0x3D43A0) == 0x82E108 && *reinterpret_cast<DWORD*>(baseAddr + 0x3D43CB) == 0x82E10C)
		{
			functionToOverWrite = baseAddr + 0x43B22C;
			newFunction = reinterpret_cast<DWORD>(&FixSetViewPort_G2);
		}

		if(functionToOverWrite != 0 && newFunction != 0)
		{
			DWORD dwOldProtect, dwNewProtect;
			if(VirtualProtect(reinterpret_cast<LPVOID>(functionToOverWrite), 4, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{
				*reinterpret_cast<DWORD*>(functionToOverWrite) = newFunction;
				VirtualProtect(reinterpret_cast<LPVOID>(functionToOverWrite), 4, dwOldProtect, &dwNewProtect);
			}
		}
    }
    return TRUE;
}