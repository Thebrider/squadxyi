#pragma once
#include "IOCTL_Define.h"
#include <TlHelp32.h>
#include <vector>
#include "..\\globals.h"

static uintptr_t FindProcessID(LPCTSTR ProcessName);
class MemoryManagement
{
public:
	HANDLE hDevice;
	bool Init();
	
	//BOOL Read_Address(LONGLONG pID, DWORD_PTR lpBaseAddress, PVOID buf, ULONG bufersize, BOOLEAN write);
	template <typename type>
	bool Read_Address(ULONG pID, ULONGLONG lpBaseAddress, type buf, SIZE_T bufersize, BOOLEAN write);
	_GETMODULERESULT GetModuleBase(ULONG pID, CHAR* ModuleName);
	_HEDRETERN GetBase(ULONG pID);
	};
inline MemoryManagement mem = MemoryManagement();
template <typename T> T ReadData(uint64_t address);
template <typename T> T ReadData(uint64_t address,uint32_t size);
template <typename T> void WriteData(uint64_t address, T data);
string ReadString(uint64_t address, size_t size);
uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets);
std::string WstrToUtf8Str(const std::wstring& wstr);
std::string GetUnicodeString(uint64_t addr, int stringLength);