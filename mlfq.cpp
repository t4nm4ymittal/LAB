#include<iostream>
#include"MLFQ.h"
using namespace std;

//main program 
int main()
{
	MLFQ obj;								//create object of MLFQ
	obj.setFileName("input3.txt");			//set file name for input data
	obj.start();							// call the start() method of MLFQ, which will perform MLFQ
	return 0;								// end of main program
}

