#include "execute.h"

//#define WINDOWS_LAB68
#define POSIX_LAB68

#ifdef WINDOWS_LAB68

#include <windows.h>
#include <pathcch.h>

void CopyCchStringToWSTR(const string& str, WCHAR* wstr, size_t cch)
{
    ZeroMemory(wstr, sizeof(WCHAR)*cch);
    for (size_t i = 0; i < min(str.length(), cch-1); i++)
    {
        wstr[i] = str[i];   //NOTE: char -> WCHAR
    }
}

int execute(const string& program, const int parametersLength, const string parameters[])
{
    WCHAR programW[MAX_PATH + 1];
    CopyCchStringToWSTR(program, programW, MAX_PATH+1);

    WCHAR pathW[MAX_PATH+1];
    GetModuleFileNameW(NULL, pathW, MAX_PATH);
    PathCchRemoveFileSpec(pathW, MAX_PATH);
    PathCchAppend(pathW, MAX_PATH, programW);
    PathCchRenameExtension(pathW, MAX_PATH, L".exe");

    string commandLine = program;
    for (int i = 0; i < parametersLength; i++)
        commandLine = commandLine + " " + parameters[i];
    WCHAR commandLineW[MAX_PATH+1];
    CopyCchStringToWSTR(commandLine, commandLineW, MAX_PATH + 1);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL success = CreateProcessW(pathW, commandLineW,
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi);           // Pointer to PROCESS_INFORMATION structure

    if (!success)
    {
        DWORD eee = GetLastError();
        return -1;
    }

    int processId = pi.dwProcessId;

    //CloseHandle(pi.hProcess);
    //CloseHandle(pi.hThread);

    return processId;
}

#endif

#ifdef POSIX_LAB68

#include <iostream>
#include <unistd.h>

using namespace std;

int execute(const string& program, const int parametersLength, const string parameters[])
{
    int processId = fork();
    if (processId == -1)  // error
        return -1;
    else if (processId == 0)  // child process
    {
        char * argv[] = new char* [parametersLength + 2]; // extra 2 items
        argv[0] = (char*)program.c_str();      // the 1st extra item
        for (int i = 0; i < parametersLength; i++)
            argv[i + 1] = (char*)parameters[i].c_str();
        argv[parametersLength + 1] = nullptr;       // the 2nd extra items

        execv(program.c_str(), argv);
        exit(0);
    }
    else    //  parent process
        return processId;
}
#endif
