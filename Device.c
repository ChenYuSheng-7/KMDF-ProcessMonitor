#include"common.h"

NTSTATUS DeviceCreate(WDFDRIVER Driver) {
	NTSTATUS status;
	PWDFDEVICE_INIT DeviceInit = NULL;
	WDFDEVICE Device = NULL;
	WDF_OBJECT_ATTRIBUTES DeviceAttributes;
	WDF_OBJECT_ATTRIBUTES_INIT(&DeviceAttributes);
	DECLARE_CONST_UNICODE_STRING(SDDL, L"D:P(A;; GA;;; SY)(A;; GA;;; BA)");
	DeviceInit = WdfControlDeviceInitAllocate(Driver, &SDDL);
	if (DeviceInit == NULL) {
		status = STATUS_INSUFFICIENT_RESOURCES;
		DbgPrint("[Device]WdfControlDeviceInitAllocate Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]WdfControlDeviceInitAllocate Success\n");
	DECLARE_CONST_UNICODE_STRING(MyDeviceName, L"\\Device\\ProcessMonitoring");
	status = WdfDeviceInitAssignName(DeviceInit, &MyDeviceName);
	if (!NT_SUCCESS(status)) {
		WdfDeviceInitFree(DeviceInit);
		DeviceInit = NULL;
		DbgPrint("[Device]WdfDeviceInitAssignName Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]WdfDeviceInitAssignName Success\n");
	status = WdfDeviceCreate(&DeviceInit, &DeviceAttributes, &Device);
	if (!NT_SUCCESS(status)) {
		WdfDeviceInitFree(DeviceInit);
		DeviceInit = NULL;
		DbgPrint("[Device]WdfDeviceCreate Exit: 0x%x\n", status);
		return status;
	}
	DECLARE_CONST_UNICODE_STRING(DeviceSymbolicLink, DOS_DEVICE_SYMBOLICLINK);
	status = WdfDeviceCreateSymbolicLink(Device, &DeviceSymbolicLink);
	if (!NT_SUCCESS(status)) {
		WdfObjectDelete(Device);
		Device = NULL;
		DbgPrint("[Device]WdfDeviceCreateSymbolicLink Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]WdfDeviceCreateSymbolicLink Success\n");
	//创建IO队列
	status = QueueCreate(Device);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Device]QueueCreate Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]QueueCreate Success\n");
	//创建锁
	WDF_OBJECT_ATTRIBUTES LockAttributes;
	WDF_OBJECT_ATTRIBUTES_INIT(&LockAttributes);
	LockAttributes.ParentObject = Driver;
	status = WdfWaitLockCreate(&LockAttributes, &RingBuffer.LOCK);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Device]WdfWaitLockCreate LockAttributes Exit: 0x%x\n",status);
		return status;
	}
	DbgPrint("[Device]WdfWaitLockCreate LockAttributes Success\n");

	WdfControlFinishInitializing(Device);

	return status;
}