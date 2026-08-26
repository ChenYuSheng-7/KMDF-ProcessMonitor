#include"Common.h"

PROCESS_EVENT g_event = { 0 };
EVENT_RING_BUFFER RingBuffer = { 0 };

VOID MyProcessNotify(
	 _Inout_ PEPROCESS Process,
	 _In_ HANDLE ProcessId,
	 _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo) {
	UNREFERENCED_PARAMETER(Process);
	WdfWaitLockAcquire(RingBuffer.LOCK, NULL);
	g_event.ProcessID = HandleToULong(ProcessId);
	g_event.Sequence++;
	if (CreateInfo != NULL) {
		g_event.Type = 1;
	}
	else if (CreateInfo == NULL) {
		g_event.Type = 0;
	}

	if (RingBuffer.Count < BUFFER_SIZE_MAX) {
		RingBuffer.Count ++;
		
	}
	else if (RingBuffer.Count == BUFFER_SIZE_MAX) {
		RingBuffer.Read = (RingBuffer.Read + 1) % BUFFER_SIZE_MAX;
		RingBuffer.Discard++;
	}

	RingBuffer.Event[RingBuffer.Write] = g_event;  //先录入与输出

	DbgPrint("ProcessID:%lu ,Sequence: %lu, Type: %lu   ", RingBuffer.Event[RingBuffer.Write].ProcessID,
		RingBuffer.Event[RingBuffer.Write].Sequence, RingBuffer.Event[RingBuffer.Write].Type);
	
	//再自增
	RingBuffer.Write = (RingBuffer.Write + 1) % BUFFER_SIZE_MAX;

	DbgPrint("Count: %lu, Read: %lu, Write:%lu, Discard:%lu \n",RingBuffer.Count
	,RingBuffer.Read,RingBuffer.Write,RingBuffer.Discard);
	

	WdfWaitLockRelease(RingBuffer.LOCK);

	return;
}