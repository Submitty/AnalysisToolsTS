#include <stdio.h>

int max(int num1, int num2) {
  if (num1 > num2)
    return num1;
  else
    return num2;
}

int main() {
  int num1 = 21;
  int num2 = 52;
  int result = max(num1, num2);
  printf("Max : %d\n", result);
  return 0;
}
