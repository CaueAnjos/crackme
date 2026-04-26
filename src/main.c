#include <math.h>
#include <stdint.h>
#include <stdio.h>

double_t generate_password(double_t input);

typedef uint8_t bool;

int main(int argc, char *argv[]) {
  const char world[] = "World";
  puts("Give me a number");

  int32_t n = 0;
  int32_t r = scanf("%d", &n);

  switch (n) {
  case 1:
    puts("1");
    break;
  case 2:
    puts("2");
    break;
  case 3:
    puts("3");
    break;
  default:
    puts("bad");
    break;
  }

  return 0;
}
