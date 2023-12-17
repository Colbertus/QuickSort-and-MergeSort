// Author: Colby McClure 
// Purpose: The Driver File for Project 2 
#include "CS317_Project_2_McClure.h"


int main()
{
    // The variable storing the name of the file 
    string input; 

    cout << "Enter the name of your input file: ";
    cin >> input; 

    // Calls the starting function to read in the input file and build the graph
    readAndBuild(input); 
    return 0;
}