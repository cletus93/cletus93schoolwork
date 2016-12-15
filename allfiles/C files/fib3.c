//Randall Rootz, 2/6/16,this program compares the runtime of finding the same fibonacci number by iteration and recursion.
#include <stdio.h>
#include <sys/time.h>

int fibonacci(int);

int main ()
{
	hrtime_t start, end;	
	int i, k, n0 = 0, n1 = 1, n2;
	
	//get the nth term the user would like to find
	do
	{
		printf("Please enter the \"nth\" term of the fibonacci sequence you would like to go to, it must be positive:\n");
		scanf("%d", &k);
	}	
//get the fibonacci number and print it with the compile time	
	while(k < 0);
	{
		start = gethrtime();	
		if (k == 1)
		{
			printf("Your fibonacci number by iteration is %d.\n", n0);
			end = gethrtime();
			printf("The runtime of the iteration is %lld nsec\n", (end - start));
		}
		else if(k == 2)
		{
			printf("Your fibonacci number by iteration is %d.\n", n1);
			end = gethrtime();
			printf("The runtime of the iteration is %lld nsec\n", (end - start));
		}
		else
		{
			for (i = 2; i < k; i++)
			{
				n2= n0 + n1;
				n0= n1;
				n1= n2;
			}
			printf("Your fibonacci number by iteration is %d.\n", n2);
			end = gethrtime();
			printf("The runtime of the iteration is %lld nsec\n", (end - start));
		}
		start = gethrtime();
		printf("Your fibonacci number by recursion is %d.\n", fibonacci(k));
		end = gethrtime();
		printf("The runtime of recursion is %lld nsec\n", (end - start));
	}
	return 0;
}

int fibonacci(int k)
{
	if(k == 1)
		return 0;
	else if(k == 2)
		return 1;
	else
		return (fibonacci(k-1) + fibonacci(k-2));
}
