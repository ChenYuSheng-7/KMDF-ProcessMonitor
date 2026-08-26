#include"Common.h"

NTSTATUS UserRead(WDFREQUEST Request) {
	NTSTATUS status;
	size_t headerSize;
	PUserEvent OutputBuffer = NULL;
	headerSize = FIELD_OFFSET(UserEvent, Event);
	size_t BufferLength = 0;
	size_t Bytesize = 0;
	status = WdfRequestRetrieveOutputBuffer(Request, headerSize,
		(PVOID*)&OutputBuffer, &BufferLength);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[UserRead]WdfRequestRetrieveOutputBuffer Exit: 0x%x", status);
		WdfRequestCompleteWithInformation(Request, status, 0);
		return status;
	}

	//实际能容纳的事件数量
	size_t UserCapacity = (BufferLength - headerSize) / sizeof(PROCESS_EVENT);

	WdfWaitLockAcquire(RingBuffer.LOCK, NULL);
	ULONG ActualCount;
	if (RingBuffer.Count < UserCapacity) {
		ActualCount = RingBuffer.Count;
	}
	else {
		ActualCount = (ULONG)UserCapacity;
	}
	if (ActualCount > MAX_BATCH_EVENTS) {
		ActualCount = MAX_BATCH_EVENTS;
	}
	OutputBuffer->CountBeforeRead = RingBuffer.Count;	
	for (ULONG i = 0; i < ActualCount; i++) {
		OutputBuffer->Event[i] = RingBuffer.Event[(RingBuffer.Read + i) % BUFFER_SIZE_MAX];
	}
	OutputBuffer->UserDiscard = RingBuffer.Discard;
	Bytesize = headerSize + sizeof(PROCESS_EVENT) * ActualCount;
	RingBuffer.Read = (RingBuffer.Read + ActualCount) % BUFFER_SIZE_MAX;
	RingBuffer.Count -= ActualCount;

	WdfWaitLockRelease(RingBuffer.LOCK);

	WdfRequestCompleteWithInformation(Request, status, Bytesize);

	return status;
}