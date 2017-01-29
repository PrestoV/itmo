#include <stdio.h>

#define LENGTH_OF_ARRAY(array) \
(sizeof(array) / sizeof(*array))

int sum(int* array, size_t length_of_array);


int array[] = {1,2,3,4,5,6,7,8,9,10};

int main(int argc, char** argv)
{
    int index;
    
    printf("Array: ");
    for(index = 0; index < LENGTH_OF_ARRAY(array); index++)
    {
        printf("%s ", array[index]);
    }
    
    printf("\nSum: %i\n", sum(array, LENGTH_OF_ARRAY(array)) );
}


int sum(int* array, size_t length_of_array)
{
    int sum = 0;
    
    int index;
    for(index = 0; index < length_of_array; index++)
    {
        sum += array[index];
    }
    
    return sum;
}

