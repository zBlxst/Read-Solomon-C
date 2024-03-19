#include "poly.h"

int set_message(unsigned int *message, char *string, int n) {
    for (int i = 0; i < n; i++) {
        message[i] = string[i];
    }
}

int main(int argc, char *argv[]) {

    char string[] = "thomas";

    srand(time(NULL));

    size_t k = strlen(string);
    size_t n = 5*k;
    unsigned int *message = malloc(k * sizeof(unsigned int));
    set_message(message, string, k);



    unsigned int *a = malloc(n*sizeof(unsigned int));
    for (int i = 0; i < n; i++) {
        a[i] = i;
    }

    int errs[4] = {0, 1, (n-k)/2, n};

    for (int i = 0; i < 4; i++) {
        printf("Sending with %d errors\n", errs[i]);
        unsigned int *message2 = malloc(k * sizeof(unsigned int));
        unsigned int *ct = malloc(n*sizeof(unsigned int));
        send_with_err(&ct, &message, &a, errs[i], k, n);

        printf("Before encoding :");
        for (int i = 0; i < k; i++) {
            printf("%c", message[i]);
        }
        printf("\n");

        printf("Encoded :");
        for (int i = 0; i < k; i++) {
            printf("%c", ct[i]);
        }
        printf("\n");

        if (!gao_decode(&message2, &ct, &a, k, n)) {
            printf("Decoded :");
            for (int i = 0; i < k; i++) {
                printf("%c", message2[i]);
            }
            printf("\n");
        }

        printf("\n");


    }


    return 0;
}