//Randall Rootz, 2/10/16, this program finds all the fibonacci numbers up to the nth term and displays if they are less than 100 digits long
#include <stdio.h>

int fibonacci(int);
void print_array(int a[], int lim);

int main ()
{
	int k, n = 99;
	
//get the nth term the user would like to find
	do
	{
		printf("Please enter the \"nth\" term of the fibonacci sequence you would like to go to, it must be positive:\n");
		scanf("%d", &k);
	}	
	while(k < 0);
		fibonacci(k);
	return 0;
}

fibonacci(int k)
{
	int i, j, m, n = 99;	
	int arr1[100], arr2[100], arr3[100];

	memset(arr1, 0, sizeof(arr1));
	memset(arr2, 0, sizeof(arr2));
	memset(arr3, 0, sizeof(arr3));
	arr1[0] = 0;
	arr2[0] = 1;

	if(k == 1)
	{
		printf("Your fibonacci number is %d and it is less than 100 digits.\n", arr1[0]);
	}
	else if(k == 2)
	{
		printf("Your fibonacci number is %d and it is less than 100 digits.\n", arr2[0]);
	}
	else
	{
		for ( j = 2; j < k; j++)
		{
			for ( i = 0; i < 100; i++)
			{
				arr3[i] = arr1[i] + arr2[i];
				if(arr3[i] > 9)
				{
					arr3[i] = arr3[i] - 10;
					m = i + 1;
					arr1[m] = arr1[m] + 1;
				}
				if(arr1[99] != 0)
				{ 
					printf("The fibonacci number %d is more than 100 digits long.\n", k);
					k = 476;
					printf("The largest 100 digit fibonacci number is 476.\n");
					fibonacci(k);
					print_array(arr3, n);
					break;
				}
			}
			memcpy(arr1, arr2, sizeof(arr2));
			memcpy(arr2, arr3, sizeof(arr3));
		}
			if(k < 476)
			{
				print_array(arr3, n);
			}
	}
}

void print_array(int a[], int lim)
{
	int i;
	for( i = 99; i > 0; i--)
	{
		if(a[i] != 0)
		{
			lim = i;
			break;
		}
	}

	printf("Your finonacci number is ");	
	for(i = lim; i >= 0; i--)
	{
		printf("%d", a[i]);
	}
	printf(" and it is less than 100 digits.\n");
}
