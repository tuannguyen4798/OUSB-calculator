// Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

using namespace std;

int Errorcheck(char *arv);     // declare function Integer checking

void WritePortB(int val2)
{
	char command[255] = { "ousb -r io PORTB " };  //create an string array to activate ousb board
	char val_string[4] = {};					//create another array to store input argument
	_itoa_s(val2, val_string, 10);				// convert valid int to character
	strcat_s(command, val_string);				// add the input int character  to the activation command
	FILE *fpipe;								// Pipe to command line.
	char line[255] = {};						// string to place return text.
	fpipe = (FILE*)_popen(command, "r");
	if (fpipe != NULL)
	{
		while (fgets(line, sizeof(line), fpipe))
		{
			cout << line;
		}
		_pclose(fpipe);
	}
	else cout << "Error" << endl;

}
void Display(int result)				//Display function using result after calculation
{
	if (result > 255)
		result = result % 256;			// Dynamic perfomance test
	if (result < 0 && result >= -256)
		result = result + 256;
	if (result < -256)
		result = (result % 256) + 256;
	WritePortB(result);
	
}
int ReadPortB()
{
	int val = 0;
	FILE *fpipe;										// Pipe to command line.
	char command[] = "ousb -r io PORTB ";				// Read the switch state.
	char line[255] = {};								// string to place return text.
	fpipe = (FILE*)_popen(command, "r");
	if (fpipe != NULL)
	{
		while (fgets(line, sizeof(line), fpipe))
		{
			cout << line;
		}
		_pclose(fpipe);
	}
	else cout << "Error" << endl;
	val = (int)atoi(line);
	return val;
}
int main(int argc, char *argv[])
{
	int value1, value2 = 0;
	int result = 0;
	int temp, temp1 = 0;
	switch (argc)
	{
	case 1:  // if no input is entered (only 1 argument found), print student name and ID
		cout << "s3651585,s3651585@rmit.edu.vn,Dat,Tran" << endl;
		cout << "s3634951,s3634951@rmit.edu.vn,Tuan,Nguyen" << endl;
		cout << "s3577794,s3577794@rmit.edu.vn,Hoang,Le" << endl;
		break;
	case 2:  // if 1 input found 
		temp = Errorcheck(argv[1]); // check the input
		if (temp == 10)   // if the input return 10, exit the program
		{
			return 1;
		}
		if (temp == 0)   //if the input return 0, calculate the integer value of the input 
		{
			value1 = int(atof(argv[1]));
		}
		
		if (value1 > 65536 || value1 < -65536) //if the calculated is out of range , print R1
		{
			cout << "R1" << endl;
			return 1;
		}
		result = ReadPortB() - value1; //result is value from PortB - the calculated value1
		Display(result);  // send the result to display function for displaying
		break;
	case 3: // if there are two inputs
		
		temp = Errorcheck(argv[1]); // check the first input
		temp1 = Errorcheck(argv[2]); // check the second input
		if (temp == 10 || temp1 == 10) // if one of two inputs is invalid, exit the code
		{
			return 1;
		}
		if (temp == 0)   //if the 1st input return 0, calculate the integer value of the input 
		{
			value1 = int(atof(argv[1]));
		}
		
		if (temp1 == 0)  //if the 2nd input return 0, calculate the integer value of the input 
		{
			value2 = int(atof(argv[2]));
		}
		if (value1 > 65536 || value1 < -65536) //if  the 1st calculated inputis out of range , print R1
		{
			cout << "R1" << endl;
			return 1;
		}
		
		if (value2 > 65536 || value2 < 1) //if  the 2st calculated inputis out of range , print R2
		{
			cout << "R2" << endl;
			return 1;
		}
		result = ReadPortB() - value1 - value2;  //result is value from PortB - the calculated value1 - the calculated value2 
		Display(result);
		break;
	default:  // More than 2 inputs are detecteed, print P
		cout << "P" << endl; 
		return 1;
	}
	return 0;
}
int Errorcheck(char *arv) // Integer checking function
{
	float val,val2 = 0.F;
	int e = 0;
	
	for (int i = 0; arv[i] != '\0'; i++)	// Loop over all elements in the argument
	{

		if (!isdigit(arv[i]) && arv[i] != 'E' && arv[i] != 'e' && arv[i] != '+' && arv[i] != '-' && arv[i] != '.')  //if the digit is not +;-;e;E;. then it's invalid
		{
			cout << "X" << endl;
			return 10; //return 10 to the main function
		}
		switch (arv[i])  // if the element is among the cases listed above, check each case separately
		{
		case 'E':  // case 'E' and 'e' are similar
		case 'e':
			
			if (!isdigit(arv[i + 1]) && arv[i + 1] != '+' && arv[i + 1] != '-'&& arv[i + 1] != '\0') //if the element after e is not a digit nor +,-, \0 then the input is invalid
			{
				cout << "X" << endl;
				return 10;  //return 10 to the main()
			}

			break;
		case '+': // case + and - are similar

		case '-':
			if (arv[i + 1] == '\0' || !(isdigit(arv[i + 1])) || (isdigit(arv[i - 1]))) // the sign is at the end of line, after sign is not a digit or before sign 
			{																			// is a digit then invalid number 
				cout << "X" << endl;
				return 10;
			}
			break;
		case '.': // This is the most special case since there are many approaches and problems
			for (int k = i; arv[k] != '\0'; k++) // from the . character, loop over all element to the end of line to count no.of 'e'
			{
				if (arv[k] == 'e' || arv[k] == 'E')
					e++;
			}
				switch (e)  //check base on number of 'e' element.
				{
				case 0: // if there is no e, check if all element after . is 0
					for (int k = i; arv[k] != '\0'; k++)
					{
						if (arv[k] != '0')
						{
							cout << "X" << endl;
							return 10;
						}
					}
							break;
				case 1: // if there is 1 'e' character, d
					break;
				default: // if there are more than 1e, return error
					cout << "X" << endl;
					return 10;
				}
			break;
		default:
			break;
		}
	}

	val = int(atof(arv)); // convert the character to float, this also convert scientific number, then convert float to int
	val2 = atof(arv);
	/*cout << val << endl;
	cout << val2 << endl;*/
	if (val2 != val) // compare the float value and the int value, if two values are similar then return true, else return false
	{
		cout << "X" << endl;
		return 10;	
	}
	return 0;
	
}
