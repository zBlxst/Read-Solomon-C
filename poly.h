#ifndef __POLY_H__
#define __POLY_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct st_poly {
    int deg; // degre du polynome, -1 pour un polynome nul
    unsigned int* coeff; // tableau des coefficients : coeff[i] pour le coefficient de X^i
} poly;


// le polynome utilise pour generer GF(256) est : X^8 + X^4 + X^3 + X + 1
extern unsigned int Log256[]; // table de logs en base 3 (soit X+1 en notation polynomiale) dans GF(256)
extern unsigned int Alog256[]; // table des puissances de 3 dans GF(256)

unsigned int add256(unsigned int a, unsigned int b); // retourne a+b
unsigned int mul256(unsigned int a, unsigned int b); // retourne a*b
unsigned int div256(unsigned int a, unsigned int b); // retourne a/b
unsigned int inv256(unsigned int a); // retourne l'inverse de a

poly* poly_init(); // initialise un polynome : a faire avant toute operation sur le polynome
unsigned int poly_coeff(poly* a, int deg); // retourne le coeff de degre deg du polynome a
void poly_add(poly* res, poly* a, poly* b); // res = a+b
void poly_scalar_mul(poly* res, poly* a, unsigned int c); // res = c*a (c dans GF(256))
void poly_mul(poly* res, poly* a, poly* b); // res = a*b
void poly_div(poly* res, poly* a, poly* b); // res = a/b (le quotient de la division)
void poly_mod(poly* res, poly* a, poly* b); // res = a mod b (le reste de la division)
void poly_copy(poly* res, poly* a); // res = a
void poly_free(poly* a); // libere la memoire occupee par a
void poly_null(poly* a); // a = 0
void poly_add_monomial(poly* res, unsigned int coeff, int deg); // res = res + coeff * X^deg
void poly_print(poly* a); // affiche a de facon "lisible"


// calule le noyau de la matrice B de taille dim*dim et le stocke dans la matrice (*kernel) de dimension (*kernel_dim)*dim
void kernel(unsigned int** B, int dim, unsigned int*** kernel, int* kernel_dim);

void eval_horner(unsigned int *res, poly *f, unsigned int *y);
void eval_multi(unsigned int **res, poly *f , int n, unsigned int **a);
void encode1(unsigned int **res, unsigned int **m, unsigned int **a, int k, int n);
void lagrange_one(poly *res, int k, unsigned int **a, int i);
void lagrange(poly **res, int k, unsigned int **a);
void interpol(poly *res, int k, unsigned int **a, unsigned int **m);
void encode2(unsigned int **res, unsigned int **m, unsigned int **a, int k, int n);
int check_rs(unsigned int **c, unsigned int **a, int k, int n);
void euclide_tr(poly *res, poly *u, poly *v, poly *a, poly *b, int d);
void precomp(poly *res, unsigned int **a, int n);
int gao_decode(unsigned int **f , unsigned int **ct, unsigned int **a, int k, int n); // J'ai ajouté une sortie, 0 si il n'y a pas eu d'erreur, 1 si il y en a eu une.
void random_error(unsigned int **e, int d, int n);
void send_with_err(unsigned int **ct, unsigned int **m, unsigned int **a, int d, int k, int n);


#endif
