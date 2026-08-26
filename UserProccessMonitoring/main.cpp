#include"Common.h"

int main() {

	HANDLE hDevice = CreateFileW(
		L"\\\\.\\MyDevice",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hDevice == INVALID_HANDLE_VALUE) {
		cout << "CreateFile Failed. Error:" << GetLastError() << endl;
		return 1;
	}
	cout << "CreateFile Success" << endl;

    ReadEvent(hDevice);


	CloseHandle(hDevice);


	system("pause");




	return 0;

}