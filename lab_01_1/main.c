#include <stdio.h>
#include "digits.h"
#include "numbio.h"
#include "calc.h"

int main()
{
	setbuf(stdout, NULL);
	int r_mantis[MANTIS_N + 1], r_exp[EXP_N + 2];
	int res_mantis[MANTIS_N + 1], res_exp[EXP_N + 2];
	int int_digits[MANTIS_N + 1];
	int rc = OK;
	
	rc = read_real(r_mantis, r_exp);
	if (rc == ERR_DIGIT)
		printf("Incorrect digit!\n");
	else if (rc == ERR_EMPTY)
		printf("Empty string was entered!\n");
	else if (rc == ERR_FORMAT)
		printf("Incorrect number format!\n");
	if (rc != OK)
		return rc;
	
	rc = normalize(r_mantis, r_exp);
	if (rc == EXP_OVERFLOW)
	{
		printf("Exponent overflow!\n");
		return rc;
	}
	
	rc = read_int(int_digits);
	if (rc == ERR_DIGIT)
		printf("Incorrect digit!\n");
	else if (rc == ERR_EMPTY)
		printf("Empty string was entered!\n");
	else if (rc == ERR_FORMAT)
		printf("Incorrect number format!\n");
	if (rc != OK)
		return rc;

	rc = divide(r_mantis, r_exp, int_digits, res_mantis, res_exp);
	if (rc == DIV_BY_ZERO)
		printf("Division by zero!\n");
	else if (rc == EXP_OVERFLOW)
		printf("Exponent overflow!\n");
	else
	{
		printf("\nResult:\n");
		print_numb(res_mantis, res_exp);
	}
	
	return rc;
}