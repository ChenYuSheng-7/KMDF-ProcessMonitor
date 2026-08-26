#include"Common.h"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath) {
	NTSTATUS status;
	WDFDRIVER Driver;
	WDF_OBJECT_ATTRIBUTES Attributes;
	WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
	WDF_DRIVER_CONFIG Config;
	WDF_DRIVER_CONFIG_INIT(&Config, NULL);
	Config.DriverInitFlags = WdfDriverInitNonPnpDriver;
	Config.EvtDriverUnload = EvtWdfDriverUnload;
	status = WdfDriverCreate(DriverObject, RegistryPath,
		&Attributes, &Config, &Driver);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Device]WdfDriverCreate Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]WdfDriverCreate Success\n");
	//设备对象
	status = DeviceCreate(Driver);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Device]DeviceCreate Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]DeviceCreate Success\n");



	//注册进程监控回调
	status = PsSetCreateProcessNotifyRoutineEx(&MyProcessNotify, FALSE);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Device]PsSetCreateProcessNotifyRoutineEx Exit: 0x%x\n", status);
		return status;
	}
	DbgPrint("[Device]PsSetCreateProcessNotifyRoutineEx Success\n");

	return status;
}