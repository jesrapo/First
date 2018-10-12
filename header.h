//Name: Jesse Rapoport
//Date: 15 May 2018
//Class: Karla Fant's CS 163
//Program #3: Make a table ADT that will get data from a file on different concepts, and display them, retrieve them, delete them, etc.

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

const char FILENAME[13] = "CS_Terms.txt";
const int ARRAYSIZE1 = 29;
const int ARRAYSIZE2 = 53;
const int SIZE = 1000;
const int EXITNUMBER = 5;

struct concept;
struct node;
class table;

struct concept
{
    char * term;
    char * definition;
    char * reference;
    char * citation;
};

struct node
{
    node * next;
    concept data;
};

class table
{
    public:
        table(int size);
        ~table();
        bool addConcept(concept & conceptBeingAdded);
        void displayAll(void);
        bool display(const char term[]);
        bool retrieve(const char term[], concept & theConcept);
        void removeItemsFromSource(const char source[]);

    private:
        int findIndex(char * & termBeingAdded);
        bool addNode(node * & head, concept & conceptBeingAdded);
        void displayConcept(concept & conceptBeingDisplayed);
        void deleteNode(node * & current, const int indexPosition);
        node ** array; 
        int arraySize;
};

void makeDynamicallyAllocatedString(char * & dynamicString, const char contents[]);
void deallocateConcept(concept & conceptBeingDeallocated);
void copyConcept(concept & newConcept, concept & oldConcept);
void copyDynamicallyAllocatedString(char * & newString, char * & oldString);
void deallocateNode(node * & nodeBeingDeallocated);
void loadFileIntoTable(const char fileName[], table & aTable);
void addConceptToTable(ifstream & fileIn, table & aTable);
void getConceptFromFile(ifstream & fileIn, concept & aConcept);
void displayInstructions(void);
void welcomeUser(void);
void getUserInput(int & userInput);
void doAppropriateFunction(table & aTable, int & userInput);
void displayAll(table & aTable);
void displaySingleTerm(table & aTable);
void addNewTerm(table & aTable);
void removeItemsFromSource(table & aTable);
void thankUser(void);
void getInfoFromUser(const char nameOfThing[], char * & actualData);
