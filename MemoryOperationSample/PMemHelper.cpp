#include "PMemHelper.h"

#include "eftstructs.h"
#include <codecvt>
#pragma comment(lib, "Ws2_32")
#pragma warning(disable : 4996)
uintptr_t MemoryManagement::FindProcessID(LPCTSTR ProcessName) // non-conflicting function name
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) { // must call this first
		do {
			if (!lstrcmpi(pt.szExeFile, ProcessName)) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap); // close handle on failure
	return 0;
};

bool MemoryManagement::Init() {

	this->hDevice = CreateFileW(DRIVER_DEVICE_PATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (this->hDevice == INVALID_HANDLE_VALUE)
		return 0;
	globals.process_id = FindProcessID(_xor_("SquadGame.exe"));
    globals.Base = GetBase(globals.process_id).imageBase;
   // globals.GOM = this->ReadData<uint64_t>(globals.Base + 0x17FFD28);
   // globals.HardSettings = memio->ReadChain(globals.Base, { 0x17FFB20, -0x28, 0xD0, 0x8, 0x58, 0x40, 0x30, 0x600 });
    globals.GameWorld = ReadData<uintptr_t>(globals.Base + OFF_WORLD);
    return true;
	


};

//BOOL Read_Address(LONGLONG pID, DWORD_PTR lpBaseAddress, PVOID buf, ULONG bufersize, BOOLEAN write);
_GETMODULERESULT MemoryManagement::GetModuleBase(LONG pID, const wchar_t* ModuleName) {
	DWORD Bytes;
	_GETMODULE ModuleBase;
	ModuleBase.pID = pID;
	_GETMODULERESULT Result;
	ModuleBase.ModuleName = (wchar_t*)ModuleName;
	DeviceIoControl(this->hDevice, IOCTL_GetModuleBase, &ModuleBase, sizeof(ModuleBase), &Result, sizeof(Result), &Bytes, 0);
	return Result;
};
_GETMODULERESULT MemoryManagement::GetBase(LONG pID) {
	DWORD Bytes;
	_GETMODULE ModuleBase;
	_GETMODULERESULT ModuleBaseret;
	ModuleBase.pID = pID;
	DeviceIoControl(this->hDevice, IOCTL_HANDLE, &ModuleBase, sizeof(ModuleBase), &ModuleBaseret, sizeof(ModuleBaseret), &Bytes, 0);
	return ModuleBaseret;
};

string MemoryManagement::ReadString(uint64_t address, size_t size)
{

	if (!address || size > 1024)
		return "";

	char string[1024] = "";
	return Read_Address(globals.process_id, address, string, size, false) ? std::string(string) : "";
};




uint64_t MemoryManagement::ReadChain(uint64_t base, const std::vector<int64_t>& offsets) {
	uint64_t result = ReadData<uint64_t>(base + offsets.at(0));
	for (int i = 1; i < offsets.size(); i++) {
		result = ReadData<uint64_t>(result + offsets.at(i));
	}
	return result;
};

std::string MemoryManagement::WstrToUtf8Str(const std::wstring& wstr)
{
	std::string retStr;
	if (!wstr.empty())
	{
		int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

		if (sizeRequired > 0)
		{
			std::vector<char> utf8String(sizeRequired);
			int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
				-1, &utf8String[0], utf8String.size(), NULL,
				NULL);
			if (bytesConverted != 0)
			{
				retStr = &utf8String[0];
			}
			else
			{
				retStr = "GAY PARTY";
			}
		}
	}
	return retStr;
};


namespace conv {

    namespace multibyte {
        std::string  convert(const wchar_t* s);
        std::wstring convert(const char* s);
        std::string  convert(const std::wstring& s);
        std::wstring convert(const std::string& s);
    }// namespace multibyte 

    namespace utf8 {
        std::string  convert(const std::wstring& s);
        std::wstring convert(const std::string& s);
    }// namespace utf8 

    namespace stdlocal {

        std::string convert(
            const wchar_t* s,
            const size_t len,
            const std::locale& loc = std::locale(),
            const char default_char = '?'
        );

        std::string convert(
            const wchar_t* s,
            const std::locale& loc = std::locale(""),
            const char default_char = '?'
        );
        std::string convert(
            const std::wstring& s,
            const std::locale& loc = std::locale(""),
            const char default_char = '?'
        );

        std::wstring convert(
            const char* s,
            const std::locale& loc = std::locale("")
        );
        std::wstring convert(
            const std::string& s,
            const std::locale& loc = std::locale("")
        );

        std::wstring convert(
            const char* s,
            const size_t len,
            const std::locale& loc = std::locale()
        );

    } //namespace stdlocal 

}//namespace conv

// --- conv.cpp


namespace conv {

    namespace multibyte {

        std::string convert(const std::wstring& s)
        {
            return convert(s.c_str());
        }
        std::wstring convert(const std::string& s)
        {
            return convert(s.c_str());
        }

        std::string convert(const wchar_t* s)
        {
            const size_t len = wcslen(s);
            std::string result(len, '\0');

#ifdef _MSC_VER
            size_t count = 0;
            const auto size_ = result.size();
            wcstombs_s(&count, &result[0], size_, s, size_);
#else
            std::mbstate_t state = std::mbstate_t();
            std::wcsrtombs(&result[0], &s, result.size(), &state);
#endif

            return result;
        }

        std::wstring convert(const char* s)
        {
            std::mbstate_t state = std::mbstate_t();

            size_t len;
            wchar_t wc[4] = {};
            std::wstring result;
            const char* end = s + std::strlen(s);
            while ((len = std::mbrtowc(wc, s, end - s, &state)) > 0)
            {
                result += wc; s += len;
                for (auto& ch : wc)  ch = L'\0';
            }
            return result;
        }

    }//namespace multibyte

    namespace utf8 {

        std::string  convert(const std::wstring& s)
        {
            typedef std::codecvt_utf8<wchar_t>
                convert_typeX;
            std::wstring_convert<convert_typeX, wchar_t>
                converterX;
            return converterX.to_bytes(s);
        }
        std::wstring convert(const std::string& s)
        {
            typedef std::codecvt_utf8<wchar_t>
                convert_typeX;
            std::wstring_convert<convert_typeX, wchar_t>
                converterX;
            return converterX.from_bytes(s);
        }

    }//namespace utf8 

    namespace stdlocal {

        std::wstring convert(const char* first, const size_t len, const std::locale& loc)
        {
            if (len == 0)
                return std::wstring();
            const std::ctype<wchar_t>& facet =
                std::use_facet< std::ctype<wchar_t> >(loc);
            const char* last = first + len;
            std::wstring result(len, L'\0');
            facet.widen(first, last, &result[0]);
            return result;
        }

        std::string convert(
            const wchar_t* first,
            const size_t len,
            const std::locale& loc,
            const char default_char
        )
        {
            if (len == 0)
                return std::string();
            const std::ctype<wchar_t>& facet =
                std::use_facet<std::ctype<wchar_t> >(loc);
            const wchar_t* last = first + len;
            std::string result(len, default_char);
            facet.narrow(first, last, default_char, &result[0]);
            return result;
        }

        std::string  convert(const wchar_t* s, const std::locale& loc, const char default_char)
        {
            return convert(s, std::wcslen(s), loc, default_char);
        }
        std::string  convert(const std::wstring& s, const std::locale& loc, const char default_char)
        {
            return convert(s.c_str(), s.length(), loc, default_char);
        }
        std::wstring convert(const char* s, const std::locale& loc)
        {
            return convert(s, std::strlen(s), loc);
        }
        std::wstring convert(const std::string& s, const std::locale& loc)
        {
            return convert(s.c_str(), s.length(), loc);
        }

    }//namespace stdlocal
}//namespace conv

std::string MemoryManagement::GetUnicodeString(uint64_t addr)
{
	wchar_t wcharTemp[300] = { '\0' };
	int32_t stringLength = memio->ReadData<int32_t>(addr + 0x10);
	if (stringLength <= 0 || stringLength >= 300)
		return WstrToUtf8Str(wcharTemp);	
	Read_Address(globals.process_id, addr + 0x14, wcharTemp, (stringLength * 2) + 2, false);
	//std::string u8_conv = WstrToUtf8Str(wcharTemp);
	std::wstring ws = (wcharTemp);
    std::string test = conv::stdlocal::convert(ws);
	return test;
}

