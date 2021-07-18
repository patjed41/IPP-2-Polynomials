/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#include "poly.h"
#include "safe_functions.h"
#include <stdlib.h>

/**
 * Komparator porównujący jednomiany po wykładniku wykorzystywany przez qsort.
 * @param[in] ptr1 : wskaźnik na jednomian @f$m@f$
 * @param[in] ptr2 : wskaźnik na jednomian @f$n@f$
 * */
static int MonoCompByExp(const void *ptr1, const void *ptr2) {
  Mono *m = (Mono *)ptr1;
  Mono *n = (Mono *)ptr2;

  if (m->exp < n->exp)
    return -1;
  else if (m-> exp == n->exp)
    return 0;
  else
    return 1;
}

/**
 * Sortuje rosnąco jednomiany w wielomianie po wykładniku.
 * Zwracany wielomian jest tożsamościowo równy @p p.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian
 * @return posortowany wielomian
 */
static Poly PolySortByExp(const Poly *p) {
  if (!PolyIsCoeff(p))
    qsort(p->arr, p->size, sizeof(Mono), MonoCompByExp);
  return *p;
}

/**
 * Dodaje dwa wielomiany i usuwa je z pamięci.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
static Poly PolyAddAndClean(Poly *p, Poly *q) {
  Poly sum = PolyAdd(p, q);
  PolyDestroy(p);
  PolyDestroy(q);
  return sum;
}

/**
 * Łączy sąsiednie jednomiany o równym wykładniku w wielomianie.
 * Zwracany wielomian jest tożsamościowo równy @p p.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian
 * @return wielomian z połączonymi sąsiednimi jednomianami o równym wykładniku
 */
static Poly PolyMergeMonosWithEqualExp(Poly *p) {
  if (!PolyIsCoeff(p)) {
    int actual = 0;
    for (size_t i = 1; i < p->size; i++) {
      if (p->arr[i].exp == p->arr[actual].exp) {
        p->arr[actual].p = PolyAddAndClean(&(p->arr[actual].p), &(p->arr[i].p));
      }
      else {
        actual++;
        p->arr[actual] = p->arr[i];
      }
    }

    p->size = actual + 1;
  }

  return *p;
}

/**
 * Usuwa jednomiany zerowe w wielomianie.
 * Zwracany wielomian jest tożsamościowo równy @p p.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian
 * @return wielomian z usuniętymi jednomianami zerowymi
 */
static Poly PolyDeleteZeros(Poly *p) {
  if (!PolyIsCoeff(p)) {
    int next = 0;
    for (size_t i = 0; i < p->size; i++) {
      if (!PolyIsZero(&(p->arr[i].p))) {
        p->arr[next] = p->arr[i];
        next++;
      }
      else {
        MonoDestroy(&(p->arr[i]));
      }
    }
    // Okazało się, że p jest wielomianem stale równym 0.
    if (next == 0) {
      PolyDestroy(p);
      Poly zero = PolyZero();
      return zero;
    }
    else {
      p->size = next;
    }
  }

  return *p;
}

/**
 * Sprowadza wielomian @f$Cx_i^0@f$ do współczynnika @f$C@f$.
 * Jeśli wielomian jest innej postaci, nie rusza go.
 * Po wywołaniu tej funcji wielomian będący współczynnikiem jest reprezentowany
 * jednoznacznie.
 * Zwracany wielomian jest tożsamościowo równy @p p.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian
 * @return wielomian, być może poprawiony, jeśli jest współczynnikiem
 */
static Poly PolyCorrectIfCoeff(const Poly *p) {
  if (!PolyIsCoeff(p) && p->size == 1 && p->arr[0].exp == 0 &&
      PolyIsCoeff(&(p->arr[0].p))) {
    Poly p_correct = PolyClone(&(p->arr[0].p));
    PolyDestroy((Poly *) p);
    return p_correct;
  }
  else {
    return *p;
  }
}

/**
 * Sprowadza wielomian do jednoznaczej, uporządkowanej postaci.
 * Tożsamościowo równe wielomiany sprowadzone do jednoznacznej postaci są
 * zawsze reprezentowane w dokładnie ten sam sposób.
 * Jednoznaczna, uporządkowana postać wielomianu oznacza:
 * - posortowanie jednomianów rosnąco względem wykładników
 * - złączenie (dodanie) wszystkich jednomianów o równych wykładnikach
 * - brak jednomianów zerowych
 * - jednoznaczne reprezentowanie wielomianu będącego współczynnikiem
 *
 * Zwracany wielomian jest tożsamościowo równy @p p.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian
 * @return wielomian w jednoznacznej, uporządkowanej postaci
 */
static Poly PolyCorrect(const Poly *p) {
  Poly p_sorted = PolySortByExp(p);
  Poly p_merged = PolyMergeMonosWithEqualExp(&p_sorted);
  Poly p_without_zeros = PolyDeleteZeros(&p_merged);
  Poly p_correct = PolyCorrectIfCoeff(&p_without_zeros);
  return p_correct;
}

void PolyDestroy(Poly *p) {
  if (!PolyIsCoeff(p)) {
    for (size_t i = 0; i < p->size; i++)
      MonoDestroy(&(p->arr[i]));
    free(p->arr);
  }
}

Poly PolyClone(const Poly *p) {
  Poly clone;

  if (PolyIsCoeff(p)) {
    clone = PolyFromCoeff(p->coeff);
  }
  else {
    clone.size = p->size;
    clone.arr = (Mono *)safeMalloc(clone.size * sizeof(Mono));
    for (size_t i = 0; i < clone.size; i++)
      clone.arr[i] = MonoClone(&(p->arr[i]));
  }

  return clone;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
  Poly sum;

  if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    sum = PolyFromCoeff(p->coeff + q->coeff);
  }
  else {
    sum.size = (PolyIsCoeff(p) ? 1 : p->size) + (PolyIsCoeff(q) ? 1 : q->size);
    sum.arr = (Mono *)safeMalloc(sum.size * sizeof(Mono));

    if (PolyIsCoeff(p)) {
      Poly p_clone = PolyClone(p);
      sum.arr[0] = MonoFromPoly(&p_clone, 0);
    }
    else {
      for (size_t i = 0; i < p->size; i++)
        sum.arr[i] = MonoClone(&(p->arr[i]));
    }

    if (PolyIsCoeff(q)) {
      Poly q_clone = PolyClone(q);
      sum.arr[sum.size - 1] = MonoFromPoly(&q_clone, 0);
    }
    else {
      for (size_t i = 0; i < q->size; i++)
        sum.arr[i + sum.size - q->size] = MonoClone(&(q->arr[i]));
    }

    // uporządkowanie zwracanego wielomianu
    sum = PolyCorrect(&sum);
  }

  return sum;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
  Poly p;

  if (count == 0 || monos == NULL) {
    p = PolyZero();
  }
  else {
    p.size = count;
    p.arr = (Mono *)safeMalloc(p.size * sizeof(Mono));
    for (size_t i = 0; i < count; i++)
      p.arr[i] = monos[i];

    // uporządkowanie zwracanego wielomianu
    p = PolyCorrect(&p);
  }

  return p;
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
  Poly p = PolyAddMonos(count, monos);
  free(monos);
  return p;
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
  Mono *monos_clone = calloc(count, sizeof(Mono));
  for (size_t i = 0; i < count; i++)
    monos_clone[i] = MonoClone(&(monos[i]));
  return PolyOwnMonos(count, monos_clone);
}

Poly PolyMul(const Poly *p, const Poly *q) {
  Poly prod;

  if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    prod.coeff = p->coeff * q->coeff;
    prod.arr = NULL;
  }
  else {
    prod.size = (PolyIsCoeff(p) ? 1 : p->size) * (PolyIsCoeff(q) ? 1 : q->size);
    prod.arr = (Mono *)safeMalloc(prod.size * sizeof(Mono));

    if (PolyIsCoeff(p)) {
      for (size_t i = 0; i < prod.size; i++) {
        prod.arr[i].exp = q->arr[i].exp;
        prod.arr[i].p = PolyMul(p, &(q->arr[i].p));
      }
    }
    else if (PolyIsCoeff(q)) {
      for (size_t i = 0; i < prod.size; i++) {
        prod.arr[i].exp = p->arr[i].exp;
        prod.arr[i].p = PolyMul(q, &(p->arr[i].p));
      }
    }
    else {
      for (size_t i = 0; i < p->size; i++)
        for (size_t j = 0; j < q->size; j++) {
          int index = (int)(i * q->size + j);
          prod.arr[index].exp = p->arr[i].exp + q->arr[j].exp;
          prod.arr[index].p = PolyMul(&(p->arr[i].p), &(q->arr[j].p));
        }
    }

    // uporządkowanie zwracanego wielomianu
    prod = PolyCorrect(&prod);
  }

  return prod;
}

Poly PolyNeg(const Poly *p) {
  Poly minus_one = PolyFromCoeff(-1);
  Poly result = PolyMul(p, &minus_one);
  PolyDestroy(&minus_one);
  return result;
}

Poly PolySub(const Poly *p, const Poly *q) {
  Poly neg_q = PolyNeg(q);
  Poly result = PolyAdd(p, &neg_q);
  PolyDestroy(&neg_q);
  return result;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned long long var_idx) {
  if (PolyIsZero(p)) {
    return -1;
  }
  else if (PolyIsCoeff(p)) {
    return 0;
  }
  else if (var_idx == 0) {
    return p->arr[p->size - 1].exp;
  }
  else {
    poly_exp_t deg = -1;
    for (size_t i = 0; i < p->size; i++) {
      poly_exp_t deg_of_mono = PolyDegBy(&(p->arr[i].p), var_idx - 1);
      if (deg_of_mono > deg)
        deg = deg_of_mono;
    }
    return deg;
  }
}

poly_exp_t PolyDeg(const Poly *p) {
  if (PolyIsZero(p)) {
    return -1;
  }
  else if (PolyIsCoeff(p)) {
    return 0;
  }
  else {
    poly_exp_t deg = -1;
    for (size_t i = 0; i < p->size; i++) {
      poly_exp_t deg_of_mono = PolyDeg(&(p->arr[i].p)) + p->arr[i].exp;
      if (deg_of_mono > deg)
        deg = deg_of_mono;
    }
    return deg;
  }
}

bool PolyIsEq(const Poly *p, const Poly *q) {
  if ((PolyIsCoeff(p) && !PolyIsCoeff(q)) ||
      (!PolyIsCoeff(p) && PolyIsCoeff(q))) {
    return false;
  }
  else if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    return p->coeff == q->coeff;
  }
  else {
    if (p->size != q->size)
      return false;
    for (size_t i = 0; i < p->size; i++)
      if (!MonoIsEq(&(p->arr[i]), &(q->arr[i])))
        return false;
    return true;
  }
}

bool MonoIsEq(const Mono *m, const Mono *n) {
  return m->exp == n->exp && PolyIsEq(&(m->p), &(n->p));
}

/**
 * Funkcja zwracająca wartość współczynnika podniesionego do pewnej potęgi.
 * @param[in] base : współczynnik
 * @param[in] exp : wykładnik
 * @return @f$base^{exp}@f$
 */
static poly_coeff_t power(poly_coeff_t base, poly_exp_t exp) {
  assert(exp >= 0);
  if (exp == 0)
    return 1;
  else if (exp % 2 == 0)
    return power(base * base, exp / 2);
  else
    return base * power(base, exp - 1);
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
  Poly multiplied;

  if (PolyIsCoeff(p)) {
    multiplied = PolyClone(p);
  }
  else {
    multiplied = PolyZero();

    for (size_t i = 0; i < p->size; i++) {
      Poly multiplier = PolyFromCoeff(power(x, p->arr[i].exp));
      Poly mono_multiplied = PolyMul(&(p->arr[i].p), &multiplier);
      multiplied = PolyAddAndClean(&multiplied, &mono_multiplied);
      PolyDestroy(&multiplier);

      multiplied = PolyCorrect(&multiplied);
    }
  }

  return multiplied;
}

void MonoPrint(const Mono *m) {
  safePrintChar('(');
  PolyPrint(&(m->p));
  safePrintChar(',');
  safePrintInt(m->exp);
  safePrintChar(')');
}

void PolyPrint(const Poly *p) {
  if (PolyIsCoeff(p)) {
    safePrintLong(p->coeff);
  }
  else {
    for (size_t i = 0; i < p->size - 1; i++) {
      MonoPrint(&(p->arr[i]));
      safePrintChar('+');
    }
    MonoPrint(&(p->arr[p->size - 1]));
  }
}

/**
 * Funkcja podnosząca wielomian do potęgi.
 * @param[in] p : wielomian
 * @param[in] exp : wykładnik
 * @return @f$p^{exp}@f$
 */
static Poly PolyPower(const Poly *p, poly_exp_t exp) {
  assert(exp >= 0);
  if (exp == 0) {
    return PolyFromCoeff(1);
  }
  else if (exp % 2 == 0) {
    Poly p_2 = PolyMul(p, p);
    Poly result = PolyPower(&p_2, exp / 2);
    PolyDestroy(&p_2);
    return result;
  }
  else {
    Poly q = PolyPower(p, exp - 1);
    Poly result = PolyMul(p, &q);
    PolyDestroy(&q);
    return result;
  }
}

/**
 * Wylicza złożenie wielomianu @p p.
 * W miejsce zmiennej @f$x_i@f$ podstawia wielomian @p q[i + index].
 * Jeśli @f$i + index >= k@f$, w miejsce @f$x_i@f$ podstawia @f$0@f$.
 * @param[in] p : wielomian
 * @param[in] k : rozmiar tablicy @p q
 * @param[in] q : tablica wielomianów do podstawienia w miejsce zmiennych $p p
 * @param[in] index : indeks wielomianu w @p q do podstawienia za @f$x_0@f$
 * @return @f$p(q_0, q_1, \ldots, q_{k-1})@f$
 */
static Poly PolyComposeRec(const Poly *p, size_t k, const Poly q[], int index) {
  if (PolyIsCoeff(p)) {
    return PolyFromCoeff(p->coeff);
  }
  else {
    Poly result = PolyZero();
    for (size_t i = 0; i < p->size; i++) {
      Poly w_1 = PolyComposeRec(&((p->arr[i]).p), k, q, index + 1);
      Poly base = (index >= (int)k ? PolyZero() : PolyClone(&(q[index])));
      Poly w_2 = PolyPower(&base, p->arr[i].exp);
      Poly w = PolyMul(&w_1, &w_2);
      PolyDestroy(&w_1);
      PolyDestroy(&base);
      PolyDestroy(&w_2);
      result = PolyAddAndClean(&result, &w);
    }
    return PolyCorrect(&result);
  }
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
  return PolyComposeRec(p, k, q, 0);
}