# Polynomials

My solution for the second project of the [Individual programming project (pol. Indywidualny projekt programistyczny)](https://usosweb.mimuw.edu.pl/kontroler.php?_action=katalog2/przedmioty/pokazPrzedmiot&prz_kod=1000-222bIPP) course taken in the 2021/2022 summer semester.

## Task

Write a library of operations on rare polynomials of many variables with integer coefficients and implement calculator working on these polynomials that uses Reverse Polish notation.

# Shortened description in polish

## Treść zadania

Zadanie polega na zaimplementowaniu operacji na wielomianach
rzadkich wielu zmiennych o współczynnikach całkowitych oraz zaimplementowaniu
kalkulatora działającego na tych wielomianach i stosującego odwrotną notację
polską.

### Definicja wielomianu w zadaniu

Zmienne wielomianu oznaczamy $x_0, x_1, x_2$ itd. Definicja wielomianu jest
rekurencyjna. Wielomian jest sumą jednomianów postaci $px^n_0$, gdzie
$n$ jest wykładnikiem tego jednomianu będącym nieujemną liczbą całkowitą,
a $p$ jest współczynnikiem, który jest wielomianem. Współczynnik w
jednomianie zmiennej $x_i$ jest sumą jednomianów zmiennej $x_{i+1}$.
Rekurencja kończy się, gdy współczynnik jest liczbą (czyli wielomianem stałym),
a nie sumą kolejnych jednomianów. Wielomiany są rzadkie, co oznacza, że stopień
wielomianu może być znacznie większy niż liczba składowych jednomianów.
Przykłady wielomianów znajdują się niżej, w opisie działania kalkulatora.

## Opis programu

### Biblioteka podstawowych operacji na wielomianach

Program zawiera bibliotekę udostępniającą podstawowe operacje na wielomianach
rzadkich wielu zmiennych, której interfejs jest zdefiniowany w pliku [**poly.h**](https://github.com/patjed41/IPP-2-Polynomials/blob/main/src/poly.h).
Biblioteka udostępnia również struktury `Poly` i `Mono`, reprezentujące odpowiednio
wielomian i jednomian.

### Kalkulator działający na wielomianach

#### Użycie

Kalkulatora można używać uruchumiając plik wykonywalny `poly` powstający przy
kompilacji programu.

#### Działanie

Program kalkulatora czyta dane wierszami ze standardowego wejścia. Wiersz
zawiera wielomian lub polecenie do wykonania. Wielomian reprezentujemy jako
stałą lub sumę jednomianów. Stała jest liczbą całkowitą. Jednomian
reprezentujemy jako parę `(coeff,exp)`, gdzie współczynnik `coeff` jest
wielomianem, a wykładnik `exp` jest liczbą nieujemną. Do wyrażenia sumy używamy
znaku `+`. Jeśli wiersz zawiera wielomian, to program wstawia go na stos.
Przykłady wielomianów:
- 3
- (2,3)
- (1,4)+(2,7)
- ((2,2),15)+(-7,8)
- (((4,3)+(5,4),2)+((1,5),0),1).

Powyższe wielomiany to kolejno:
- 3
- $2x_0^3$
- $x_0^4+2x_0^7$
- $2x_1^2x_0^{15}-7x_0^8$
- $((4x_2^3+5x_2^4)x_1^2 + x_2^5x_1^0)x_0 = 4x_2^3x_1^2x_0 + 5x_2^4x_1^2x_0 + x_2^5x_0$.

Wiersz nie zawiera wielomianu, czyli zawiera polecenie, gdy rozpoczyna się
literą alfabetu angielskiego. Kalkulator wykonuje polecenia:
- `ZERO` – wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru;
- `IS_COEFF` – sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem;
- `IS_ZERO` – sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru;
- `CLONE` – wstawia na stos kopię wielomianu z wierzchołka;
- `ADD` – dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę;
- `MUL` – mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn;
- `NEG` – neguje wielomian na wierzchołku stosu;
- `SUB` – odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę;
- `IS_EQ` – sprawdza, czy dwa wielomiany na wierzchu stosu są równe
- `DEG` – wypisuje na standardowe wyjście stopień wielomianu;
- `DEG_BY idx` – wypisuje na standardowe wyjście stopień wielomianu ze względu nanzmienną o numerze `idx`;
- `AT x` – wylicza wartość wielomianu w punkcie `x`, usuwa wielomian z wierzchołkami wstawia na stos wynik operacji;
- `PRINT` – wypisuje na standardowe wyjście wielomian z wierzchołka stosu w najprostszej postaci;
- `POP` – usuwa wielomian z wierzchołka stosu;
- `COMPOSE k` – zdejmuje z wierzchołka stosu najpierw wielomian $p$, a potem kolejno wielomiany $q_{k-1}, q_{k-2}, \ldots, q_{0}$ i umieszcza na stosie wynik operacji złożenia $p(q_0, q_1, \ldots, q_{k-1})$.

Polecenia sprawdzające wypisują na standardowe wyjście 1 w przypadku prawdy
oraz 0 w przypadku fałszu.

Program kalkulatora wypisuje komunikaty o błędach na standardowe wyjście błędów.

#### Biblioteka stosu wielomianów

Implementacja kalkulatora korzysta z interfejsu biblioteki [**poly_stack.h**](https://github.com/patjed41/IPP-2-Polynomials/blob/main/src/poly_stack.h) - udostępniającej strukturę stosu wielomianów rzadkich wielu zmiennych wraz z operacjami na tym stosie.
