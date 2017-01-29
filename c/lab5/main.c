#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list_menu.h"
#include "list.h"
#include "high_func.h"
#include "limits.h"

void print_int_element(void* element);
void println_int_element(void* element);
void* square_int(void* num);
void* cube_int(void* num);
void* max_int(void* num1, void* num2);
void* min_int(void* num1, void* num2);
void* sum_int(void* num1, void* num2);
void* abs_int(void* num);
void* twice_int(void *num);

int main()
{
        list_t *header;
    list_t *res_header;
    int *num; 
    clock_t start_t, end_t;

    num = malloc( sizeof(int) );
    header = list_create(NULL);
    if( !(header && num) )
        return 1;

    header->next = re.next;
    header->prev = header;

    list_input_back(header);

    start_t = clock();

    foreach(header, print_int_element);
    printf("\n");
    foreach(header, println_int_element);

    res_header = map(header, square_int);
    printf("Squares: ");
    foreach(res_header, print_int_element);
    printf("\n");
    list_free(res_header);

    res_header = map(header, cube_int);
    printf("Cubes: ");
    foreach(res_header, print_int_element);
    printf("\n");
    list_free(res_header);

    *num = 0;
    printf("Sum: ");
    print_int_element( foldl(num, sum_int, header) );
    printf("\n");

    *num = INT_MIN;
    printf("Max: ");
    print_int_element( foldl(num, max_int, header) );
    printf("\n");

    *num = INT_MAX;
    printf("Min: ");
    print_int_element( foldl(num, min_int, header) );
    printf("\n");

    map_mut(header, abs_int);
    printf("Abs nums: ");
    foreach(header, print_int_element);
    printf("\n");

    *num = 2;
    res_header = iterate(num, 10, twice_int);
    printf("Powers of two: ");
    foreach(res_header, print_int_element);
    printf("\n");
    list_free(res_header);

    end_t = clock();

    printf("Time: %f\n", (double)(end_t - start_t)/CLOCKS_PER_SEC);	

    return 0;
}

void print_int_element(void *element)
{
    if(element)
        printf("%d ", *((int*)element) );
}

void println_int_element(void *element)
{
    if(element)
        printf("%d\n", *((int*)element) );
}

void *square_int(void *num)
{
    int* res;
    res = malloc( sizeof(int) );

    if(!res)
        return NULL;

    *res = (*((int*)num)) * (*((int*)num));

    return res;
}

void *cube_int(void *num)
{
    int* res;
    res = malloc( sizeof(int) );

    if(!res)
        return NULL;

    *res = (*((int*)num)) * (*((int*)num)) * (*((int*)num));

    return res;
}

void *max_int(void *num1, void *num2)
{
    return (*((int*)num1)) > (*((int*)num2)) ? num1 : num2;
}

void *min_int(void *num1, void *num2)
{
    return (*((int*)num1)) < (*((int*)num2)) ? num1 : num2;
}

void *sum_int(void *num1, void *num2)
{
    int* sum;
    sum = malloc( sizeof(int) );

    if(!sum)
        return NULL;

    *sum = (*((int*)num1)) + (*((int*)num2));

    return sum;
}

void *abs_int(void *num)
{
    int* res;
    res = malloc( sizeof(int) );

    if(!res)
        return NULL;

    *res = (*((int*)num)) >= 0  ? (*((int*)num)) : -(*((int*)num));

    return res;
}

void* twice_int(void *num)
{
    int* res;
    res = malloc( sizeof(int) );

    if(!res)
        return NULL;

    *res = *((int*)num) * 2;

    return res;
}
