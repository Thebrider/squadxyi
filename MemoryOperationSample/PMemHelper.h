#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <time.h>
#include <Psapi.h>
#include <functional>
#include <tlhelp32.h>
#include "xorstr.hpp"
#include "subauth.h"
#include "IOCTL_Define.h"
#include <TlHelp32.h>
#include <vector>
#include "globals.h"


class MemoryManagement;
class MemoryManagement {
public:
	HANDLE hDevice;
	bool Init();

	//BOOL Read_Address(LONGLONG pID, DWORD_PTR lpBaseAddress, PVOID buf, ULONG bufersize, BOOLEAN write);
	template <typename type>
	bool Read_Address(LONG pID, LONGLONG lpBaseAddress, type buf, SIZE_T bufersize, BOOLEAN write);
	_GETMODULERESULT GetModuleBase(LONG pID, const wchar_t* ModuleName);
	_GETMODULERESULT GetBase(LONG pID);
	uintptr_t FindProcessID(LPCTSTR ProcessName);
	template <typename T> T ReadData(uint64_t address);
	template <typename T> T ReadData(uint64_t address, int32_t size);
	template <typename T> void WriteData(uint64_t address, T data);
	string ReadString(uint64_t address, size_t size);
	uint64_t ReadChain(uint64_t base, const std::vector<int64_t>& offsets);
	std::string WstrToUtf8Str(const std::wstring& wstr);
	std::string GetUnicodeString(uint64_t addr);
};

template <typename type>
inline bool MemoryManagement::Read_Address(LONG pID, LONGLONG lpBaseAddress, type buf, SIZE_T bufersize, BOOLEAN write) {
	RWM copy;
	DWORD Bytes;
	copy.pid = pID;
	copy.tbuff = lpBaseAddress;
	copy.size = bufersize;
	copy.write = write;
	copy.lbuff = (LONGLONG)buf;

	if (DeviceIoControl(this->hDevice, IOCTL_RWM, &copy, sizeof(copy), &copy, sizeof(copy), &Bytes, NULL))
	{
		return true;
	}
	return false;
};

template <typename T>
inline T MemoryManagement::ReadData(uint64_t address)
{
	if (!address)
		return T();

	T buffer;
	Read_Address(globals.process_id, address, &buffer, sizeof(T), false) ? buffer : T();
	return buffer;
};
template <typename T>
inline T MemoryManagement::ReadData(uint64_t address, int32_t size)
{
	if (!address)
		return T();

	T buffer;
	Read_Address(globals.process_id, address, &buffer, size, false) ? buffer : T();
	return buffer;
};

template <typename T>
inline void MemoryManagement::WriteData(uint64_t address, T data)
{
	if (!address)
		return;

	Read_Address(globals.process_id, address, &data, sizeof(T), true) ? data : T();

};



extern MemoryManagement* memio;