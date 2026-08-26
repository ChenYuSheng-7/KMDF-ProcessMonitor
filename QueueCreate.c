#include"Common.h"

NTSTATUS QueueCreate(WDFDEVICE Device) {
	NTSTATUS status;
	WDF_IO_QUEUE_CONFIG QueueConfig;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&QueueConfig, WdfIoQueueDispatchParallel);
	WDF_OBJECT_ATTRIBUTES QueueAttributes;
	WDF_OBJECT_ATTRIBUTES_INIT(&QueueAttributes);
	QueueAttributes.ExecutionLevel = WdfExecutionLevelPassive;
	WDFQUEUE Queue;
	QueueConfig.EvtIoDeviceControl = IOCTLDistribute;
	status = WdfIoQueueCreate(Device, &QueueConfig, &QueueAttributes, &Queue);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[QueueCreate]WdfIoQueueCreate Exit: 0x%x\n", status);
		return status;
	}

	DbgPrint("[QueueCreate]WdfIoQueueCreate Success\n");

	return status;
}