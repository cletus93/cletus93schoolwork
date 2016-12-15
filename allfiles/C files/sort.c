//This program is designed to display the difference in time it takes to sort information using differents types of sort functions.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int selection(int[]);
int insertion(int[]);
int bubble(int[]);
int bubbles(int[]);
int quick(int[], int, int);
int mergesort(int[], int, int);
int merge(int[], int, int, int);
int partition(int[], int, int);
int funrun(int[]);

int main ()
{
	hrtime_t start, end;
	int a = 100, b = 1000, c = 10000, i, j, p, q, r, x, m, n, k, key, swaps, length;
	int arr[c], arr2[a], arr3[a], arr4[a], arr5[b], arr6[b], arr7[b], arr8[c], arr9[c], arr10[c], arrn[c], arro[c], arrorg[c];
	
	srand(time(NULL));
	for(i = 0; i < a; i++)
	{
		arr2[i] = rand() % 100 + 1;
		arr3[i] = i + 1;
		arr4[i] = i + 1;
		if(i % 10 == 0)
		{
			arr4[i] = 1;
		}
	}
	for(i = 0; i < b; i++)
	{
		arr5[i] = rand() % 100 + 1;
		arr6[i] = i + 1;
		arr7[i] = i + 1;
		if(i % 10 == 0)
		{
			arr7[i] = 1;
		}
	}
	for(i = 0; i < c; i++)
	{
		arr8[i] = rand() % 100 + 1;
		arr9[i] = i + 1;
		arr10[i] = i = 1;
		if(i % 10 == 0)
		{
			arr10[i] = 1;
		}
	}

	funrun(arr2);
	funrun(arr3);
	funrun(arr4);
	funrun(arr5);
	funrun(arr6);
	funrun(arr7);
	funrun(arr8);
	funrun(arr9);
	funrun(arr10);
	return 0;
}

int selection(int arr[])
{
	int position, key, i, j;
	for(i = 0; i < (sizeof(arr)/sizeof(arr[0]) - 1); i++)
	{
		position = i;
		for(j = (i + 1); j < (sizeof(arr)/sizeof(arr[0])); j++)
		{
			if(arr[position] > arr[j])
			{
				position = j;
			}
		}
		if(position != i)
		{
			key = arr[i];
			arr[i] = arr[position];
			arr[position] = key;
		}
	}
}

int insertion(int arr[])
{
	int i, j, key;
	for(j = 1; j < (sizeof(arr)/sizeof(arr[0])); j++)
	{
		key = arr[j];
		i = j - 1;
		while(key < arr[i] && i >= 0)
		{
			arr[i + 1] = arr[i];
			i = i - 1;
		}
		arr[i + 1] = key;
	}
}

int bubble(int arr[])
{
	int key, i, j;
	for(i = 0; i < (sizeof(arr)/sizeof(arr[0]) - 1); i++)
	{
		for(j = 0; j > (sizeof(arr)/sizeof(arr[0]) - i - 1); j++)
		{
			if(arr[j] > arr[j + 1])
			{
				key = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = key;
			}
		}
	}
}

int bubbles(int arr[])
{
	int key, i, j;
	int didswap = 1;
	for(i = 0; i < (sizeof(arr)/sizeof(arr[0]) - 1) && didswap; i++)
	{
		didswap = 0;
		for(j = 0; j < (sizeof(arr)/sizeof(arr[0]) - i - 1); j++)
		{
			if(arr[j] > arr[j + 1])
			{
				didswap = 1;
				key = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = key;
			}
		}
	}
}
int quick(int arr[], int p, int r)
{
	int q;
	if(p < r)
	{
		q = partition(arr, p, r);
		quick(arr, p, q - 1);
		quick(arr, q + 1, r);
	}
}

int mergesort(int arr[], int p, int r)
{
	int q;
	if(p < r)
	{
		q = p + (r - 1) / 2;
		mergesort(arr, p, q);
		mergesort(arr, q + 1, r);
		merge(arr, p, q, r);
	}
}

int merge(int arr[], int p, int q, int r)
{
	int m, n, i, j, k;
	m = q - p + 1;
	n = r - q;
	int arr11[m], arr12[n];

	for(i = 0; i < m; i++)
	{
		arr11[i] = arr[p + i];
	}
	for(j = 0; j < n; j++)
	{
		arr12[j] = arr[q + 1 + j];
	}
	i = 0;
	j = 0;
	k = p;
	while(i < m && j < n)
	{
		if(arr11[i] <= arr12[j])
		{
			arr[k] = arr11[j];
			i = i + 1;
		}
		else
		{
			arr[k] = arr12[j];
			j = j + 1;
		}
		k = k + 1;
	}
	while(i < m)
	{
		arr[k] = arr11[i];
		i = i + 1;
		k = k + 1;
	}
	while(j < n)
	{
		arr[k] = arr12[j];
		j = j + 1;
		k = k + 1;
	}
}

int partition(int arr[], int p, int r)
{
	int key, x, i, j;
	x = arr[p];
	i = p;
	j = r + 1;
	while(1)
	{
		do
		{
			++i;
		}
		while(arr[i] <= x && i <= r);
		{
			do
			{
				--j;
			}
			while(arr[j] > x);
			{
				if( i >= j)
				{
					break;
					key = arr[i];
					arr[i] = arr[j];
					arr[j] = key;
				}
			}
		}
	}
	key = arr[i + 1];
	arr[i + 1] = arr[r];
	arr[r] = key;
return j;
}

int funrun(int arrorg[])
{
	hrtime_t start, end;
	int i, p, r, c = 10000;
	int arr[c];

	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	start = gethrtime();
	selection(arr);
	end = gethrtime();
	printf("The runtime of selectionsort is %d\n", (end - start));
	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	start = gethrtime();
	insertion(arr);
	end = gethrtime();
	printf("The runtime of insertionsort is %d\n", (end - start));
	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	start = gethrtime();
	bubble(arr);
	end = gethrtime();
	printf("The runtime of bubblesort without swaps counting is %d\n", (end - start));
	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	start = gethrtime();
	bubbles(arr);
	end = gethrtime();
	printf("The runtime of bubblesort with swaps counting is %d\n", (end - start));
	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	p = 0;
	r = sizeof(arr)/sizeof(arr[0]);
	start = gethrtime();
	quick(arr, p, r);
	end = gethrtime();
	printf("The runtime of quicksort is %d\n", (end - start));
	for(i = 0; i < (sizeof(arrorg)/sizeof(arrorg[0])); i++)
	{
		arr[i] = arrorg[i];
	}
	start = gethrtime();
	mergesort(arr, p, r);
	end = gethrtime();
	printf("The runtime of mergesort is %d\n", (end - start));
}

