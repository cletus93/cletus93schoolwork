//Randall Rootz 5/28/17, this program is a simple text editing program to reverse a string
package srcpac;

import java.util.Scanner;

public class ReverseString 
{
	public static void main(String[] args) 
	{
		Scanner scanner = new Scanner(System.in);  //Scanner to recieve user input
		String inputstring;
		String outputstring;
		System.out.println("Enter the string you would like to reverse:");
		inputstring = scanner.nextLine();  //Scan for user input
		System.out.println("You typed: " + inputstring); //print old string
		outputstring = reverse(inputstring);  //reverse the string
		System.out.println("Your reverse string is: " + outputstring);  //print new string
	}

	//function to flip the string
	public static String reverse(String input)
	{
		char[] in = input.toCharArray();  //save the string to an array
		int begin=0;   //set first array point
		int end=in.length-1;  //find string length
		char temp;
		while(end>begin){  //flip the string using the array positions
			temp = in[begin];
			in[begin]=in[end];
			in[end] = temp;
			end--;
			begin++;
		}
		return new String(in);  //send string back to main
	}
}