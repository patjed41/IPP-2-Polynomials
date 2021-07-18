/** @file
  Implementacja modułu udostępniającego funkcje pomocne przy wczytywaniu danych
  przez kalkulator

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "input.h"

/** maksymalna wartość typu int */
#define MAX_INT 2147483647

bool isLetter(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

/**
 * Sprawdza, czy znak jest cyfrą.
 * @param[in] ch : znak
 * @return czy ch jest cyfrą?
 */
static bool isDigit(char ch) {
  return ch >= '0' && ch <= '9';
}

bool isULL(const char *str) {
  if (strlen(str) == 0 || !isDigit(str[0]))
    return false;

  errno = 0;
  char *end_ptr;
  strtoull(str, &end_ptr, 10);

  return *end_ptr == '\0' && errno == 0;
}

bool isLL(const char *str) {
  if (strlen(str) == 0 || (!isDigit(str[0]) && str[0] != '-'))
    return false;

  errno = 0;
  char *end_ptr;
  strtoll(str, &end_ptr, 10);

  return *end_ptr == '\0' && errno == 0;
}

/**
 * Sprawdza, czy słowo jest dodatnią liczbą z zakresu int.
 * @param[in] str : słowo
 * @return czy słowo jest intem?
 */
static bool isUInt(const char *str) {
  if (strlen(str) == 0 || !isDigit(str[0]))
    return false;

  errno = 0;
  char *end_ptr;
  long value = strtol(str, &end_ptr, 10);

  return *end_ptr == '\0' && errno == 0 && value <= (long)MAX_INT;
}

/**
 * Liczy z ilu jednomianów składa się wielomian reprezentowany przez słowo
 * @p str.
 * @param[in] str : słowo-wielomian
 * @return liczba jednomianów
 */
static int countMonos(char str[]) {
  int count = 1; // numer aktualnego jednomianu
  int brackets = 0; // brackets == lewe nawiasy - prawe nawiasy
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == '(')
      brackets++;
    else if (str[i] == ')')
      brackets--;
    else if (str[i] == '+' && brackets == 0)
      count++;
  }
  return count;
}

/**
 * Wypełnia początek tablicy pos pozycjami, na których rozpoczynają się
 * kolejne jednomiany w wielomianie reprezentowanym przez @p str. Należy zadbać
 * o to, aby tablica @p pos miała wystarczający rozmiar.
 * @param[in] str : słowo-wielomian
 * @param[in] pos : tablica pozycji do wypełnienia
 */
static void positionsOfMonos(char str[], int pos[]) {
  assert(strlen(str) > 0 && str[0] == '(');
  pos[0] = 0;
  int count = 1; // numer aktualnego jednomianu
  int brackets = 1; // brackets == lewe nawiasy - prawe nawiasy
  for (size_t i = 1; i < strlen(str); i++) {
    if (str[i] == '(') {
      brackets++;
    }
    else if (str[i] == ')'){
      brackets--;
    }
    else if (str[i] == '+' && brackets == 0) {
      pos[count] = i + 1;
      count++;
    }
  }
}

/**
 * Kopiuje fragment słowa @p str od pozycji @p l do pozycji @p r do tablicy
 * @p arr. Należy zadbać o to, aby tablica @p arr miała wystarczający rozmiar.
 * @param[in] str : słowo
 * @param[in] arr : tablica, do której funkcja kopiuje
 * @param[in] l : pozycja początku kopiowanego fragmentu
 * @param[in] r : pozycja końca kopiowanego fragmentu
 */
static void copyFragment(char arr[], char str[], int l, int r) {
  for (int i = 0; i < r - l + 1; i++)
    arr[i] = str[l + i];
  arr[r - l + 1] = '\0';
}

/**
 * Lokalizuje przecinek w jednomianie reprezentowanym przez @p str. Zwraca jego
 * pozycję. Jeśli jednomian jest niepoprawny i przecinek nie istnieje, zwraca 0.
 * Jeśli jednomian jest niepoprawny i ma kilka przecinków, zwraca pozycję
 * pierwszego.
 * @param[in] str : słowo-jednomian
 * @return pozycja przecinka
 */
static int comaPosition(char str[]) {
  int brackets = 0; // brackets == lewe nawiasy - prawe nawiasy
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == '(')
      brackets++;
    else if (str[i] == ')')
      brackets--;
    else if (str[i] == ',' && brackets == 1)
      return i;
  }
  return 0;
}

/**
 * Wczytuje jednomian. Informuje, jeśli wczytywany wielomian jest błędny,
 * ustawiając *wrong_poly na true.
 * @param[in] str : słowo reprezentujące jednomian
 * @param[in] wrong_poly : wskaźnik na zmienną informującą o błędnym jednomianie
 * @return wczytany jednomian
 */
static Mono readMono(char str[], bool *wrong_poly) {
  Mono m = (Mono) {.p = PolyZero(), .exp = 0};
  if (strlen(str) == 0 || str[0] != '(' || str[strlen(str) - 1] != ')') {
    (*wrong_poly) = true;
  }
  else {
    int coma_pos = comaPosition(str);
    if (coma_pos == 0) {
      (*wrong_poly) = true;
    }
    else {
      char poly_str[coma_pos];
      copyFragment(poly_str, str, 1, coma_pos - 1);
      m.p = readPoly(poly_str, wrong_poly);
      char exp_str[strlen(str) - coma_pos - 1];
      copyFragment(exp_str, str, coma_pos + 1, strlen(str) - 2);
      if (!isUInt(exp_str))
        (*wrong_poly) = true;
      else
        m.exp = strtol(exp_str, NULL, 10);
    }
  }
  return m;
}

Poly readPoly(char str[], bool *wrong_poly) {
  Poly p = PolyZero();
  if (strlen(str) == 0) {
    (*wrong_poly) = true;
  }
  else if (isDigit(str[0]) || str[0] == '-') { // wielomian jest współczynnikiem
    if (!isLL(str))
      (*wrong_poly) = true;
    else
      p = PolyFromCoeff(strtoll(str, NULL, 10));
  }
  else if (str[0] == '(') {
    int num_of_monos = countMonos(str);
    Mono monos[num_of_monos];
    int pos[num_of_monos + 1]; // pozycje, na których zaczynają się jednomiany
    positionsOfMonos(str, pos);
    pos[num_of_monos] = strlen(str) + 2;
    for (int i = 0; i < num_of_monos; i++) {
      char mono_str[pos[i + 1] - pos[i]];
      copyFragment(mono_str, str, pos[i], pos[i + 1] - 2);
      monos[i] = readMono(mono_str, wrong_poly);
    }
    p = PolyAddMonos((unsigned int)num_of_monos, monos);
  }
  else {
    (*wrong_poly) = true;
  }
  return p;
}