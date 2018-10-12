//Name: Jesse Rapoport
//Date: 15 May 2018
//Class: Karla Fant's CS 163
//Program #3: Make a table ADT that will get data from a file on different concepts, and display them, retrieve them, delete them, etc.

#include "header.h"

int main()
{
    int userInput;
    table myTable(ARRAYSIZE1); //makes a table of ARRAYSIZE1, you can switch this to ARRAYSIZE2 for different level of efficiency

    loadFileIntoTable(FILENAME, myTable);
    
    welcomeUser();
    do
    {
        displayInstructions();
        getUserInput(userInput);
        doAppropriateFunction(myTable, userInput);
    }
    while(userInput != EXITNUMBER);
}
