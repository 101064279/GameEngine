// Alex Chao
// Paolo Geronimo
// Gavin Tang

#include "stdafx.h"
#include <atlhandler.h>
#include <iostream>
#include <winnt.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <DirectXPackedVector.h>
#include <direct.h>
#include <ctype.h>
#include <string>

using namespace DirectX;
using namespace std;

//Initializing Function
bool IsOnlyInstance();
bool CheckStorage(const DWORDLONG diskSpaceNeeded);
bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);



//Checks if an instance of the application is already running or not.
bool IsOnlyInstance()
{
	CreateMutex(NULL, TRUE, "IDK");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		cout << "more than one instances is running\n" << endl;
		return false;
	}
	cout << "no multiple instances is running\n" << endl;
	return true;
}

//Checks if device has required disk space needed.
bool CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	LPCTSTR lpDirectoryName = NULL;
	__int64 lpFreeBytesAvailable;
	__int64 lpTotalNumberOfBytes;
	__int64 lpTotalNumberOfFreeBytes;

	GetDiskFreeSpaceEx(lpDirectoryName, (PULARGE_INTEGER)&lpFreeBytesAvailable, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);

	if ((DWORDLONG)lpTotalNumberOfBytes <= diskSpaceNeeded) {
		cout << "\tCheck Storage Failure: Not enough physical storage.\n" << endl;
		return false;
	}
	else {
		cout << "Disk Space Check: \n" << endl;
		cout << "\tRequested " << diskSpaceNeeded << " bytes needed." << endl;
		cout << "\tThere are " << (DWORDLONG)lpTotalNumberOfBytes << " available bytes." << endl;

		return true;
	}
}

//Displays the size of physical and virtual memory available to use.
bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded) {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

	cout << "\nPhysical & Virtual Memory Check:\n" << endl;
	cout << "\tRAM Requested: " << physicalRAMNeeded << "." << endl;
	cout << "\tVirtual RAM Requested: " << virtualRAMNeeded << "." << endl;
	cout << "\tThe total physical RAM available: " << status.ullAvailPhys << "." << endl;
	cout << "\tThe total virtual RAM avaialable: " << status.ullAvailVirtual << ".\n" << endl;

	if (status.ullAvailPhys < physicalRAMNeeded) {
		cout << "\tCheckMemory Failure : Not enough physical memory.\n" << endl;

		return false;
	}

	if (status.ullAvailVirtual < virtualRAMNeeded) {
		cout << "\tCheckMemory Failure : Not enough virtual memory.\n" << endl;

		return false;
	}
}

DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0,
		KEY_READ,
		&hKey);
	if (lError == ERROR_SUCCESS) {
		// query the key:
		RegQueryValueEx(hKey,
			"MHz",
			NULL,
			&type,
			(LPBYTE)&dwMHz,
			&BufSize);
	}
	cout << "CPU speed: " << dwMHz / pow(1024, 3) << "GHz" << endl;
	return dwMHz;
}

int main()
{

	IsOnlyInstance();
	DWORDLONG spaceNeeded = 3000000000;
	CheckStorage(spaceNeeded);
	DWORDLONG ramNeeded = 800000000;
	DWORDLONG vRamNeeded = 160000000;
	CheckMemory(ramNeeded, vRamNeeded);
	ReadCPUSpeed();

	return 0;
}

