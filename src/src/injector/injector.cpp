#include <windows.h>

#include <iostream>
#include <tlhelp32.h>

DWORD GetProcessId(const wchar_t *processName) {
  DWORD pid = 0;
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snap == INVALID_HANDLE_VALUE)
    return 0;

  PROCESSENTRY32W entry = {sizeof(entry)};
  if (Process32FirstW(snap, &entry)) {
    do {
      if (_wcsicmp(entry.szExeFile, processName) == 0) {
        pid = entry.th32ProcessID;
        break;
      }
    } while (Process32NextW(snap, &entry));
  }
  CloseHandle(snap);
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
  const wchar_t *processName = L"LEGOMARVEL.exe";
  const char *dllPath = "C:\\Path\\To\\Your\\Mod.dll"; // <-- change this

  std::wcout << L"Looking for process: " << processName << L"\n";

  DWORD pid = GetProcessId(processName);
  if (pid == 0) {
    std::cerr << "Process not found. Make sure the game is running.\n";
    return 1;
  }

  std::cout << "Found process with PID: " << pid << "\n";

  if (InjectDLL(pid, dllPath)) {
    std::cout << "DLL injected successfully!\n";
  } else {
    std::cerr << "Injection failed.\n";
    return 1;
  }

  return 0;
}
