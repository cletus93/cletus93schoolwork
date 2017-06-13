//Randall Rootz 6/13/17, This is a simple project to display math functions in Java
package srcpac;

import java.util.Scanner;

public class Primes 
{
	public static void main(String[] args) 
	{
		Scanner scanner = new Scanner(System.in);  //Scanner to recieve user input
		int inputnum = 0;
		int flag = 0; //0 is prime, 1 is not
		System.out.println("Enter the number you would like to check as a prime:");
		if(scanner.hasNextInt()){
        	inputnum = scanner.nextInt();  //save num input
        }
        else{
         //error, no number was entered
          System.out.println("You did not enter a number, exiting now...");
        }
		
		flag = checkprime(inputnum);  //run number through funtion to determine if it is a prime
		if(flag == 0) //Prime check was good
		{
			System.out.println(inputnum + " is a Prime");
		}
		else if(flag == 1) //Failed prime check
		{
			System.out.println(inputnum + " is not a Prime");
		}
		else //unknown return value reached
		{
			System.out.println("Error");
		}
	}
	
	//function to check if a number entered is prime
	public static int checkprime(int input)
	{
		int tempflag = 0;  //0 means prime, 1 means not prime
		if(input % 2 == 0 && input != 2)//check if number is not 2 and is divisible by 2
		{
			tempflag = 1;
		}
		if(input % 3 == 0 && input != 3)//check if number is not 3 and is divisible by 3
		{
			tempflag = 1;
		}
		if(input % 5 == 0 && input != 5)//check if number is not 5 and is divisible by 5
		{
			tempflag = 1;
		}
		if(input % 7 == 0 && input != 7)//check if number is not 7 and is divisible by 7
		{
			tempflag = 1;
		}
		return tempflag;
	}
}
