/** @file
  Kalkulator działający na wielomianach i stosujący odwrotną notację polską

  @authors Patryk Jędrzejczak <pj429285@students.mimuw.edu.pl>
  @date 2021
*/

// Poniższa dyrektywa zapewnia działanie funkcji getline.
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "poly.h"
#include "input.h"
#include "poly_stack.h"
#include "safe_functions.h"

/**
 * Funkcja main kalkulatora.
 * @return 0
 */
int main() {
  // Zmienne potrzebne do wywoływania funkcji getline:
  char *line_buffer = NULL;      // bufor
  size_t line_buffer_size = 0;   // rozmiar bufora
  int line_size = 0;             // długość wczytanego wiersza

  int current_line = 0;  // numer ostatnio wczytanego wiersza

  PolyStack poly_stack = PolyStackNew();

  while ((line_size = getline(&line_buffer, &line_buffer_size, stdin)) != -1) {
    current_line++;

    // Wiersz jest komentarzem.
    if (line_size > 0 && line_buffer[0] == '#')
      continue;

    // Wiersz jest pusty.
    if (line_size == 0 || (line_size == 1 && line_buffer[0] == '\n'))
      continue;

    // W wierszu wystąpił znak '\0'.
    if ((int)strlen(line_buffer) < line_size) {
      if (isLetter(line_buffer[0]))
        safePrintError(current_line, "WRONG COMMAND");
      else
        safePrintError(current_line, "WRONG POLY");
      continue;
    }

    int line_length = strlen(line_buffer); // długość wczytanego wiersza
    char line[line_length + 1];
    strcpy(line, line_buffer);
    if (line[line_length - 1] == '\n' || line[line_length - 1] == EOF)
      line[line_length - 1] = '\0';

    if (isLetter(line[0])) { // Wiersz jest poleceniem.
      if (!strcmp(line, "ZERO")) {
        PolyStackZero(&poly_stack);
      }
      else if (!strcmp(line, "IS_COEFF")) {
        if (!PolyStackIsCoeff(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "IS_ZERO")) {
        if (!PolyStackIsZero(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "CLONE")) {
        if (!PolyStackClone(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "ADD")) {
        if (!PolyStackAdd(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "MUL")) {
        if (!PolyStackMul(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "NEG")) {
        if (!PolyStackNeg(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "SUB")) {
        if (!PolyStackSub(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "IS_EQ")) {
        if (!PolyStackIsEq(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "DEG")) {
        if (!PolyStackDeg(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strncmp("DEG_BY", line, 6)) {
        if (strlen(line) == 6)
          safePrintError(current_line, "DEG BY WRONG VARIABLE");
        else if (!isspace(line[6]))
          safePrintError(current_line, "WRONG COMMAND");
        else if (line[6] != ' ' || !isULL(line + 7))
          safePrintError(current_line, "DEG BY WRONG VARIABLE");
        else if (!PolyStackDegBy(&poly_stack, strtoull(line + 6, NULL, 10)))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strncmp("AT", line, 2)) {
        if (strlen(line) == 2)
          safePrintError(current_line, "AT WRONG VALUE");
        else if (!isspace(line[2]))
          safePrintError(current_line, "WRONG COMMAND");
        else if (line[2] != ' ' || !isLL(line + 3))
          safePrintError(current_line, "AT WRONG VALUE");
        else if (!PolyStackAt(&poly_stack, strtoll(line + 3, NULL, 10)))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "PRINT")) {
        if (!PolyStackPrint(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strcmp(line, "POP")) {
        if (!PolyStackPop(&poly_stack))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else if (!strncmp("COMPOSE", line, 7)) {
        if (strlen(line) == 7)
          safePrintError(current_line, "COMPOSE WRONG PARAMETER");
        else if (!isspace(line[7]))
          safePrintError(current_line, "WRONG COMMAND");
        else if (line[7] != ' ' || !isULL(line + 8))
          safePrintError(current_line, "COMPOSE WRONG PARAMETER");
        else if (!PolyStackCompose(&poly_stack, strtoll(line + 8, NULL, 10)))
          safePrintError(current_line, "STACK UNDERFLOW");
      }
      else {
        safePrintError(current_line, "WRONG COMMAND");
      }
    }
    else { // Wiersz jest wielomianem.
      bool wrong_poly = false;
      Poly p = readPoly(line, &wrong_poly);
      if (wrong_poly) {
        safePrintError(current_line, "WRONG POLY");
        PolyDestroy(&p);
      }
      else {
        PolyStackPush(&poly_stack, p);
      }
    }

    errno = 0;
  }

  // Sprawdzenie, czy ostatnie wywołanie funkcji getline zakończyło się błędem.
  if (errno != 0)
    exit(1);

  PolyStackDestroy(&poly_stack);
  free(line_buffer);

  return 0;
}