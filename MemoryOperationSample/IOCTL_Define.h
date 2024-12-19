#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
using namespace std::string_literals;

#define DRIVER_NAME            L"gigantic"
#define DRIVER_DEVICE_NAME     L"\\Device\\gigantic"
#define DRIVER_DOS_DEVICE_NAME L"\\DosDevices\\gigantic"
#define DRIVER_DEVICE_PATH     L"\\\\.\\gigantic"
#define DRIVER_DEVICE_TYPE 0x00007071
 
#define IOCTL_INJECT CTL_CODE( DRIVER_DEVICE_TYPE, 0x804, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )
#define IOCTL_HANDLE CTL_CODE( DRIVER_DEVICE_TYPE, 0x803, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )
#define IOCTL_RWM CTL_CODE( DRIVER_DEVICE_TYPE, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )
#define IOCTL_ALLOC CTL_CODE( DRIVER_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )
#define IOCTL_GetModuleBase CTL_CODE( DRIVER_DEVICE_TYPE, 0x805, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS )

typedef struct _INJECT {
    LONG		pID;
    LONGLONG		imageBase;
} INJECT, * PINJECT;

typedef struct _RWM {
    LONGLONG lbuff;
    LONGLONG tbuff;
    LONG    size;
    LONG     pid;
    BOOLEAN   write;
} RWM, * PRWM;

typedef struct _ALLOC {
    LONG		pID;
    LONG		size;
    LONGLONG	dAddress;
    BOOLEAN		allocate;

    LONG		type;
    LONG		protection;
} ALLOC, * PALLOC;

typedef struct _GETMODULE {
    LONG	pID;
    wchar_t*        ModuleName;
} GETMODULE, * PGETMODULE;

typedef struct _ALLOCRESULT {
    LONGLONG	dAddress;
    LONG	size;
} ALLOCRESULT, * PALLOCRESULT;

typedef struct _GETMODULERESULT {
    LONGLONG	imageBase;
    LONG	size;
} GETMODULERESULT, * PGETMODULERESULT;