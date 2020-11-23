#include "procmanager_winapi.h"

#include <Windows.h>
#include <Psapi.h>

#include <QString>
#include <QDebug>
#include <QFileInfo>

Pid ProcManagerWindows::find(const QString &executable)
{
	Pid result = 0;

	static DWORD processIds[2048];
	static const DWORD cb = sizeof(processIds);
	DWORD bytesReturned;

	QString key = QFileInfo(executable).fileName();

	EnumProcesses(processIds, cb, &bytesReturned);

	size_t numberOfProcesses = bytesReturned / sizeof(DWORD);

	for (size_t i = 0u; i < numberOfProcesses; i++) {
		DWORD pid = processIds[i];
		HANDLE hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, pid
		);
		if (hProcess) {
			wchar_t buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			DWORD bufSize = GetModuleBaseNameW(hProcess, 0, buffer, 1024u);
			CloseHandle(hProcess);
			QString procName = QString::fromWCharArray(buffer, bufSize);
			if (procName == key) {
				result = pid;
				break;
			}
		}
	}

	return result;
}
