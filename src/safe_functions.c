/** @file
  Implementacja modułu udostępniającego kilka "bezpiecznych" funkcji.

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#include <stdio.h>
#include <stdlib.h>

#include "safe_functions.h"

void *safeMalloc(size_t size) {
  void *ptr = malloc(size);

  if (ptr == NULL)
    exit(1);

  return ptr;
}

void *safeRealloc(void *memblock, size_t size) {
  void *ptr = realloc(memblock, size);

  if (ptr == NULL)
    exit(1);

  return ptr;
}

void safePrintError(int line_number, const char *error_type) {
  if (fprintf(stderr, "ERROR %d %s\n", line_number, error_type) < 0)
    exit(1);
}

void safePrintInt(int n) {
  if (printf("%d", n) < 0)
    exit(1);
}

void safePrintLong(long n) {
  if (printf("%ld", n) < 0)
    exit(1);
}

void safePrintChar(char ch) {
  if (printf("%c", ch) < 0)
    exit(1);
}

void safePrintString(char *str) {
  if (printf("%s", str) < 0)
    exit(1);
}