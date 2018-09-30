#include <stdio.h>
#include <assert.h>
#include "calc.h"
#include "numbio.h"
#include "digits.h"

/**
 * \fn int is_zero(int *digits, int n)
 * \brief Check number for zero
**/
int is_zero(int digits[], int n)
{
	assert(digits);
	assert(n > 0);
	for (int i = 0; i < n; i++)
		if (digits[i] != 0)
			return 0;
	return 1;
}

/**
 * \fn int numb_len(int digits[])
 * \brief Calculate length of number
**/
int numb_len(int digits[])
{
	assert(digits);
	int len = MANTIS_N, i = 0;
	while (digits[i] == 0 && i < MANTIS_N)
	{
		len--;
		i++;
	}
	if (len == 0)
		len = 1;
	return len;
}

/**
 * \fn int compare(int *d1, int n1, int *d2, int n2)
 * \brief Compares two numbers
**/
int compare(int *d1, int n1, int *d2, int n2)
{
	int j1 = 0;
	while (d1[j1] == 0 && n1 - j1 > 0)
		j1++;
	
	int j2 = 0;
	while (d2[j2] == 0 && n2 - j2 > 0)
		j2++;
	
	if (n1 - j1 > n2 - j2)
		return GREATER;
	if (n1 - j1 < n2 - j2)
		return LESS;
	for (int i = j1, j = j2; i < n1 && j < n2; i++, j++)
	{
		if (d1[i] > d2[j])
			return GREATER;
		else if (d1[i] < d2[j])
			return LESS;
	}
	return EQUAL;
}

/**
 * \fn void subtract(int a[], int n1, int b[], int n2)
 * \brief Subtract from numb a numb b
**/
void subtract(int a[], int n1, int b[], int n2)
{
	int d = 0;
	int i = n1 - 1;
	
	for (int j = n2 - 1; i >= 0 && j >= 0; i--, j--)
	{
		if (a[i] + d < b[j])
		{
			a[i] = a[i] + d + 10 - b[j];
			d = -1;
		}
		else
		{
			a[i] = a[i] + d - b[j];
			d = 0;
		}
	}
	a[i] = a[i] + d;
}

/**
 * \fn int exp_sum(int e1[], int e2[])
 * \brief Sum exponents or return overflow error code
**/
int exp_sum(int e1[], int e2[])
{
	int d = 0;
	for (int i = EXP_N - 1; i >= 0; i--)
	{
		e1[i] = e1[i] + e2[i] + d;
		if (e1[i] >= 10)
		{
			e1[i] = (e1[i]) % 10;
			d = 1;
		}
		else	
			d = 0;
	}
	if (d == 1)
		return EXP_OVERFLOW;
	return OK;
}

/**
 * \fn int exp_operate(int e1[], int e2[])
 * \brief Sum or subtract exponents 
**/
int exp_operate(int e1[], int e2[])
{
	if (e1[EXP_N] == e2[EXP_N])
		return exp_sum(e1, e2);
	
	if (compare(e1, EXP_N, e2, EXP_N) == LESS)
	{
		if (e1[EXP_N] == '+')
			e2[EXP_N] = '-';
		else
			e1[EXP_N] = '+';
		subtract(e2, EXP_N, e1, EXP_N);
		for (int i = 0; i < EXP_N; i++)
			e1[i] = e2[i];
	}
	else
		subtract(e1, EXP_N, e2, EXP_N);
	return OK;
}

/**
 * \fn int divide(int mant[], int expon[], int numb[], int res[], int rexp[])
 * \brief Divide real number by integer number
**/
int divide(int mant[], int expon[], int numb[], int res[], int rexp[])
{
	for (int i = 0; i < MANTIS_N; i++)
		res[i] = 0;
	if (mant[MANTIS_N] != numb[MANTIS_N])
		res[MANTIS_N] = '-';
	else
		res[MANTIS_N] = '+';
	
	for (int i = 0; i < EXP_N + 1; i++)
		rexp[i] = 0;
	rexp[EXP_N] = '+';
	rexp[EXP_N + 1] = -1;
	
	if (is_zero(mant, MANTIS_N))
		return OK;
	if (is_zero(numb, MANTIS_N))
		return DIV_BY_ZERO;
	
	int nlen = numb_len(numb);
	int *pb = mant;
	int *pe = pb + nlen;
	int res_i = 0;
	int add_exp = expon[EXP_N + 1];
	int extra_digit = 0;
	
	while (compare(pb, pe - pb, numb + MANTIS_N - nlen, nlen) == LESS)
	{
		if (pe - mant >= MANTIS_N)
		{
			for (int *pc = pe - nlen; pc < pe; pc++)
				*pc = *(pc + 1);
			*(pe - 1) = 0;
		}
		else
			pe++;
	}
	if (pe - pb >= expon[EXP_N + 1])
		add_exp -= pe - pb - expon[EXP_N + 1];
	
	while (res_i < MANTIS_N)
	{
		if (compare(pb, pe - pb, numb + MANTIS_N - nlen, nlen) == LESS)
			res[res_i] = 0;
		else
		{
			while (compare(pb, pe - pb, numb + MANTIS_N - nlen, nlen) !=
				LESS && res[res_i] < 9)
			{
				subtract(pb, pe - pb, numb + MANTIS_N - nlen, nlen);
				res[res_i]++;
			}
		}
		res_i++;
		if (pe - mant >= MANTIS_N)
		{
			for (int *pc = pe - nlen; pc < pe; pc++)
				*pc = *(pc + 1);
			*(pe - 1) = 0;
		}
		else
		{
			pe++;
			pb++;
		}
		
		if (pe - pb < expon[EXP_N + 1] && rexp[EXP_N + 1] == -1)
			rexp[EXP_N + 1] = res_i;
	}
	
	if (compare(pb, pe - pb, numb + MANTIS_N - nlen, nlen) == LESS)
		extra_digit = 0;
	else
	{
		while (compare(pb, pe - pb + 1, numb + MANTIS_N - nlen, nlen) ==
			GREATER)
		{
			subtract(pb, pe - pb + 1, numb + MANTIS_N - nlen, nlen);
			extra_digit++;
		}
	}
	
	add_exp -= rexp[EXP_N + 1];
	if (add_exp < 0)
	{
		rexp[EXP_N] = '-';
		add_exp /= -1;
	}
	int k = EXP_N - 1;
	while (add_exp > 0)
	{
		rexp[k] = add_exp % 10;
		add_exp /= 10;
		k--;
	}
	
	return exp_operate(rexp, expon);
}

/**
 * \fn int normalize(int mantis[], int expon[])
 * \brief Normalize real number output format to 0. E+
**/
int normalize(int mantis[], int expon[])
{
	int dot_pos[EXP_N + 1];
	for (int i = 0; i < EXP_N; i++)
		dot_pos[i] = 0;
	dot_pos[EXP_N] = '+';
	
	int add_exp = expon[EXP_N + 1];
	int i = 0;
	
	if (expon[EXP_N + 1] == 0)
		while (mantis[i] == 0 && i < MANTIS_N)
		{
			add_exp--;
			i++;
		}
	
	if (add_exp < 0)
	{
		for (int k = expon[EXP_N + 1] - add_exp; k < MANTIS_N; k++)
			mantis[k + add_exp - expon[EXP_N + 1]] = mantis[k];
		dot_pos[EXP_N] = '-';
		add_exp /= -1;
	}
	i = EXP_N - 1;
	while (add_exp > 0)
	{
		dot_pos[i] = add_exp % 10;
		add_exp /= 10;
		i--;
	}
	expon[EXP_N + 1] = 0;
	return exp_operate(expon, dot_pos);
}