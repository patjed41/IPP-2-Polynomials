/** @file
  Implementacja modułu udostępniającego stos wielomianów

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#include <stdlib.h>

#include "poly_stack.h"
#include "safe_functions.h"

PolyStack PolyStackNew() {
  PolyStack poly_stack;
  poly_stack.polys = (Poly *)safeMalloc(sizeof(Poly));
  poly_stack.polys_size = 1;
  poly_stack.num_of_polys = 0;
  return poly_stack;
}

void PolyStackDestroy(PolyStack *poly_stack) {
  while (poly_stack->num_of_polys > 0)
    PolyStackPop(poly_stack);
  free(poly_stack->polys);
}

void PolyStackPush(PolyStack *poly_stack, Poly p) {
  // Dwukrotne powiększenie poly_stack->polys, jeśli brakuje miejsca.
  if (poly_stack->num_of_polys == poly_stack->polys_size) {
    poly_stack->polys_size *= 2;
    poly_stack->polys = (Poly *)safeRealloc(poly_stack->polys,
                                poly_stack->polys_size * sizeof(Poly));
  }

  poly_stack->polys[poly_stack->num_of_polys] = p;
  poly_stack->num_of_polys++;
}

/**
 * Zwraca wielomian znajdujący się na szczycie stosu.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return wielomian
 */
static Poly *PolyStackTop(const PolyStack *poly_stack) {
  assert (poly_stack->num_of_polys > 0);
  return poly_stack->polys + (poly_stack->num_of_polys - 1);
}

/**
 * Zwraca wielomian znajdujący się pod wielomianem na szczycie stosu.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return wielomian
 */
static Poly *PolyStackSecondTop(const PolyStack *poly_stack) {
  assert (poly_stack->num_of_polys > 1);
  return poly_stack->polys + (poly_stack->num_of_polys - 2);
}

/**
 * Sprawdza czy stos wielomianów jest pusty.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return Czy stos jest pusty?
 */
static bool PolyStackIsEmpty(const PolyStack *poly_stack) {
  return poly_stack->num_of_polys == 0;
}

void PolyStackZero(PolyStack *poly_stack) {
  PolyStackPush(poly_stack, PolyZero());
}

bool PolyStackIsCoeff(const PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    if (PolyIsCoeff(PolyStackTop(poly_stack)))
      safePrintString("1\n");
    else
      safePrintString("0\n");
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackIsZero(const PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    if (PolyIsZero(PolyStackTop(poly_stack)))
      safePrintString("1\n");
    else
      safePrintString("0\n");
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackClone(PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack))
    PolyStackPush(poly_stack, PolyClone(PolyStackTop(poly_stack)));
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackAdd(PolyStack *poly_stack) {
  if (poly_stack->num_of_polys >= 2) {
    Poly new_top = PolyAdd(PolyStackTop(poly_stack),
                           PolyStackSecondTop(poly_stack));
    PolyStackPop(poly_stack);
    PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
    return true;
  }
  return false;
}

bool PolyStackMul(PolyStack *poly_stack) {
  if (poly_stack->num_of_polys >= 2) {
    Poly new_top = PolyMul(PolyStackTop(poly_stack),
                           PolyStackSecondTop(poly_stack));
    PolyStackPop(poly_stack);
    PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
    return true;
  }
  return false;
}

bool PolyStackNeg(PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    Poly new_top = PolyNeg(PolyStackTop(poly_stack));
    PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackSub(PolyStack *poly_stack) {
  if (poly_stack->num_of_polys >= 2) {
    Poly new_top = PolySub(PolyStackTop(poly_stack),
                           PolyStackSecondTop(poly_stack));
    PolyStackPop(poly_stack);
    PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
    return true;
  }
  return false;
}

bool PolyStackIsEq(PolyStack *poly_stack) {
  if (poly_stack->num_of_polys >= 2) {
    if (PolyIsEq(PolyStackTop(poly_stack), PolyStackSecondTop(poly_stack)))
      safePrintString("1\n");
    else
      safePrintString("0\n");
    return true;
  }
  return false;
}

bool PolyStackDeg(const PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    safePrintInt(PolyDeg(PolyStackTop(poly_stack)));
    safePrintChar('\n');
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackDegBy(const PolyStack *poly_stack, unsigned long long idx) {
  if (!PolyStackIsEmpty(poly_stack)) {
    safePrintInt(PolyDegBy(PolyStackTop(poly_stack), idx));
    safePrintChar('\n');
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackAt(PolyStack *poly_stack, long long x) {
  if (!PolyStackIsEmpty(poly_stack)) {
    Poly new_top = PolyAt(PolyStackTop(poly_stack), x);
    PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackPrint(const PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    PolyPrint(PolyStackTop(poly_stack));
    safePrintChar('\n');
  }
  return !PolyStackIsEmpty(poly_stack);
}

bool PolyStackPop(PolyStack *poly_stack) {
  if (!PolyStackIsEmpty(poly_stack)) {
    PolyDestroy(PolyStackTop(poly_stack));
    poly_stack->num_of_polys--;
    return true;
  }
  else {
    return false;
  }
}

bool PolyStackCompose(PolyStack *poly_stack, size_t k) {
  if (poly_stack->num_of_polys <= k) {
    return false;
  }
  else {
    Poly new_top =
      PolyCompose(PolyStackTop(poly_stack), k, poly_stack->polys +
        (poly_stack->num_of_polys - k - 1));
    for (size_t i = 0; i <= k; i++)
      PolyStackPop(poly_stack);
    PolyStackPush(poly_stack, new_top);
    return true;
  }
}





