#include <windows.h>

#include <iostream>
#include <string>
#include <tlhelp32.h>

DWORD GetPidByName(const std::wstring &processName) {
  DWORD pid = 0;
  // Take a snapshot of all processes
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot == INVALID_HANDLE_VALUE)
    return 0;

  PROCESSENTRY32W pe;
  pe.dwSize = sizeof(PROCESSENTRY32W);

  if (Process32FirstW(hSnapshot, &pe)) {
    do {
      std::wcout << "process: [" << pe.th32ProcessID << "]\t " << pe.szExeFile
                 << "\n";
      if (processName == pe.szExeFile) {
        pid = pe.th32ProcessID;
        break;
      }
    } while (Process32NextW(hSnapshot, &pe));
  }

  CloseHandle(hSnapshot);
  return pid;
}

bool InjectDLL(DWORD pid, const char *dllPath) {
  HANDLE hProcess =
      OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
                      PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
                  FALSE, pid);

  if (!hProcess) {
    std::cerr << "Failed to open process. Error: " << GetLastError() << "\n";
    return false;
  }

  // Allocate memory in the target process for the DLL path
  SIZE_T pathLen = strlen(dllPath) + 1;
  LPVOID remoteBuffer = VirtualAllocEx(
      hProcess, nullptr, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!remoteBuffer) {
    std::cerr << "VirtualAllocEx failed. Error: " << GetLastError() << "\n";
    CloseHandle(hProcess);
    return false;
  }

  // Write the DLL path into the target process
  if (!WriteProcessMemory(hProcess, remoteBuffer, dllPath, pathLen, nullptr)) {
    std::cerr << "WriteProcessMemory failed. Error: " << GetLastError() << "\n";
    VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return false;
  }

  // Get the address of LoadLibraryA in kernel32.dll
  HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
  FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");

  // Create a remote thread that calls LoadLibraryA with our DLL path
  HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
                                      (LPTHREAD_START_ROUTINE)loadLibraryAddr,
                                      remoteBuffer, 0, nullptr);

  if (!hThread) {
    std::cerr << "CreateRemoteThread failed. Error: " << GetLastError() << "\n";
    VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return false;
  }

  WaitForSingleObject(hThread, INFINITE);

  // Cleanup
  DWORD exitCode = 0;
  GetExitCodeThread(hThread, &exitCode);
  CloseHandle(hThread);
  VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
  CloseHandle(hProcess);

  return exitCode != 0;
}

int main() {
  std::wstring processName = L"zellij";
  const char *dllPath = "";

  std::wcout << L"Looking for process: " << processName << L"\n";

  DWORD pid = GetPidByName(processName);
  if (pid == 0) {
    std::cerr << "Process not found. Make sure the game is running.\n";
    return 1;
  }

  std::cout << "Found process with PID: " << pid << "\n";
  return 0;

  if (InjectDLL(pid, dllPath)) {
    std::cout << "DLL injected successfully!\n";
  } else {
    std::cerr << "Injection failed.\n";
    return 1;
  }

  return 0;
}
