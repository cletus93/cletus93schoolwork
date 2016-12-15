//Randall Rootz, 2/5/16, this program finds the nth term of the fibonacci sequence using recursion when prompted by the user
#include <stdio.h>

int main ()
{
	int i, k, n0 = 0, n1 = 1, n2;
	
//get the nth term the user would like to find
	do
	{
		printf("Please enter the \"nth\" term of the fibonacci sequence you would like to go to, it must be positive:\n");
		scanf("%d", &k);
	}	
//find the fibonacci number	and print it
	while(k < 0);
	{
		if (k == 1)
		{
			printf("Your fibonacci number is %d.\n", n0);
		}
		else if(k == 2)
		{
			printf("Your fibonacci number is %d.\n", n1);
		}
		else
		{
			for (i = 2; i < k; i++)
			{
				n2= n0 + n1;
				n0= n1;
				n1= n2;
			}
			printf("Your fibonacci number is %d.\n", n2);	
		}
	}
return 0;
}
