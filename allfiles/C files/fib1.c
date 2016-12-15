//Randall Rootz, 2/4/16, This program finds the nth fibonacci number in the sequence when prompted by the user.
#include <stdio.h>

int fibonacci(int);

int main ()
{
	int i, k, n0 = 0, n1 = 1, n2;

//get the nth term the user would like to find
	do
	{
		printf("Please enter the \"nth\" term of the fibonacci sequence you would like to go to, it must be postive:\n");
		scanf("%d", &k);
	}
	while(k < 0);
//find the fibonacci number and print it	
	printf("Your fibonacci number is %d.\n", fibonacci(k));
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
