#include "list_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int menu()
{
    list_t header;
    int choice = -1;
    void (*menu[5])(list_t *header);

    header.element = NULL;
    header.prev = &header;
    header.next = &header;

    menu[0] = list_input_front;
    menu[1] = list_input_back;
    menu[2] = list_print_sum;
    menu[3] = list_print_number;
    menu[4] = list_clear;

    do
    {
        printf("*********************\n");
        printf("1. Add numbers in the front of the list\n");
        printf("2. Add numbers in the back of the list\n");
        printf("3. Sum of the numbers in the list\n");
        printf("4. Get a number by index\n");
        printf("5. Clear the list\n");
        printf("Press menu item number or 0 to exit...");

        fseek(stdin, 0, SEEK_END);
        if (scanf("%d", &choice) == 1 && choice > 0 && choice < 6)
        {
            printf("\n");
            menu[choice - 1](&header);
            fseek(stdin, 0, SEEK_END);
            getchar();
        }

    } while (choice != 0);

    return 0;
}

void list_input_front(list_t *header)
{
    int *num = malloc(sizeof(int));

    printf("Enter items (press Ctrl+D to complete): ");
    fseek(stdin, 0, SEEK_END);
    while (scanf("%d", num) > 0)
    {
        if (!list_add_front(num, header))
        {
            printf("Error adding!\n");
            break;
        } else
        {
            num = malloc(sizeof(int));
        }
    }

    printf("\nNumber of items in the list: %i\n", list_length(header));
}

void list_input_back(list_t *header)
{
    int *num = malloc(sizeof(int)); 
    int i;

  /*  printf("Enter items (press Ctrl+D to complete): ");
    fseek(stdin, 0, SEEK_END);
    while (scanf("%d", num) > 0)
    {
        if (!list_add_back(num, header))
        {
            printf("Error adding!\n");
            break;
        } else
        {
            num = malloc(sizeof(int));
        }
    }
*/
	srand(time(NULL));
	for(i = 0; i < 1000000; i++)
	{
		*num = rand() % 10;
	 	if (!list_add_back(num, header))
        	{
            	    	printf("Error adding!\n");
          	    	break;
        	} 
		else
        	{
            		num = malloc(sizeof(int));
        	}
	}

    printf("\nNumber of items in the list: %i\n", list_length(header));
}

void list_print_sum(list_t *header)
{
    printf("Sum: %d", int_list_sum(header));
}

void list_print_number(list_t *header)
{
    int index;
    int length;
    list_t *node;

    length = list_length(header);
    if (!length) {
        printf("The list is empty.\n");
        return;
    }

    printf("Index: ");
    do
    {
        fseek(stdin, 0, SEEK_END);
    } while (scanf("%d", &index) != 1);

    if (index < 0)
    {
        printf("An index can not be negative!\n");
        return;
    }

    node = list_node_at(header, index);
    if (node)
    {
        printf("list[%d]: %d", index, *((int *) (node->element)));
    } else
    {
        printf("The index is out of range, available: [0, %d]\n", length - 1);
    }
}

void list_clear(list_t *header)
{
    list_free(header);
    printf("The list has been cleared.\n");
}
