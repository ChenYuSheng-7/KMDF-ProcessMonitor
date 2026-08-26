#pragma once
#include<Windows.h>
#include<iostream>
using namespace std;

#define UserRead_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define BUFFER_SIZE_MAX 128

int ReadEvent(HANDLE hDevice);

typedef struct _PROCESS_EVENT {
	ULONG Sequence;
	ULONG ProcessID;
	ULONG Type;
}PROCESS_EVENT, * PPROCESS_EVENT;

typedef struct _User_Event {
	ULONG CountBeforeRead;
	ULONG UserDiscard;

	PROCESS_EVENT Event[32];
}UserEvent, * PUserEvent;