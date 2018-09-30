#include <stdio.h>
#include <assert.h>
#include "numbio.h"
#include "digits.h"

//считывание строки с целым числом
int read_int(int numb[])
{
	char str[MANTIS_N + 1];
	
	//сообщение
	printf("Enter integer number in format\n");
	printf("| - sign, = - digit\n");
	printf("|");
	for (int i = 0; i < MANTIS_N; i++)
		printf("=");
	printf("\n");
	
	//ввод
	int len = -1;
	do
	{
		len++;
		scanf("%c", &str[len]);
	}
	while(str[len] != '\n' && len < MANTIS_N + 1);
	
	//обработка полученной строки
	if (len == 0)
		return ERR_EMPTY;
	return str_to_int(str, len, numb, MANTIS_N);
}

//считывание строки с вещественным числом
int read_real(int mantis[], int expon[])
{
	char str[MANTIS_N + EXP_N + 6];
	
	//сообщение
	printf("Enter real number in format\n");
	printf("| - sign, = - digit, E - sign 'E' or 'e'\n");
	printf("|");
	for (int i = 0; i < MANTIS_N; i++)
		printf("=");
	printf(" E |");
	for (int i = 0; i < EXP_N; i++)
		printf("=");
	printf("\n");
	
	//ввод
	int len = -1;
	do
	{
		len++;
		scanf("%c", &str[len]);
	}
	while(str[len] != '\n' && len < MANTIS_N + EXP_N + 6);
	
	//обработка полученной строки
	if (len == 0)
		return ERR_EMPTY;
	return str_to_real(str, len, mantis, expon);
}

//перевод строки в цифры целого числа
int str_to_int(char str[], int slen, int digits[], int max_size)
{
	assert(max_size > 0);
	assert(str && digits);
	if (slen <= 0 || slen > max_size + 1)
		return ERR_FORMAT;
	//инициализация
	for(int i = 0; i < max_size; i++)
		digits[i] = 0;
	digits[max_size] = '+';
	//определение знака
	int j = 0;
	if (str[0] == '-' || str[0] == '+')
	{
		digits[max_size] = str[0];
		j = 1;
		slen--;
	}
	if (slen > max_size)
		return ERR_FORMAT;
	//считывание
	for(int i = max_size - slen; i < max_size; i++, j++)
	{
		if (str[j] < '0' || str[j] > '9')
			return ERR_DIGIT;
		else
			digits[i] = str[j] - '0';
	}
	return OK;
}

//перевод строки в цифры вещественного числа
int str_to_real(char str[], int slen, int mant[], int expon[])
{
	assert(str);
	assert(mant && expon);
	if (slen <= 0 || slen >= MANTIS_N + EXP_N + 6)
		return ERR_FORMAT;
	
	///инициализация
	for(int i = 0; i <= MANTIS_N; i++)
		mant[i] = 0;
	mant[MANTIS_N] = '+';
	for(int i = 0; i <= EXP_N; i++)
		expon[i] = 0;
	expon[EXP_N] = '+';
	expon[EXP_N + 1] = -1;
	
	int i = 0, j = 0;
	int left = -1, right = -1;
	
	///мантисса
	
	///определение знака
	if (str[0] == '-' || str[0] == '+')
	{
		mant[MANTIS_N] = str[0];
		i++;
	}
	
	///считывание до пробела
	
	while(str[i] != ' ' && i < slen)
	{
		if (str[i] == '.')
		{
			expon[EXP_N + 1] = j;
			i++;
		}
		else if (str[i] < '0' || str[i] > '9')
			return ERR_DIGIT;
		else
		{
			mant[j] = str[i] - '0';
			if (left == -1 && (mant[j] != 0 || expon[EXP_N + 1] != -1))
				left = j;
			if (expon[EXP_N + 1] != -1 && mant[j] != 0)
				right = j;
			if (left != -1)
				j++;
			i++;
		}
	}
	if (expon[EXP_N + 1] == -1)
		expon[EXP_N + 1] = j;
	if (left == -1)
		left = MANTIS_N - 1;
	if (right == -1)
		right = j - 1;
	
	///проверка соответствия формату
	if (j == 0)
		return ERR_FORMAT;
	if (!(str[i] == ' ' && (str[i+1] == 'E' || str[i+1] == 'e') &&
		str[i+2] == ' '))
		return ERR_FORMAT;
	
	///порядок
	j = 0;
	i += 3;
	
	///считывание порядка
	return str_to_int(&str[i], slen - i, expon, EXP_N);
}

//вывод вещественного числа
void print_numb(int mantis[], int expon[])
{
	printf("%c0.", mantis[MANTIS_N]);
	for (int i = 0; i < MANTIS_N; i++)
		printf("%d", mantis[i]);
	printf(" E %c", expon[EXP_N]);
	for (int i = 0; i < EXP_N; i++)
		printf("%d", expon[i]);
}