#include <windows.h>

#include <MinHook.h>
#include <fstream>

// Simple log helper
std::ofstream g_log;

void Log(const char *msg) {
  if (g_log.is_open())
    g_log << msg << std::endl;
}

// --- Hook example: MessageBoxA just to verify MinHook works ---
typedef int(WINAPI *pMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
pMessageBoxA fpMessageBoxA = nullptr;

int WINAPI HookedMessageBoxA(HWND hWnd, LPCSTR text, LPCSTR caption,
                             UINT type) {
  Log("[Hook] MessageBoxA intercepted!");
  // Call the original
  return fpMessageBoxA(hWnd, text, caption, type);
}

void InitHooks() {
  if (MH_Initialize() != MH_OK) {
    Log("[MinHook] MH_Initialize failed");
    return;
  }

  // Hook MessageBoxA as a simple test
  if (MH_CreateHookApi(L"user32", "MessageBoxA", (LPVOID)HookedMessageBoxA,
                       (LPVOID *)&fpMessageBoxA) != MH_OK) {
    Log("[MinHook] MH_CreateHookApi failed");
    return;
  }

  if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
    Log("[MinHook] MH_EnableHook failed");
    return;
  }

  Log("[MinHook] All hooks enabled successfully");
}

void CleanupHooks() {
  MH_DisableHook(MH_ALL_HOOKS);
  MH_Uninitialize();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
  switch (reason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hModule);
    g_log.open("hook.log", std::ios::out | std::ios::trunc);
    Log("[DLL] Injected into process!");
    InitHooks();
    break;

  case DLL_PROCESS_DETACH:
    CleanupHooks();
    Log("[DLL] Detached.");
    g_log.close();
    break;
  }
  return TRUE;
}
