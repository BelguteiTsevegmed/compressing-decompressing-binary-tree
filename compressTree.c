/**
 * Program do kompresji drzewa binarnego do skierowanego,
 * acyklicznego grafu i dekompresji takiego grafu do pierwotnego drzewa.
 *
 * Program:
 * - Wywołany z argumentem "c" czyta z wejścia reprezentację drzewa i pisze na
 * wyjście reprezentację grafu będącego wynikiem kompresji tego drzewa.
 * - Wywołany z argumentem d czyta z wejścia reprezentację grafu będącego
 * wynikiem kompresji drzewa i pisze na wyjście reprezentację drzewa,
 * z kompresji którego ten graf powstał.
 *
 * autor: Belgutei Tsevegmed
 * data: 24 stycznia 2023 r.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struktury danych */
typedef struct drzewo {
  int val;
  struct drzewo *lsyn;
  struct drzewo *psyn;
} drzewo;

/* Korzystam z tablicy, by zapamietywac elementy drzewa. Bedzie to tablica
 * wskaznikow do elementow drzewa. */
drzewo **tablica = NULL;
int count = 0;

/**
 * Funkcja do czytania drzewa
 */
drzewo *czytaj_drzewo() {
  int val;
  scanf("%d", &val);
  if (val == 0)
    return NULL;
  drzewo *korzen = (drzewo *)malloc(sizeof(drzewo));
  korzen->val = val;
  korzen->lsyn = czytaj_drzewo();
  korzen->psyn = czytaj_drzewo();
  return korzen;
}

/**
 * Funkcja do czytania grafu
 */
drzewo *czytaj_dag() {
  int val;
  scanf("%d", &val);
  if (val == 0)
    return NULL;
  drzewo *korzen = (drzewo *)malloc(sizeof(drzewo));
  korzen->val = val;
  if (val < 0) {
    korzen->lsyn = NULL;
    korzen->psyn = NULL;
  } else {
    korzen->lsyn = czytaj_dag();
    korzen->psyn = czytaj_dag();
  }
  return korzen;
}

/**
 * Funkcja do drukowania drzewa
 */
void drukuj_drzewo(drzewo *korzen) {
  if (korzen == NULL) {
    printf("0\n");
    return;
  }
  printf("%d\n", korzen->val);
  drukuj_drzewo(korzen->lsyn);
  drukuj_drzewo(korzen->psyn);
}

/**
 * Funkcja do drukowania grafu
 */
void drukuj_dag(drzewo *korzen) {
  if (korzen == NULL) {
    printf("0\n");
    return;
  }
  printf("%d\n", korzen->val);
  if (korzen->val > 0) {
    drukuj_dag(korzen->lsyn);
    drukuj_dag(korzen->psyn);
  }
}

/**
 * Funkcja pomocnicza dla funkcji kompresji,
 * słuzy do sprawdzania czy dwa drzewa sa izomorficzne.
 */
bool izo(drzewo *d1, drzewo *d2) {
  if (d1 == d2) {
    return true;
  } else if (d1 == NULL || d2 == NULL) {
    return false;
  } else {
    return (d1->val == d2->val && izo(d1->lsyn, d2->lsyn) &&
            izo(d1->psyn, d2->psyn));
  }
}

/**
 * Funkcja do kompresji drzewa
 */
drzewo *kompresja(drzewo *korzen) {
  if (korzen == NULL)
    return NULL;
  drzewo *dag_korzen = NULL;
  int indeks = -1;
  /* Sprawdza czy drzewo aktualne nie występuje juz w tablicy drzew */
  for (int i = 0; i < count; i++) {
    if (izo(korzen, tablica[i])) {
      indeks = i;
      break;
    }
  }
  /* Jesli aktualne drzewo nie wystepuje w tablicy, to tworzę nowe drzewo */
  if (indeks == -1) {
    dag_korzen = (drzewo *)malloc(sizeof(drzewo));
    dag_korzen->val = korzen->val;
    /* Zwieksza rozmiar tablicy i dodaje nowe drzewo */
    tablica =
        (drzewo **)realloc(tablica, sizeof(drzewo *) * (size_t)(count + 1));
    tablica[count++] = korzen;
    dag_korzen->lsyn = kompresja(korzen->lsyn);
    dag_korzen->psyn = kompresja(korzen->psyn);
  } else {
    /* Jeśli drzewo wystepuje w tablicy, to dodaje nowe drzewo z val
     * rownym -(indeks+1), indeks to indeks w tablicy */
    dag_korzen = (drzewo *)malloc(sizeof(drzewo));
    dag_korzen->val = -(indeks + 1);
    dag_korzen->lsyn = NULL;
    dag_korzen->psyn = NULL;
  }
  return dag_korzen;
}

/**
 * Funkcja pomocnicza dla funkcji dekompresji.
 *
 * Jeśli w grafie wystepuje ujemna wartosc, to funkcja ta uzupelnia to miejsce
 * kopiujac do tego miejsca odpowiednie elementy oryginalnego drzewa.
 */
drzewo *uzupelnij_miejsce(drzewo *oryg_korzen) {
  if (oryg_korzen == NULL) {
    return NULL;
  }
  drzewo *nowy = malloc(sizeof(drzewo));
  nowy->val = oryg_korzen->val;
  nowy->lsyn = uzupelnij_miejsce(oryg_korzen->lsyn);
  nowy->psyn = uzupelnij_miejsce(oryg_korzen->psyn);
  return nowy;
}

/**
 * Funkcja do dekompresji grafu
 *
 * Funkcja ta zapamietuje elementy, w ktorych juz była w tablicy, a gdy
 * napotka ujemna wartosc, wyszukuje w tablicy odpowiedniego elementu
 * i kopiuje w to miejsce.
 */
drzewo *dekompresja(drzewo *korzen, drzewo *oryg_korzen) {
  if (korzen == NULL)
    return NULL;
  if (korzen->val < 0) {
    return uzupelnij_miejsce(tablica[-(korzen->val) - 1]);
  } else {
    drzewo *drzewo_korzen = (drzewo *)malloc(sizeof(drzewo));
    drzewo_korzen->val = korzen->val;
    /* Zwieksza rozmiar tablicy i dodaje nowe drzewo */
    tablica =
        (drzewo **)realloc(tablica, sizeof(drzewo *) * (size_t)(count + 1));
    tablica[count++] = drzewo_korzen;
    drzewo_korzen->lsyn = dekompresja(korzen->lsyn, oryg_korzen);
    drzewo_korzen->psyn = dekompresja(korzen->psyn, oryg_korzen);
    return drzewo_korzen;
  }
}

/**
 * Zwalnia pamiec zajęta przez drzewo lub graf
 */
void zwolnij_pamiec(drzewo *root) {
  if (root == NULL) {
    return;
  }
  zwolnij_pamiec(root->lsyn);
  zwolnij_pamiec(root->psyn);
  free(root);
}

/**
 * Uruchamia program
 */
int main(int argc, char *argv[]) {
  if (argc < 2)
    return 0;
  char arg = argv[1][0];
  if (arg == 'c') {
    drzewo *tree_korzen = czytaj_drzewo();
    drzewo *dag_korzen = kompresja(tree_korzen);
    drukuj_dag(dag_korzen);
    zwolnij_pamiec(tree_korzen);
    zwolnij_pamiec(dag_korzen);
    free(tablica);
  } else if (arg == 'd') {
    drzewo *dag_korzen = czytaj_dag();
    drzewo *tree_korzen = dekompresja(dag_korzen, dag_korzen);
    drukuj_drzewo(tree_korzen);
    zwolnij_pamiec(dag_korzen);
    zwolnij_pamiec(tree_korzen);
    free(tablica);
  }
  return 0;
}
