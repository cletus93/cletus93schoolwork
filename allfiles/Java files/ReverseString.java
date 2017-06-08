package srcpac;

import java.util.Scanner;

public class ReverseString 
{
	public static void main(String[] args) 
	{
		Scanner scanner = new Scanner(System.in);
		String inputstring;
		String outputstring;
		System.out.println("Enter the string you would like to reverse:");
		inputstring = scanner.nextLine();
		System.out.println("You typed: " + inputstring);
		outputstring = reverse(inputstring);
		System.out.println("Your reverse string is: " + outputstring);
	}

	public static String reverse(String input)
	{
		char[] in = input.toCharArray();
		int begin=0;
		int end=in.length-1;
		char temp;
		while(end>begin){
			temp = in[begin];
			in[begin]=in[end];
			in[end] = temp;
			end--;
			begin++;
		}
		return new String(in);
	}
}