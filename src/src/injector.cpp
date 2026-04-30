#include "MinHook.h"
#include <windows.h>

// Original function pointer
typedef int(WINAPI *MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxA_t fpMessageBoxA = nullptr;

// Hooked function
int WINAPI HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption,
                             UINT uType) {
  // Modify the message
  return fpMessageBoxA(hWnd, "Hooked by MinHook!", lpCaption, uType);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine,
                   int nShowCmd) {
  // Initialize MinHook
  if (MH_Initialize() != MH_OK) {
    return 1;
  }

  // Create hook for MessageBoxA
  if (MH_CreateHook(reinterpret_cast<LPVOID *>(&MessageBoxA),
                    reinterpret_cast<LPVOID *>(&HookedMessageBoxA),
                    reinterpret_cast<LPVOID *>(&fpMessageBoxA)) != MH_OK) {
    return 1;
  }

  // Enable hook
  if (MH_EnableHook(reinterpret_cast<LPVOID *>(&MessageBoxA)) != MH_OK) {
    return 1;
  }

  // Test call
  MessageBoxA(NULL, "Original text", "Test", MB_OK);

  // Cleanup
  MH_DisableHook(reinterpret_cast<LPVOID *>(&MessageBoxA));
  MH_Uninitialize();

  return 0;
}
