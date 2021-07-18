/** @file
  Moduł udostępniający stos wielomianów wraz z kilkoma operacjami

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#ifndef POLYNOMIALS_POLY_STACK_H
#define POLYNOMIALS_POLY_STACK_H

#include "poly.h"

/**
 * To jest struktura reprezentująca stos wielomianów.
 */
typedef struct PolyStack {
  Poly *polys; ///< dynamiczna tablica reprezentująca stos wielomianów
  size_t polys_size; ///< rozmiar tablicy @p polys
  unsigned int num_of_polys; ///< liczba wielomianów na stosie
} PolyStack;

/**
 * Tworzy nowy, pusty stos wielomianów.
 */
PolyStack PolyStackNew();

/**
 * Usuwa z pamięci stos wielomianów.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 */
void PolyStackDestroy(PolyStack *poly_stack);

/**
 * Wkłada na szczyt stosu wielomianów nowy wielomian.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @param[in] p : wielomian
 */
void PolyStackPush(PolyStack *poly_stack, Poly p);

/**
 * Wkłada na szczyt stosu wielomianów wielomian zerowy.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 */
void PolyStackZero(PolyStack *poly_stack);

/**
 * Sprawdza, czy wielomian na szczycie stosu jest współczynnikiem. Jeśli jest,
 * wypisuje 1 na standardowe wyjście, a jeśli nie jest, wypisuje 0. Jeśli
 * stos jest pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackIsCoeff(const PolyStack *poly_stack);

/**
 * Sprawdza, czy wielomian na szczycie stosu jest równy 0. Jeśli jest,
 * wypisuje 1 na standardowe wyjście, a jeśli nie jest, wypisuje 0. Jeśli
 * stos jest pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackIsZero(const PolyStack *poly_stack);

/**
 * Wstawia na stos kopię wielomianu będącego na szczycie. Jeśli stos jest
 * pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackClone(PolyStack *poly_stack);

/**
 * Dodaje dwa wielomiany znajdujące się na szczycie stosu, zdejmuje je ze stosu
 * i wstawia na szczyt ich sumę. Jeśli na stosie nie ma przynajmniej 2
 * wielomianów i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy na stosie są przynajmniej 2 wielomiany?
 */
bool PolyStackAdd(PolyStack *poly_stack);

/**
 * Mnoży dwa wielomiany znajdujące się na szczycie stosu, zdejmuje je ze stosu
 * i wstawia na szczyt ich iloczyn. Jeśli na stosie nie ma przynajmniej 2
 * wielomianów i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy na stosie są przynajmniej 2 wielomiany?
 */
bool PolyStackMul(PolyStack *poly_stack);

/**
 * Neguje wielomian na znajdujący się na szczycie stosu. Jeśli stos jest
 * pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackNeg(PolyStack *poly_stack);

/**
 * Odejmuje od wielomianu na szczycie stosu, wielomian znajdujący się pod nim,
 * zdejmuje oba wielomiany ze stosu i wstawia na szczyt stosu ich różnicę.
 * Jeśli na stosie nie ma przynajmniej 2 wielomianów i nie da się wykonać
 * operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy na stosie są przynajmniej 2 wielomiany?
 */
bool PolyStackSub(PolyStack *poly_stack);

/**
 * Sprawdza czy dwa wielomiany znajdujące się na szczycie stosu są równe.
 * Jeśli są, wypisuje na standardowe wyjście 1, a jeśli nie są, wypisuje 0.
 * Jeśli na stosie nie ma przynajmniej 2 wielomianów i nie da się wykonać
 * operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy na stosie są przynajmniej 2 wielomiany?
 */
bool PolyStackIsEq(PolyStack *poly_stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu na szczycie stosu. Jeśli
 * stos jest pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackDeg(const PolyStack *poly_stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu na szczycie stosu
 * ze względu na zmienną o numerze @p idx. Jeśli stos jest pusty i nie da się
 * wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @param[in] idx : numer zmiennej
 * @return czy stos nie jest pusty?
 */
bool PolyStackDegBy(const PolyStack *poly_stack, unsigned long long idx);

/**
 * Wylicza wartość wielomianu na szczycie stosu w punkcie @p x, zdejmuje go ze
 * stosu i wstawia na stos wynik operacji. Jeśli stos jest pusty i nie da się
 * wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @param[in] x : punkt
 * @return czy stos nie jest pusty?
 */
bool PolyStackAt(PolyStack *poly_stack, long long x);

/**
 * Wypisuje na standardowe wyjście wielomian ze szczytu stosu. Jeśli stos jest
 * pusty i nie da się wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackPrint(const PolyStack *poly_stack);

/**
 * Usuwa wielomian ze szczytu stosu. Jeśli stos jest pusty i nie da się wykonać
 * operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @return czy stos nie jest pusty?
 */
bool PolyStackPop(PolyStack *poly_stack);

/**
 * Wylicza złożenie @f$p(q_0, q_1, \ldots, q_{k-1})@f$, gdzie @f$p@f$ jest
 * wielomianem znajdującym się na szczycie stosu, a @f$q_i@f$ jest wielomianem
 * znajdującym się na stosie @f$k - i@f$ pozycji niżej niż @f$p@f$.
 * Umieszcza na stosie wynik operacji składania. Jeśli na stosie jest mniej niż
 * @f$k + 1@f$ wielomianów i nie da sie wykonać operacji, zwraca false.
 * @param[in] poly_stack : wskaźnik na stos wielomianów
 * @param[in] k : ilość wielomianów podstawianych za zmienne @f$p@f$
 * @return czy na stosie jest przynajmniej @f$k + 1@f$ wielomianów?
 */
bool PolyStackCompose(PolyStack *poly_stack, size_t k);

#endif //POLYNOMIALS_POLY_STACK_H
