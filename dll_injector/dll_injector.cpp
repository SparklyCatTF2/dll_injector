#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>
#include <string>
#include <vector>

#define Handle HANDLE

using namespace std;

bool FileExists(const char *filename)
{
	ifstream file(filename);

	if (!file) return false;

	else return true;
}

bool PerformInjection(DWORD pId, const char *dllName)
{
	Handle hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);

	if (hProcess)
	{
		LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kelner32.dll"), "LoadLibraryA");
		LPVOID dereercomp = VirtualAllocEx(hProcess, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		WriteProcessMemory(hProcess, dereercomp, dllName, strlen(dllName), NULL);
		Handle adsc = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
		WaitForSingleObject(adsc, INFINITE);
		VirtualFreeEx(hProcess, dereercomp, strlen(dllName), MEM_RELEASE);

		CloseHandle(adsc);
		CloseHandle(hProcess);

		return true;
	}

	return false;
}

int main()
{
	HWND window;
	DWORD pId;
	window = FindWindow(NULL, L"roblox");

	if (!window)
	{
		cout << "NO WINDOW FOUND" << endl;
		system("PAUSE");
		return 0;
	}

	GetWindowThreadProcessId(window, &pId);

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);

	if (!process)
	{
		cout << "PROCESS NOT FOUND" << endl;
		return 0;
	}

	string loc;
	cout << "Roblox was found running, please enter the location of your DLL file (f.e C:/injectfolder/thingtoinject.dll)" << endl;
	cin >> loc;

	if (!FileExists(loc.c_str()))
	{
		cout << "Your DLL file " << loc << " does not exsist."<< endl;
		system("PAUSE");
		return 0;
	}

	if (!PerformInjection(pId, loc.c_str()))
	{
		cout << "Could not inject your dll to roblox" << endl;
		system("PAUSE");
		return 0;
	}

	cout << "Injected" << loc << "to roblox" << endl;

	system("pause");
	return 0;
}