#include <math.h>
#include <stdint.h>
#include <stdio.h>

double_t generate_password(double_t input);

typedef uint8_t bool;

bool approx_p(double_t a, double_t b, double_t precision) {
  return (a < b + precision) && (a > b - precision);
}

bool approx(double_t a, double_t b) { return approx_p(a, b, 0.0001); }

int main(int argc, char *argv[]) {
  const char world[] = "World";
  printf("Hello %s!\n", world);

  double_t input = 0;
  puts("gess the password!");
  int r = scanf("%lf", &input);
  if (0 == r) {
    return 1;
  }

  double_t calc = input + 2 * input;
  printf("%lf\n", calc);

  bool is_valid = approx(input, calc);
  if (is_valid) {
    puts("Good guess");
    return 0;
  }

  puts("Bad guess");
  return 1;
}

double_t generate_password(double_t input) { return input / (input * 2); }
