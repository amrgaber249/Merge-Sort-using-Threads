#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int size;
int *list;

typedef struct
{
	// holds the start and the end of an array
    int i;
    int j;
} NODE;


void list_read()
{
    //take input array
    FILE *file;
    file = fopen("input.txt", "r+");

    //read the size of the array
    fscanf(file, "%d", &size);
    printf("size: %d\n",size);
	
	//allocating array size
    list = malloc( sizeof *list * size);
	
	//reading the array
    for (int i = 0; i < size; i++)
        fscanf(file, "%d", &list[i]);
	
    fclose(file);
}


void merge(int i, int j)
{
    int index = 0;
	
	int tsize = j - i + 1;
    int temp[tsize];
	
    int mid = (i + j) / 2;
    int i2 = i;
    int j2 = mid + 1;

    while(i2 <= mid && j2 <= j)
    {
        if (list[i2] > list[j2])
            temp[index++] = list[j2++];
        else
            temp[index++] = list[i2++];
    }

    while(i2 <= mid)
    {
        temp[index++] = list[i2++];
    }

    while(j2 <= j)
    {
        temp[index++] = list[j2++];
    }

    for (int k = 0; k < tsize; k++)
        list[k+i] = temp[k];

}


void * mergesort(NODE *n)
{
	NODE n1;
    NODE n2;
	
	int mid = (n->i + n->j) / 2;

    n1.i = n->i;
    n1.j = mid;

    n2.i = mid + 1;
    n2.j = n->j;

	//base case for splitting
    if (n->i >= n->j)
        return;
	
	//creating 2 threads for each split
	pthread_t t1;
    pthread_t t2;
	
    pthread_create(&t1, NULL, mergesort, &n1);
    pthread_create(&t2, NULL, mergesort, &n2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    merge(n->i, n->j);
    pthread_exit(NULL);
}


int main()
{
    list_read();
    if(size>1)
    {
        NODE n;
		
        printf("\ncreating threads\n");
        pthread_t thread;
        clock_t start, end;
        double cpu_time_used;
		
		//initalizing the node to the list
        n.i = 0;
        n.j = size-1;

        start = clock();
        pthread_create(&thread, NULL, mergesort, &n);
        pthread_join(thread, NULL);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("END	%lf\n\n",cpu_time_used);
		
        for (int i = 0; i < size; i++)
            printf ("%d ", list[i]);
    }
    else
    {
        if(size == 0)
            printf("\nList is Empty and sorted\n");
        else
            printf("\nlist is already sorted\n%d",list[0]);
    }
    printf ("\n");
    free(list);
    return 0;
}
