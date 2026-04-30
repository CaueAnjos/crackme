#include "MinHook.h"

#include <iostream>

DWORD MainThread(HMODULE hModule) {
  std::cout << "Hello World!" << std::endl;
  return 0;
}
