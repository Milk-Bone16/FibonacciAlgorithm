#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <math.h>

/*
* Given F(k) and F(k+1)
* F(2k) = F(k)[2F(k + 1) - F(k)]
* F(2k + 1) = F(k + 1)^2 + F(k)^2
*
* returns the nth fibonacci number found
*/
size_t calculateFibonacci(uint64_t n, mpz_t res, size_t predictedSizeBits) {
	size_t currentFib = 2;


	mpz_t a;
	mpz_init2(a, predictedSizeBits);
	mpz_set_ui(a, 1);

	mpz_t b;	// b will store F(2k + 1) after F(2k) is calculated
	mpz_init2(b, predictedSizeBits);
	mpz_set_ui(b, 2);

	mpz_t c;	// c will store F(2k)
	mpz_init2(c, predictedSizeBits);
	
	while(currentFib < n){
		mpz_mul_ui(c, b, 2);
		mpz_sub(c, c, a);
		mpz_mul(c, c, a); // c now holds F(2k)

		//mpz_pow_ui(a, a, 2);
		//mpz_pow_ui(b, b, 2);
		mpz_mul(a, a, a);
		mpz_mul(b, b, b);
		mpz_add(b, b, a); // b now holds F(2k + 1)

		mpz_swap(a, c); // swap so that a equals F(2k)

		currentFib *= 2;
	}

	mpz_set(res, b);

	//printf("a: %d\n", a[0]._mp_size);
	//printf("b: %d\n", b[0]._mp_size);

	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);

	return currentFib + 1;
}

int main(int argc, char* argv[]){
	if (argc == 1) {
		fprintf(stderr, "Provide number to find fibanacci number to\n");
		return 1;
	}
	uint64_t findNumber = strtoul(argv[argc - 1], NULL, 10);
	// predictedSize is (2^(ceil(log_2(n))+1) * log(phi) - log(sqrt(5)))
	// This prediction is reasonbly accurate but could be better with larger ocnstants
	size_t predictedSizeBits = ((pow(2, ceil(log2((double)findNumber))) + 1) * 0.6942419136306173017387902669 - 1.16096404744368117393515971474);
	printf("Predicted Size Bits: %lu\n", predictedSizeBits);
	mpz_t finalNumber;
	mpz_init2(finalNumber, predictedSizeBits);
	size_t foundFib;

	clock_t start, end;
	start = clock();
	foundFib = calculateFibonacci(findNumber, finalNumber, predictedSizeBits);
	end = clock();

	printf("Actual Size Bits:    %d\n", 64 * finalNumber[0]._mp_size);
	printf("Actual Size Bits al: %d\n", 64 * finalNumber[0]._mp_alloc);

	printf("F(%lu) = \n", foundFib);
	if (*argv[1] != 's') {
		mpz_out_str(stdout, 10, finalNumber);
	}
	putchar('\n');
	
	printf("Time taken: %fs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	
	return 0;
}

