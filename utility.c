#include "utility.h"

int IsEven(void* data) {
  return (*(int*)data) % 2 == 0;
}

void IncrementInt(void* data) {
  ++(*(int*)data);
}

