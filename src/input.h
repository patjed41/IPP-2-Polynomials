/** @file
  Moduł udostępniający funkcje pomocne przy wczytywaniu danych przez kalkulator

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#ifndef POLYNOMIALS_INPUT_H
#define POLYNOMIALS_INPUT_H

#include <stdbool.h>

#include "poly.h"

/**
 * Sprawdza, czy znak jest literą alfabetu angielskiego.
 * @param[in] ch : znak
 * @return czy ch jest literą?
 */
bool isLetter(char ch);

/**
 * Sprawdza, czy słowo jest liczbą z zakeresu unsigned long long.
 * @param[in] str : słowo
 * @return czy słowo jest unsigned long longiem?
 */
bool isULL(const char *str);

/**
 * Sprawdza, czy słowo jest liczbą z zakeresu long long.
 * @param[in] str : słowo
 * @return czy słowo jest long longiem?
 */
bool isLL(const char *str);

/**
 * Wczytuje wielomian. Informuje, jeśli wczytywany wielomian jest błędny,
 * ustawiając *wrong_poly na true.
 * @param[in] str : słowo reprezentujące wielomian
 * @param[in] wrong_poly : wskaźnik na zmienną informującą o błędnym wielomianie
 * @return wczytany wielomian
 */
Poly readPoly(char str[], bool *wrong_poly);

#endif //POLYNOMIALS_INPUT_H
