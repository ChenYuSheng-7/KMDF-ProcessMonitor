#include"Common.h"

VOID EvtWdfDriverUnload(WDFDRIVER Driver) {
	UNREFERENCED_PARAMETER(Driver);

	NTSTATUS status;
	status = PsSetCreateProcessNotifyRoutineEx(&MyProcessNotify, TRUE);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[Unload]PsSetCreateProcessNotifyRoutineEx Close Exit: 0x%x\n", status);
		return ;
	}
	DbgPrint("[Unload]PsSetCreateProcessNotifyRoutineEx Close Success\n");


	DbgPrint("[Unload]Driver Stop Success\n");


	return;
}