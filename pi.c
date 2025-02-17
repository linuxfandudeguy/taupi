#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpfr.h>

// This function computes π using the Chudnovsky algorithm.
void calculate_pi(mpfr_t pi, int prec) {
    // Set the precision globally (in bits)
    mpfr_set_default_prec(prec);

    // Determine number of iterations required.
    double approx_digits = prec * 0.30103;
    int iterations = (int)(approx_digits / 14.181647) + 1;
    if (iterations < 1) iterations = 1;

    mpfr_t sum, term, K, C, C3, X, multiplier, numerator, denominator, temp;
    mpfr_inits(sum, term, K, C, C3, X, multiplier, numerator, denominator, temp, (mpfr_ptr) 0);

    mpfr_set_si(C, 640320, MPFR_RNDN);
    mpfr_pow_ui(C3, C, 3, MPFR_RNDN);
    mpfr_set_ui(X, 1, MPFR_RNDN);
    mpfr_set_zero(sum, 1);
    mpfr_set_ui(K, 1, MPFR_RNDN);

    for (int k = 0; k < iterations; k++) {
        mpfr_set_si(numerator, 13591409 + 545140134LL * k, MPFR_RNDN);
        mpfr_mul(term, K, numerator, MPFR_RNDN);
        mpfr_div(term, term, X, MPFR_RNDN);
        if (k % 2 != 0) {
            mpfr_neg(term, term, MPFR_RNDN);
        }
        mpfr_add(sum, sum, term, MPFR_RNDN);

        if (k < iterations - 1) {
            mpfr_set_ui(temp, 1, MPFR_RNDN);
            for (int i = 1; i <= 6; i++) {
                long factor = 6 * k + i;
                mpfr_mul_si(temp, temp, factor, MPFR_RNDN);
            }

            mpfr_t temp2;
            mpfr_init_set_ui(temp2, 1, MPFR_RNDN);
            for (int i = 1; i <= 3; i++) {
                long factor = 3 * k + i;
                mpfr_mul_si(temp2, temp2, factor, MPFR_RNDN);
            }

            mpfr_t temp3;
            mpfr_init_set_si(temp3, k + 1, MPFR_RNDN);
            mpfr_pow_ui(temp3, temp3, 3, MPFR_RNDN);

            mpfr_mul(denominator, temp2, temp3, MPFR_RNDN);
            mpfr_div(multiplier, temp, denominator, MPFR_RNDN);
            mpfr_mul(K, K, multiplier, MPFR_RNDN);
            mpfr_clear(temp2);
            mpfr_clear(temp3);
        }

        mpfr_mul(X, X, C3, MPFR_RNDN);
    }

    mpfr_t C3_2, denom;
    mpfr_inits(C3_2, denom, (mpfr_ptr) 0);
    mpfr_sqrt(C3_2, C3, MPFR_RNDN);
    mpfr_mul_si(denom, sum, 12, MPFR_RNDN);
    mpfr_div(pi, C3_2, denom, MPFR_RNDN);

    mpfr_clears(sum, term, K, C, C3, X, multiplier, numerator, denominator, temp, C3_2, denom, (mpfr_ptr) 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <precision (in bits)>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int precision = atoi(argv[1]); // precision in bits
    if (precision <= 0) {
        fprintf(stderr, "Error: Precision must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    // Initialize the MPFR variable for π with the specified precision.
    mpfr_t pi;
    mpfr_init2(pi, precision);

    // Calculate π using our manual implementation.
    calculate_pi(pi, precision);

    // Calculate the number of decimal digits to print based on precision.
    int decimal_digits = (int)(precision * 0.30103); // Rough conversion from bits to decimal digits

    // Format the output to print the calculated value of π with the correct number of decimal places.
    char format[10];
    snprintf(format, sizeof(format), "%%.%dRf", decimal_digits);
    printf("Calculated π with precision %d bits: ", precision);
    mpfr_printf(format, pi);

    // Clean up.
    mpfr_clear(pi);
    return EXIT_SUCCESS;
}
