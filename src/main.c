#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

int my_add(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  lua_pushinteger(L, a + b);
  return 1;
}
int main() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  lua_register(L, "add", my_add);
  luaL_dostring(L, "print(add(10, 20))");
  lua_close(L);
  return 0;
}
