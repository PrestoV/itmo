#include <stdio.h>
#include <stdlib.h>
#define type double

#define type_size sizeof(type)

void* sum(void** array, size_t length_of_array);

int main(int argc, const char * argv[])
{
    size_t length_of_array;
    void** array;
    int index;
    double num;
    
    printf("Enter length of array: ");
    while( !scanf("%lu", &length_of_array) )
    {
        printf("Incorrect input!\n");
        fseek(stdin, 0, SEEK_END);
        printf("Enter length of array: ");
    }
    
    array = malloc(length_of_array * 8);
    if(array == NULL)
    {
        printf("Cannot allocate memory!\n");
        return 1;
    }
    
    for(index = 0; index < length_of_array; index++)
    {
        printf("Num[%i]: ", index + 1);
        while (!scanf("%lf", &num))
        {
            printf("Incorrect input!\n");
            fseek(stdin, 0, SEEK_END);
            printf("Num[%i]: ", index + 1);
        }
        
        array[index] = malloc(type_size);
        * ((type*)(array[index]) ) = num;
        
    }
    
    printf("Sum: %f\n", *( (type*)sum(array, length_of_array) ));
    
    return 0;
}

void* sum(void** array, size_t length_of_array)
{
    void* sum;
    int index;
   
    sum = malloc(sizeof(type));
   
    for(index = 0; index < length_of_array; index++)
    {
        *((type*)sum) += *((type*)array[index]);

    }
    
    return sum;
}
