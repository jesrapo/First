//Name: Jesse Rapoport
//Date: 15 May 2018
//Class: Karla Fant's CS 163
//Program #3: Make a table ADT that will get data from a file on different concepts, and display them, retrieve them, delete them, etc.

#include "header.h" 

//takes a const char array and turns it into a dynamically allocated string of the correct size
void makeDynamicallyAllocatedString(char * & dynamicString, const char contents[])
{
    int length = strlen(contents);
    dynamicString = new char[length + 1];
    strcpy(dynamicString, contents);
}

//constructor for the table
table::table(int size)
{
    arraySize = size; //make the arraySize whatever was given to the constructor
    array = new node*[arraySize]; //make the array of nodes
    for(int i = 0; i < arraySize; ++i) //set them all to NULL
        array[i] = NULL;
}

//destructor for the table 
table::~table()
{
    node * current;
    node * tempNext;

    //delete all the nodes and their contents
    for(int i = 0; i < arraySize; ++i)
    {
        if(array[i])
        {
            current = array[i]; //head
            while(current)
            {
                tempNext = current->next;
                deallocateNode(current);
                current = tempNext;
            }
        }
    }

    //delete the array
    delete array;
}

//add a concept to the table
bool table::addConcept(concept & conceptBeingAdded)
{
    concept conceptCopy;
    int index;
    bool didItWork;

    copyConcept(conceptCopy, conceptBeingAdded); //copy the concept passed in, so we're not dealing with the client program's copy
    index = findIndex(conceptCopy.term); //where the concept will be stored in the array
    didItWork = addNode(array[index], conceptCopy); //attempts to add the node

    if(!didItWork) //deallocate the concept copy if it wasn't stored into a node
        deallocateConcept(conceptCopy);

    return didItWork;
}

//returns the appropriate index for the conceptBeingAdded
int table::findIndex(char * & termBeingAdded)
{
    char firstLetter = *termBeingAdded;
    int firstLetterValue = int(firstLetter);
    int index = firstLetterValue % arraySize;
    return index;
}

//adds a node to the array
bool table::addNode(node * & head, concept & conceptBeingAdded)
{
    node * current = head;
    node * trailing = current;
    int areTheyTheSame; //checks whether the new concept's term is already included

    //if it's an empty list, add the new concept to the LLL
    if(!head)
    {
        head = new node;
        head->next = NULL;
        head->data = conceptBeingAdded;
    }

    //if it's a list with nodes, add it to the end of the LLL
    else
    {
        //traverse to the end of the list, checking if the term being added already exists
        while(current)
        {
            trailing = current;
            areTheyTheSame = strcmp(conceptBeingAdded.term, current->data.term);
            if(areTheyTheSame == 0)
                return false;
            current = current->next;
        }

        //add the new node to the end of the LLL
        trailing->next = new node;
        current = trailing->next;
        current->next = NULL;
        current->data = conceptBeingAdded;
    }

    //if it didn't return false earlier, then it worked
    return true;
}

//displays all of the table
void table::displayAll(void)
{
    node * current;
    for(int i = 0; i < arraySize; ++i)
    {
        current = array[i];
        while(current)
        {
            displayConcept(current->data);
            current = current->next;
        }
    }
}

//displays a single concept
void table::displayConcept(concept & conceptBeingDisplayed)
{
    cout << "Concept: " << conceptBeingDisplayed.term << endl;
    cout << "Definition: " << conceptBeingDisplayed.definition << endl;
    cout << "Reference information: " << conceptBeingDisplayed.reference << endl;
    cout << "Citation: " << conceptBeingDisplayed.citation << endl;
    cout << endl;
}

//deallocates a concept
void deallocateConcept(concept & conceptBeingDeallocated)
{
    delete conceptBeingDeallocated.term;
    delete conceptBeingDeallocated.definition;
    delete conceptBeingDeallocated.reference;
    delete conceptBeingDeallocated.citation;
}

//copys a concept
void copyConcept(concept & newConcept, concept & oldConcept)
{
    copyDynamicallyAllocatedString(newConcept.term, oldConcept.term);
    copyDynamicallyAllocatedString(newConcept.definition, oldConcept.definition);
    copyDynamicallyAllocatedString(newConcept.reference, oldConcept.reference);
    copyDynamicallyAllocatedString(newConcept.citation, oldConcept.citation);
}

//copys a dynamically allocated string into another one
void copyDynamicallyAllocatedString(char * & newString, char * & oldString)
{
    int length = strlen(oldString);
    newString = new char[length + 1];
    strcpy(newString, oldString);
}

//deallocate a node
void deallocateNode(node * & nodeBeingDeallocated)
{
    deallocateConcept(nodeBeingDeallocated->data);
    delete nodeBeingDeallocated;
}

//loads a file into the table
void loadFileIntoTable(const char fileName[], table & aTable)
{
    //open the file
    ifstream fileIn;
    fileIn.open(fileName);

    //get the data from the file if it exists
    if(fileIn)
    {
        //add concepts to the table until it runs out
        while(!fileIn.eof())
            addConceptToTable(fileIn, aTable);
    }
    //if the file doesn't exist
    else
        cout << "The file " << fileName << " was not found." << endl;

    //close the file
    fileIn.close();
}

//gets a concept from the file, and adds it to the table
void addConceptToTable(ifstream & fileIn, table & aTable)
{
    concept aConcept;

    getConceptFromFile(fileIn, aConcept); //get a concept from the file
    aTable.addConcept(aConcept); //add the file to the table
    deallocateConcept(aConcept); //delete the copy of the concept from out here
}

//gets the next concept from the file
//runs assuming there is a next concept
void getConceptFromFile(ifstream & fileIn, concept & aConcept)
{
    char temp[SIZE];

    //get the term
    fileIn.get(temp, SIZE, '|');
    fileIn.ignore();
    makeDynamicallyAllocatedString(aConcept.term, temp);

    //get the definition
    fileIn.get(temp, SIZE, '|');
    fileIn.ignore();
    makeDynamicallyAllocatedString(aConcept.definition, temp);

    //get the reference information
    fileIn.get(temp, SIZE, '|');
    fileIn.ignore();
    makeDynamicallyAllocatedString(aConcept.reference, temp);

    //get the citation
    fileIn.get(temp, SIZE, '\n');
    fileIn.ignore();
    makeDynamicallyAllocatedString(aConcept.citation, temp);
}

/*
//displays all the information about the term passed in.
//returns true if the term exists, false if it doesn't
bool table::display(const char term[])
{
    int index; //the index of the term in the array
    char * dynamicTerm; //the term converted to a dynamic array
    node * current; 
    bool areTheyTheSame;
    makeDynamicallyAllocatedString(dynamicTerm, term); //make the term dynamically allocated
    index = findIndex(dynamicTerm); //find the index
    current = array[index]; //set current to the first node in the right index of the array

    //traverse the LLL at array[index] for all data items at that spot
    while(current)
    {
        areTheyTheSame = strcmp(dynamicTerm, current->data.term); //check if the term is found
        //if it's found, display it, delete the dynamically allocated term, and return true
        if(areTheyTheSame == 0) 
        {
            displayConcept(current->data);
            delete dynamicTerm;
            return true;
        }
        current = current->next;
    }
    delete dynamicTerm; //delete the dynamically allocated version of the term
    return false; //if true hasn't been returned, return false;
}
*/

//welcomes user
void welcomeUser(void)
{
    cout << endl;
    cout << "Hello, and welcome to the program where you" << endl;
    cout << "can see the names of the terms stored in the" << endl;
    cout << FILENAME << " file. You will also see their definitions," << endl;
    cout << "their reference information, and a citation." << endl;
    cout << "You cannot alter the file itself, but you can" << endl;
    cout << "alter the way they're stored in this program." << endl;
}

//displays instructions
void displayInstructions(void)
{
    cout << endl;
    cout << "To display all terms and their information, press 1." << endl;
    cout << "To display all information about a specific term, press 2." << endl;
    cout << "To add a new term, press 3." << endl;
    cout << "To remove all items from a particular source, press 4." << endl;
    cout << "To exit, press 5." << endl;
}

//gets the user input of which function to execute
void getUserInput(int & userInput)
{
    cout << endl;
    cout << "Please enter your choice: ";
    cin >> userInput;
    cin.ignore(SIZE, '\n');

    if(userInput < 1 || userInput > EXITNUMBER)
    {
        cout << "Error. ";

        //recursively call the function again until a valid answer is received
        getUserInput(userInput);
    }
}

//calls whichever function was requested by the user
void doAppropriateFunction(table & aTable, int & userInput)
{
    switch(userInput)
    {
        case 1:
            displayAll(aTable);
            break;
        case 2:
            displaySingleTerm(aTable);
            break;
        case 3:
            addNewTerm(aTable);
            break;
        case 4:
            removeItemsFromSource(aTable);
            break;
        case 5:
            thankUser();
            break;
    }
}

//displays all of a table
void displayAll(table & aTable)
{
    cout << endl;
    cout << "Here are all the terms:" << endl << endl;
    aTable.displayAll();
}

//displays a single term requested by a user
void displaySingleTerm(table & aTable)
{
    bool didItWork;
    char userResponse[SIZE];
    cout << endl << "Please enter the name of the term you'd like displayed exactly, paying attention to capitals: ";
    cin.get(userResponse, SIZE, '\n');
    cin.ignore(SIZE, '\n');
    cout << endl;
    didItWork = aTable.display(userResponse);
    if(!didItWork)
        cout << "Sorry, but no term with the name " << userResponse << " was found." << endl;
}

//adds a new term to the table
void addNewTerm(table & aTable)
{
    concept newConcept;
    bool didItWork;
    getInfoFromUser("term", newConcept.term);
    getInfoFromUser("definition", newConcept.definition);
    getInfoFromUser("reference information", newConcept.reference);
    getInfoFromUser("citation", newConcept.citation);

    didItWork = aTable.addConcept(newConcept);
    if(!didItWork)
        cout << "Error. Term with name " << newConcept.term << " already present." << endl;

    deallocateConcept(newConcept); //delete the concept from here, leaving it just in the table
}

//removes all items of a source given by user
void removeItemsFromSource(table & aTable)
{
    char tempSource[SIZE];
    cout << "Please enter the name of the source whose entries you would like deleted: ";
    cin.get(tempSource, SIZE, '\n');
    cin.ignore(SIZE, '\n');
    aTable.removeItemsFromSource(tempSource);
}

//thank the user for using the program
void thankUser(void)
{
    cout << endl;
    cout << "Thanks for using the program!" << endl;
    cout << endl;
}

//get information from user, asking for the nameOfThing
void getInfoFromUser(const char nameOfThing[], char * & actualData)
{
    char temp[SIZE];

    cout << "Please enter the " << nameOfThing << ": ";
    cin >> temp;
    cin.ignore(SIZE, '\n');

    makeDynamicallyAllocatedString(actualData, temp);
}

//retrieves a concept from the table
bool table::retrieve(const char term[], concept & theConcept)
{
    int index; //the index of the term in the array
    char * dynamicTerm; //the term converted to a dynamic array
    node * current; 
    bool areTheyTheSame;
    makeDynamicallyAllocatedString(dynamicTerm, term); //make the term dynamically allocated
    index = findIndex(dynamicTerm); //find the index
    current = array[index]; //set current to the first node in the right index of the array

    //traverse the LLL at array[index] for all data items at that spot
    while(current)
    {
        areTheyTheSame = strcmp(dynamicTerm, current->data.term); //check if the term is found
        //if it's found, copy it into the concept being returned, delete the dynamically allocated term, and return true
        if(areTheyTheSame == 0) 
        {
            copyConcept(theConcept, current->data);
            delete dynamicTerm;
            return true;
        }
        current = current->next;
    }
    delete dynamicTerm; //delete the dynamically allocated version of the term
    return false; //if true hasn't been returned, return false;
}

//removes items from a given source
void table::removeItemsFromSource(const char source[])
{
    node * current;
    node * next;
    bool areTheyTheSame;
    char * dynamicSource; //the source converted to a dynamic array
    makeDynamicallyAllocatedString(dynamicSource, source); //make the source dynamically allocated

    for(int i = 0; i < arraySize; ++i)
    {
        current = array[i];
        while(current)
        {
            next = current->next;
            areTheyTheSame = strcmp(dynamicSource, current->data.reference);
            if(areTheyTheSame == 0)
            {
                deleteNode(current, i);
                current = next;
            }
            else
                current = current->next;
        }
    }
    delete dynamicSource;
}

//deletes a node given its location
void table::deleteNode(node * & current, const int indexPosition)
{
    node * trailing;
    node * next;
    node * head;
    node * nodeBeingDeleted = current;
    char * termBeingDeleted;
    bool areTheyTheSame;
    bool isNodeFirst;
    bool isThereNextNode;

    makeDynamicallyAllocatedString(termBeingDeleted, nodeBeingDeleted->data.term);
    head = array[indexPosition]; //the first node in the current LLL
    current = head;

    while(current)
    {
        areTheyTheSame = strcmp(current->data.term, termBeingDeleted);

        //if the term was found
        if(areTheyTheSame == 0)
        {
            //if head and current have the same address, then this is the first node in the LLL
            isNodeFirst = head == current;

            //if it's the first node
            if(isNodeFirst)
            {
                array[indexPosition] = current->next;
                deallocateNode(current);
                delete termBeingDeleted;
                return;
            }
            //if it's not the first node
            else
            {
                next = current->next;
                deallocateNode(current);
                trailing->next = next;
                delete termBeingDeleted;
                return;
            }
        }

        //update trailing and current
        trailing = current;
        current = current->next;
    }
}
