#include "poly.h"

unsigned int Log256[256] = {
  0,   0,  25,   1,  50,   2,  26, 198,  75, 199,  27, 104,  51, 238, 223,   3,
100,   4, 224,  14,  52, 141, 129, 239,  76, 113,   8, 200, 248, 105,  28, 193,
125, 194,  29, 181, 249, 185,  39, 106,  77, 228, 166, 114, 154, 201,   9, 120,
101,  47, 138,   5,  33,  15, 225,  36,  18, 240, 130,  69,  53, 147, 218, 142,
150, 143, 219, 189,  54, 208, 206, 148,  19,  92, 210, 241,  64,  70, 131,  56,
102, 221, 253,  48, 191,   6, 139,  98, 179,  37, 226, 152,  34, 136, 145,  16,
126, 110,  72, 195, 163, 182,  30,  66,  58, 107,  40,  84, 250, 133,  61, 186,
 43, 121,  10,  21, 155, 159,  94, 202,  78, 212, 172, 229, 243, 115, 167,  87, 
175,  88, 168,  80, 244, 234, 214, 116,  79, 174, 233, 213, 231, 230, 173, 232,
 44, 215, 117, 122, 235,  22,  11, 245,  89, 203,  95, 176, 156, 169,  81, 160, 
127,  12, 246, 111,  23, 196,  73, 236, 216,  67,  31,  45, 164, 118, 123, 183,
204, 187,  62,  90, 251,  96, 177, 134,  59,  82, 161, 108, 170,  85,  41, 157,
151, 178, 135, 144,  97, 190, 220, 252, 188, 149, 207, 205,  55,  63,  91, 209,
 83,  57, 132,  60,  65, 162, 109,  71,  20,  42, 158,  93,  86, 242, 211, 171,
 68,  17, 146, 217,  35,  32,  46, 137, 180, 124, 184,  38, 119, 153, 227, 165,
103,  74, 237, 222, 197,  49, 254,  24,  13,  99, 140, 128, 192, 247, 112,   7,
};

unsigned int Alog256[256] = {
  1,   3,   5,  15,  17,  51,  85, 255,  26,  46, 114, 150, 161, 248,  19,  53,
 95, 225,  56,  72, 216, 115, 149, 164, 247,   2,   6,  10,  30,  34, 102, 170,
229,  52,  92, 228,  55,  89, 235,  38, 106, 190, 217, 112, 144, 171, 230,  49,
 83, 245,   4,  12,  20,  60,  68, 204,  79, 209, 104, 184, 211, 110, 178, 205,
 76, 212, 103, 169, 224,  59,  77, 215,  98, 166, 241,   8,  24,  40, 120, 136,
131, 158, 185, 208, 107, 189, 220, 127, 129, 152, 179, 206,  73, 219, 118, 154,
181, 196,  87, 249,  16,  48,  80, 240,  11,  29,  39, 105, 187, 214,  97, 163,
254,  25,  43, 125, 135, 146, 173, 236,  47, 113, 147, 174, 233,  32,  96, 160,
251,  22,  58,  78, 210, 109, 183, 194,  93, 231,  50,  86, 250,  21,  63,  65,
195,  94, 226,  61,  71, 201,  64, 192,  91, 237,  44, 116, 156, 191, 218, 117,
159, 186, 213, 100, 172, 239,  42, 126, 130, 157, 188, 223, 122, 142, 137, 128,
155, 182, 193,  88, 232,  35, 101, 175, 234,  37, 111, 177, 200,  67, 197,  84,
252,  31,  33,  99, 165, 244,   7,   9,  27,  45, 119, 153, 176, 203,  70, 202,
 69, 207,  74, 222, 121, 139, 134, 145, 168, 227,  62,  66, 198,  81, 243,  14,
 18,  54,  90, 238,  41, 123, 141, 140, 143, 138, 133, 148, 167, 242,  13,  23,
 57,  75, 221, 124, 132, 151, 162, 253,  28,  36, 108, 180, 199,  82, 246,   1,
};

unsigned int add256(unsigned int a, unsigned int b) {
    return a^b;
}

unsigned int mul256(unsigned int a, unsigned int b) {
    if ((a==0) || (b==0)) {
        return 0;
    }
    return Alog256[(Log256[a]+Log256[b])%255];
}

unsigned int div256(unsigned int a, unsigned int b) {
    if (b==0) {
        fprintf(stderr,"Division par 0 !\n");
        return 0;
    }
    if (a==0) {
        return 0;
    }
    return Alog256[(255+Log256[a]-Log256[b]) % 255];
}

unsigned int inv256(unsigned int a) {
    if (a==0) {
        fprintf(stderr,"Inversion de 0 !\n");
        return 0;
    }
    return Alog256[255-Log256[a]];
}

poly* poly_init() {
    poly* P = malloc(sizeof(poly));
    P->deg = -1;
    P->coeff = NULL;
    return P;
}

void poly_add(poly* res, poly* a, poly* b) {
    int i;
    poly* tmp;
    // a est le polynome de plus haut degre: on inverse si necessaire
    if (a->deg < b->deg) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if (a->deg == -1) {
        // somme de deux polynomes nuls
        res->deg = -1;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        return;
    }
    tmp = poly_init();
    tmp->coeff = (unsigned int*) malloc((a->deg+1)*sizeof(unsigned int));
    for (i=0; i<=b->deg; i++) {
        tmp->coeff[i] = add256(a->coeff[i], b->coeff[i]);
    }
    for (i=b->deg+1; i<=a->deg; i++) {
        tmp->coeff[i] = a->coeff[i];
    }
    i = a->deg;
    while ((i>=0) && (tmp->coeff[i] == 0)) {
        i--;
    }
    if (i < 0) {
        free(tmp->coeff);
        free(tmp);
        res->deg = -1;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        return;        
    }
    if (i != a->deg) {
        tmp->coeff = (unsigned int*) realloc(tmp->coeff, (i+1)*sizeof(unsigned int));
    }
    // on recopie le resultat dans res
    res->deg = i;
    if (res->coeff) {
        free(res->coeff);
    }
    res->coeff = tmp->coeff;
    free(tmp);
}

void poly_scalar_mul(poly* res, poly* a, unsigned int c) {
    int i;
    poly* tmp = poly_init();
    if (c != 0) {
        for (i=0; i<= a->deg; i++) {
            poly_add_monomial(tmp, mul256(c,a->coeff[i]), i); 
        }
    }
    poly_copy(res, tmp);
    poly_free(tmp);
}

void poly_mul(poly* res, poly* a, poly* b) {
    int i,j;
    poly* tmp;
    if ((a->deg == -1) || (b->deg == -1)) {
        res->deg = -1;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        return;                
    }
    tmp = poly_init();
    tmp->deg = a->deg+b->deg;
    tmp->coeff = (unsigned int*) calloc((tmp->deg+1), sizeof(unsigned int));
    
    for (i=0; i<=a->deg; i++) {
        for (j=0; j<=b->deg; j++) {
            tmp->coeff[i+j] = add256(tmp->coeff[i+j], mul256(a->coeff[i], b->coeff[j]));
        }
    }
    poly_copy(res, tmp);
    poly_free(tmp);    
}

void poly_mod(poly* res, poly* a, poly* b) {
    int i,j;
    unsigned int c;
    poly* tmp;
    if (b->deg == -1) {
        fprintf(stderr,"Division par un polynome nul !\n");
        return;
    }
    tmp = poly_init();
    tmp->coeff = (unsigned int*) malloc((a->deg+1)*sizeof(unsigned int));
    for (i=0; i<=a->deg; i++) {
        tmp->coeff[i] = a->coeff[i];
    }
    
    if (a->deg < b->deg) {
        res->deg = a->deg;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = tmp->coeff;
        free(tmp);
        return;
    }

    for (i=a->deg; i>=b->deg; i--) {
        c = div256(tmp->coeff[i], b->coeff[b->deg]);
        for (j=0; j<=b->deg; j++) {
            tmp->coeff[i-b->deg+j] = add256(tmp->coeff[i-b->deg+j], mul256(b->coeff[j],c));
        }
    }
    
    i = b->deg-1;
    while ((i>=0) && (tmp->coeff[i] == 0)) {
        i--;
    }

    if (i<0) {
        res->deg = -1;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        free(tmp->coeff);
        free(tmp);
        return;
    }
    
    res->deg = i;
    if (res->coeff) {
        free(res->coeff);
    }
    res->coeff = (unsigned int*) realloc(tmp->coeff, (i+1)*sizeof(unsigned int));
    free(tmp);
}

unsigned int poly_coeff(poly* a, int deg) {
    if ((a->deg < deg) || (deg < 0)) {
        return 0;
    }
    return a->coeff[deg];
}

void poly_div(poly* res, poly* a, poly* b) {
    int i;
    unsigned int c;
    if (b->deg == -1) {
        fprintf(stderr, "Division par le polynome nul !\n");
        return;
    }
    poly* tmp1 = poly_init();
    poly* tmp2 = poly_init();
    poly* quot = poly_init();
    
    poly_copy(tmp1, a);
    for (i=a->deg-b->deg; i>=0; i--) {
        // c est le coefficient de degre i du quotient
        c = div256(poly_coeff(tmp1, i+b->deg),b->coeff[b->deg]);
        poly_add_monomial(quot, c, i);
        poly_null(tmp2);
        poly_add_monomial(tmp2, c, i);
        poly_mul(tmp2, b, tmp2);
        poly_add(tmp1, tmp1, tmp2);
    } 
    poly_copy(res, quot);
    poly_free(tmp1);
    poly_free(tmp2);
    poly_free(quot);
}

void poly_copy(poly* res, poly* a) {
    unsigned int* tmp;
    int i;
    res->deg = a->deg;
    if (res->deg == -1) {
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        return;
    }

    tmp = (unsigned int*) malloc((a->deg+1)*sizeof(unsigned int));
    for (i=0; i<=a->deg; i++) {
        tmp[i] = a->coeff[i];
    }
    if (res->coeff) {
        free(res->coeff);
    }
    res->coeff = tmp;
}

void poly_free(poly* a) {
    if (a->coeff) {
        free(a->coeff);
    }
    free(a);
}

void poly_add_monomial(poly* res, unsigned int coeff, int deg) {
    int i;
    if (coeff == 0) {
        return;
    }
    if (res->deg < deg) {
        res->coeff = (unsigned int*) realloc(res->coeff, (deg+1)*sizeof(unsigned int));
        for (i=res->deg+1; i<deg; i++) {
            res->coeff[i] = 0;
        }
        res->coeff[deg] = coeff;
        res->deg = deg;
        return;
    }
    res->coeff[deg] = add256(res->coeff[deg],coeff);
    i = res->deg;
    while ((i>=0) && (res->coeff[i] == 0)) {
        i--;
    }
    if (i < 0) {
        res->deg = -1;
        if (res->coeff) {
            free(res->coeff);
        }
        res->coeff = NULL;
        return;        
    }
    if (i != res->deg) {
        res->coeff = (unsigned int*) realloc(res->coeff, (i+1)*sizeof(unsigned int));
    }
    res->deg = i;
}


void poly_print(poly *a) {
    int i;
    int first = 1;
    
    for (i=0; i<=a->deg; i++) {
        if (a->coeff[i] != 0) {
            if (!first) {
	printf(" + ");
            } else {
	first = 0;
            }
            if (i==0) {
	printf("%u", a->coeff[i]);
            } else if (i==1) {
	if (a->coeff[i] == 1) {
	    printf("T");
	} else {
	    printf("%uT", a->coeff[i]);
	}
            } else {
	if (a->coeff[i] == 1) {
	    printf("T^%d", i);
	} else {
	    printf("%uT^%d", a->coeff[i], i);
	}
            }
        }
    }
    if (first) {
        printf("0");
    }
    printf("\n");
}

void poly_null(poly* a) {
    a->deg = -1;
    if (a->coeff) {
        free(a->coeff);
    }
    a->coeff = NULL;
}


void kernel(unsigned int** B, int dim, unsigned int*** kernel, int* kernel_dim) {
    int i,j,k,l;
    unsigned int c;
    unsigned int* tmpui;

    // on utilise la matrice I pour trouver le noyau
    unsigned int** I = (unsigned int**) malloc(dim*sizeof(unsigned int*));
    for (i=0; i<dim; i++) {
        I[i] = (unsigned int*) calloc(dim,sizeof(unsigned int));
        I[i][i] = 1;
    }

    i=0; // indice de la ligne du pivot
    j=0; // indice de la colonne du pivot
    
    while (j<dim) {
        k=i; // k est la premiere ligne de B ayant un coefficient non-nul dans la colonne j
        while ((k<dim) && (B[k][j] == 0)) {
            k++;
        }
        if (k == dim) { // si la colonne j ne contient plus que des 0, on passe a la suivante
            j++;
            continue;
        } 
        if (k != i) { // on echange les lignes si necessaire (et aussi dans I)
            tmpui = B[i]; B[i] = B[k]; B[k] = tmpui;
            tmpui = I[i]; I[i] = I[k]; I[k] = tmpui;
        }
        for (k=i+1; k<dim; k++) {
            c = div256(B[k][j],B[i][j]);
            for (l=0; l<dim; l++) {
	B[k][l] = add256(B[k][l],mul256(B[i][l],c));
	I[k][l] = add256(I[k][l],mul256(I[i][l],c));
            }
        }
        i++; // on passe a la ligne d'apres
        j++; // on avance aussi d'une colonne, car on vient de mettre la precedente a 0
    }

    // le pivot est termin�, on recopie ce qu'il faut dans kernel et kernel_dim
    *kernel_dim = dim - i;
    *kernel = (unsigned int**) malloc((*kernel_dim) * sizeof(unsigned int*));
    for (j=0; j<(*kernel_dim); j++) {
        (*kernel)[j] = I[i+j];
    }

    // on lib�re la m�moire qui ne sert plus
    for (j=0; j<i; j++) {
        free(I[j]);
    }
    free(I);

}

void eval_horner(unsigned int *res, poly *f, unsigned int *y) {
    *res = poly_coeff(f, f->deg);

    for (int i = f->deg-1; i >= 0; i--) {
        *res = mul256(*res, *y);
        *res = add256(*res, poly_coeff(f, i));
    }
}

void eval_multi(unsigned int **res, poly *f , int n, unsigned int **a) {
    for (int i = 0; i < n; i++) {
        eval_horner(*res+i, f, *a+i);
    }
}

void encode1(unsigned int **res, unsigned int **m, unsigned int **a, int k, int n) {
    poly *pre_image = poly_init();
    for (int i = 0; i < k; i++) {
        poly_add_monomial(pre_image, (*m)[i], i);
    }
    eval_multi(res, pre_image, n, a);
}

void lagrange_one(poly *res, int k, unsigned int **a, int i) {
    poly_null(res);
    poly_add_monomial(res, 1, 0);
    for (int j = 0; j < k; j++) {
        if (j != i) {
            poly *monome = poly_init();
            poly_add_monomial(monome, 1, 1);
            poly_add_monomial(monome, (*a)[j], 0);
            poly_mul(res, res, monome);
        }
    }
    unsigned int evaluated;
    eval_horner(&evaluated, res, *a + i);
    poly_scalar_mul(res, res, inv256(evaluated)); 
}

void lagrange(poly **res, int k, unsigned int **a) {
    for (int i = 0; i < k; i++) {
        lagrange_one(res[i], k, a, i);
    }
}

void interpol(poly *res, int k, unsigned int **a, unsigned int **m) {
    poly_null(res);
    poly **lagranges = malloc(k*sizeof(poly*));
    for (int i = 0; i < k; i++) {
        lagranges[i] = poly_init();
    }


    lagrange(lagranges, k, a);

    for (int i = 0; i < k; i++) {
        poly_scalar_mul(lagranges[i], lagranges[i], (*m)[i]);
        poly_add(res, res, lagranges[i]);
    }
}

void encode2(unsigned int **res, unsigned int **m, unsigned int **a, int k, int n) {
    poly *interpoled = poly_init();
    interpol(interpoled, k, a, m);
    eval_multi(res, interpoled, n, a);
}

int check_rs(unsigned int **c, unsigned int **a, int k, int n) {
    poly *interpoled;
    interpol(interpoled, k, a, c);
    for (int i = 0; i < n; i++) {
        unsigned int res;
        eval_horner(&res, interpoled, a[i]);
        if (res != *(c[i])) {
            return 0;
        }
    }
    return 1;
}

void euclide_tr(poly *res, poly *u, poly *v, poly *a, poly *b, int d) {
    poly *ri = poly_init(); 
    poly *ri1 = poly_init();
    poly *ui = poly_init(); 
    poly *ui1 = poly_init();
    poly *vi = poly_init(); 
    poly *vi1 = poly_init();
    poly_copy(ri, a);
    poly_copy(ri1, b);
    poly_null(ui);
    poly_null(ui1);
    poly_null(vi);
    poly_null(vi1);

    poly_add_monomial(ui, 1, 0);
    poly_add_monomial(vi1, 1, 0);

    while (ri1->deg >= d) {
        poly *div = poly_init();
        poly *temp = poly_init();
        poly *tempi1 = poly_init();
        poly_div(div, ri, ri1);

        poly_copy(tempi1, ri1);
        poly_mul(temp, div, ri1);
        poly_add(ri1, ri, temp);
        poly_copy(ri, tempi1);

        poly_copy(tempi1, ui1);
        poly_mul(temp, div, ui1);
        poly_add(ui1, ui, temp);
        poly_copy(ui, tempi1);

        poly_copy(tempi1, vi1);
        poly_mul(temp, div, vi1);
        poly_add(vi1, vi, temp);
        poly_copy(vi, tempi1);
    }

    poly_copy(res, ri1);
    poly_copy(u, ui1);
    poly_copy(v, vi1);
}

void precomp(poly *res, unsigned int **a, int n) {
    poly_null(res);
    poly_add_monomial(res, 1, 0);
    for (int i = 0; i < n; i++) {
        poly *monome = poly_init();
        poly_add_monomial(monome, 1, 1);
        poly_add_monomial(monome, (*a)[i], 0);
        poly_mul(res, res, monome);
    }
}

int gao_decode(unsigned int **f , unsigned int **ct, unsigned int **a, int k, int n) {
    poly *g0 = poly_init();
    poly *g1 = poly_init();
    poly *u = poly_init();
    poly *v = poly_init();
    poly *r = poly_init();
    precomp(g0, a, n);
    interpol(g1, n, a, ct);
    
    
    euclide_tr(r, u, v, g0, g1, (n+k)/2);


    poly *div = poly_init();
    poly *mod = poly_init();


    poly_div(div, r, v);
    poly_mod(mod, r, v);

    if (mod->deg == -1 && div->deg < k) {
        for (int i = 0; i < k; i++) {
            (*f)[i] = poly_coeff(div, i);
        } 
        return 0;
    } else {
        fprintf(stderr, "Échec du décodage!\n");
        return 1;
    }
}

void random_error(unsigned int **e, int d, int n) {
    if (d > n) {
        fprintf(stderr, "Pas assez de place pour stocker toute cette erreur");
        return;
    }
    
    for (int i = 0; i < n; i++) {
        (*e)[i] = 0;
    }
    int hit = 0;
    while (hit < d) {
        int r = rand() % n;
        if ((*e)[r] == 0) {
            hit++;
            (*e)[r] = rand() % 256;
        }
    }
}

void send_with_err(unsigned int **ct, unsigned int **m, unsigned int **a, int d, int k, int n) {
    encode1(ct, m, a, k, n);
    unsigned int *e = malloc(n * sizeof(unsigned int));
    random_error(&e, d, n);
    for (int i = 0; i < n; i++) {
        (*ct)[i] = add256((*ct)[i], e[i]);
    }
}

