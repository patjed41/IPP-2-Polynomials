/** @file
  Moduł udostępniający kilka "bezpiecznych" funkcji.

  "Bezpieczne" funkcje, to funkcje, które kończą program kodem wyjścia 1, jeśli
  ich wywołanie zakończy się niepowodzeniem.

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#ifndef POLYNOMIALS_SAFE_FUNCTIONS_H
#define POLYNOMIALS_SAFE_FUNCTIONS_H

/**
 * Funkcja malloc kończąca program kodem wyjścia 1 w przypadku niepowodzenia
 * przydzielania pamięci.
 * @param[in] size : oczekiwany rozmiar pamięci
 * @return wskaźnik
 * */
void *safeMalloc(size_t size);

/**
 * Funkcja realloc kończąca program kodem wyjścia 1 w przypadku niepowodzenia
 * przydzielania pamięci.
 * @param[in] memblock : miejsce przydzielanej pamięci
 * @param[in] size : oczekiwany rozmiar pamięci
 * @return wskaźnik
 * */
void *safeRealloc(void *memblock, size_t size);

/**
 * Funkcja wypisująca na stderr komunikaty o błędach w obsłudze kalkulatora.
 * @param[in] line_number : numer wiersza, w którym wystąpił błąd
 * @param[in] error_type : rodzaj błędu
 * */
void safePrintError(int line_number, const char *error_type);

/**
* Funkcja wypisująca na standardowe wyjście liczbę typu int.
* @param[in] n : liczba typu int
* */
void safePrintInt(int n);

/**
* Funkcja wypisująca na standardowe wyjście liczbę typu long.
* @param[in] n : liczba typu long
* */
void safePrintLong(long n);

/**
* Funkcja wypisująca na standardowe wyjście char.
* @param[in] ch : znak
* */
void safePrintChar(char ch);

/**
* Funkcja wypisująca na standardowe wyjście słowo.
* @param[in] str : słowo
* */
void safePrintString(char *str);

#endif //POLYNOMIALS_SAFE_FUNCTIONS_H
