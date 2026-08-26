#pragma once
#include<ntddk.h>
#include<wdf.h>
#include <wdmsec.h>

#define DOS_DEVICE_SYMBOLICLINK  L"\\DosDevices\\MyDevice"
#define BUFFER_SIZE_MAX 128
#define MAX_BATCH_EVENTS 32
#define UserRead_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

EVT_WDF_DRIVER_UNLOAD EvtWdfDriverUnload;
NTSTATUS DeviceCreate(WDFDRIVER Driver);
NTSTATUS QueueCreate(WDFDEVICE Device);
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL IOCTLDistribute;
NTSTATUS UserRead(WDFREQUEST Request);

VOID MyProcessNotify(
	_Inout_ PEPROCESS Process,
	_In_ HANDLE ProcessId,
	_Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);

typedef struct _PROCESS_EVENT {
	ULONG Sequence;
	ULONG ProcessID;
	ULONG Type;
}PROCESS_EVENT, * PPROCESS_EVENT;

typedef struct _User_Event {
	ULONG CountBeforeRead;
	ULONG UserDiscard;
	
	PROCESS_EVENT Event [1];
}UserEvent,*PUserEvent;


typedef struct _EVENT_RING_BUFFER {
	PROCESS_EVENT Event[BUFFER_SIZE_MAX];
	ULONG Read;     //读取位置
	ULONG Write;    //写入位置
	ULONG Count;    //现有事件数量
	ULONG Discard;  //丢弃事件数量

	WDFWAITLOCK LOCK;
}EVENT_RING_BUFFER,* PEVENT_RING_BUFFER;

extern EVENT_RING_BUFFER RingBuffer ;

