#ide <stdio.h>
#include <stdlib.h>
#include <math.h>

int input_long(long *const num);
int is_prime(const long num);

int main(int argc, const char * argv[])
{
    long num = 0;

    printf("Enter a number: ");
    while( !input_long(&num) )
    {
        printf("Incorrect input, try again: ");
        fseek(stdin, 0, SEEK_END);
    }

    if(num < 0)
	num *= -1;	

    printf("'%li' is prime: %s\n", num, is_prime(num) ? "yes" : "no");

    return 0;

}

int input_long(long *const num)
{
    char input_string[129];
    char *pos_end_num = input_string;

    while( !scanf("%128[^\n]s", input_string) );

    *num = (long) round( strtod(input_string, &pos_end_num) );

    return pos_end_num != input_string ? 1 : 0;
}

int is_prime(const long num)
{
    int upper_limit = sqrt(num);
    int test_num;

    if(num < 2)
        return 0;

    for(test_num = 2; test_num <= upper_limit; test_num++)
    {
        if(num % test_num == 0)
            return 0;
    }

    return 1;
}

