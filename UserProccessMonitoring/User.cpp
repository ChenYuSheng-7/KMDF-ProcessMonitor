#include"Common.h"

int ReadEvent(HANDLE hDevice) {
	DWORD bytesReturned = 0;
	UserEvent OutputBuffer = { 0 };
	DWORD Temp = FIELD_OFFSET(UserEvent, Event);
	DWORD UserCapacity = sizeof(PROCESS_EVENT) * 32 + Temp;
	cout << "UserBytesSIZE: " << UserCapacity << endl;
	BOOL ok = DeviceIoControl(
		hDevice,
		UserRead_IOCTL,
		NULL,
		0,
		(LPVOID*)&OutputBuffer,
		UserCapacity,//UserCapacity
		&bytesReturned,
		NULL
	);
	cout << "bytesReturned:"<< bytesReturned << endl;
	if (!ok) {
		cout << "UserRead_IOCTL Failed. Error = " << GetLastError() << endl;
		return 0;
	}
	cout << "UserRead_IOCTL Success " << endl;
	if (bytesReturned == 0) {
		cout << "No process event returned." << endl;
		return 0;
	}
	if (bytesReturned > UserCapacity) {
		cout << "Invalid eventCount: larger than buffer capacity." << endl;
		return 0;
	}
	DWORD EventNum = (bytesReturned- Temp)/ sizeof(PROCESS_EVENT);
	cout <<"EventNum:" << EventNum << endl;
	for (DWORD i = 0; i < EventNum; i++) {
		cout << "ProcessID :" << OutputBuffer.Event[i].ProcessID << " ";
		cout << "Sequence :" << OutputBuffer.Event[i].Sequence << " ";
		cout << "Type: "<< OutputBuffer.Event[i].Type << endl;
	}
	cout << "本次读取 " << EventNum <<" 有效事件" << endl;
	cout << "缓存区内有效事件数量共:" << OutputBuffer.CountBeforeRead << endl;
	cout << "丢弃事件共: " << OutputBuffer.UserDiscard << endl;



	return 0;
}