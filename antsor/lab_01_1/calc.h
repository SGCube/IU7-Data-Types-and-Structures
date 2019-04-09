#ifndef __CALC_H__
#define __CALC_H__

#define EQUAL 0
#define GREATER 1
#define LESS 2

#define EXP_OVERFLOW -11
#define DIV_BY_ZERO -12

/**
 * \fn int is_zero(int *digits, int n)
 * \brief Check number for zero
 * \details Determine if number is zero
 * \param [in] digits array of number digits
 * \param [in] n amount of digits
 * \return True(1) or False(0)
**/
int is_zero(int *digits, int n);

/**
 * \fn int numb_len(int digits[])
 * \brief Calculate length of number
 * \param [in] digits array of number digits
 * \return length of number
**/
int numb_len(int digits[]);

/**
 * \fn int compare(int *d1, int n1, int *d2, int n2)
 * \brief Compares two numbers
 * \param [in] d1 array of 1st number digits
 * \param [in] n1 amount of 1st number digits
 * \param [in] d2 array of 2nd number digits
 * \param [in] n2 amount of 2nd number digits
 * \return EQUAL(0)/GREATER(1)/LESS(2)
**/
int compare(int *d1, int n1, int *d2, int n2);

/**
 * \fn void subtract(int a[], int n1, int b[], int n2)
 * \brief Subtract from numb a numb b
 * \param [in] a array of 1st number digits
 * \param [in] n1 amount of 1st number digits
 * \param [in] b array of 2nd number digits
 * \param [in] n2 amount of 2nd number digits
**/
void subtract(int a[], int n1, int b[], int n2);

/**
 * \fn int exp_sum(int e1[], int e2[])
 * \brief Sum exponents or return overflow error code
**/
int exp_sum(int e1[], int e2[]);

/**
 * \fn int exp_operate(int e1[], int e2[])
 * \brief Sum or subtract exponents 
**/
int exp_operate(int e1[], int e2[]);

/**
 * \fn int divide(int mant[], int expon[], int numb[], int res[], int rexp[])
 * \brief Divide real number by integer number
**/
int divide(int mant[], int expon[], int numb[], int res[], int rexp[]);

/**
 * \fn int normalize(int mantis[], int expon[])
 * \brief Normalize real number output format to 0. E+
**/
int normalize(int mantis[], int expon[]);

#endif