#include"Common.h"

VOID IOCTLDistribute(WDFQUEUE Queue,
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength,
	ULONG IoControlCode) {
	//此处只是为了让编译通过,后续再编辑
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Request);
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);
	UNREFERENCED_PARAMETER(IoControlCode);
	NTSTATUS status;
	switch (IoControlCode) {
	case UserRead_IOCTL:
		status = UserRead(Request);
		break;
	default:
		DbgPrint("[Queue] Invalid IoControlCode: 0x%x\n", IoControlCode);
		WdfRequestCompleteWithInformation(Request, STATUS_INVALID_DEVICE_STATE, 0);
	}

	return;
}