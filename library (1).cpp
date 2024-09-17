/// Dale Buencillo, Project 1, CS 251, 3 PM Lecture, Fall 2023
/// The following project is a library management system that allows the user to view, add, and remove entries to the libaries. Users can add entries via direct user input or through the use of a database file.


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

void printMenu();

// Function definitions

string addEntry(){
    string bookTitle;
    string ISBN;
    string entry;
    cout << "What is the book title? " << endl;
    getline(cin, bookTitle);
    if (bookTitle.find(',') != bookTitle.npos){
        cout << "The book title cannot contain commas." << endl;
        entry = "";
    }
    else{
        cout << "What is the 13-digit ISBN (with hyphens)? " << endl;
        getline(cin, ISBN);
        cout << "The Following Entry Was Added" << endl << "-----------------------------" << endl;
        cout << bookTitle << " --- " << ISBN << " --- " << "In Library" << endl;
        entry = bookTitle + " --- " + ISBN + " --- " + "In Library";
    }

    return entry;
}
//verifies if ISBN is valid by using 13 point ISBN formula
int validISBN(string ISBN){
    vector<char> nums;
    for (int i = 0; i < ISBN.length(); ++i){
        if ((ISBN[i]) != '-'){
            nums.push_back(ISBN[i]);
        }
    }
    int num = ((nums[0] + (3*(nums[2])) + nums[3] + (3*(nums[4])) + nums[5] + (3*(nums[6])) + nums[7] + (3*(nums[8])) + nums[9] + (3*(nums[10])) + nums[11] + (3*(nums[11]))));
    int remainder = 10%num;
    if ((10 - remainder) != 0){
        return 1;
    }
    else{
        return 2;
    }
}

void invalidEntries(vector<string> entries){
        for (int i = 0; i < entries.size()-2; i++ ){
            string curString = entries.at(i);
            if (curString.find(',') != curString.npos)  {
                int comma1 = curString.find(',');
                int comma2 = curString.find(',', comma1+1);
                string ISBN = curString.substr (comma1+2, 17);
                //checks if ISBN is valid
                if (validISBN(ISBN) == 1)
                    cout << entries.at(i) << endl;
            }
            //if no ISBN, the entry is invalid
            else{
                cout << entries.at(i) << endl;
            }
        }
}

//outputs the current library to the terminal
void displayCurLibrary(vector<string> entries){
    cout << "Your Current Library" << endl << "--------------------" << endl;
    if (entries.size() != 0){
        for (int i = 0; i <entries.size(); ++i){
        cout << entries.at(i) << endl;
    }
    }
    else{
        cout <<"The library has no books." << endl;
    }

}
//loads library from previously established database
vector<string> loadFromData(vector <string> vec){
    ifstream file;
    string fileName;
    vector <string> newVec;
    cout << "What database to read from? " << endl;
    getline (cin,fileName);
    file.open(fileName);
    if(file.is_open() == true){
        while(!file.eof()){
        string entry;
        string bookTitle;
        string ISBN;
        string status;
        getline(file, entry);
        int comma1 = entry.find(',');
        int comma2 = entry.find(',', comma1+1);
        if ((comma1 != entry.npos) && (comma2 != entry.npos)){
            bookTitle = entry.substr(0, comma1);
            ISBN = entry.substr(comma1+1, 18);
            status = entry.substr (comma1+21);
            if (status == "0" ){
            status = "In Library";
            }
            else if (status == "1"){
                status = "Checked Out";
            }
            else if (status == "2"){
                status = "On Loan";
            }
            else if (status == "3"){
                status = "Unknown State";
            }
            else{
                status = "Invalid State";
            }
        }
        else if ((comma1 != entry.npos) && (comma2 == entry.npos)){
            bookTitle = entry.substr(0, comma1);
            ISBN = entry.substr(comma1+1,entry.length()-1);
            status = "In Library";
        }
        else{
            bookTitle = entry;
            ISBN = " 000-0-00-000000-0";
            status = "In Library";
        }
        entry = bookTitle + " ---" + ISBN + " --- " + status;
        
        newVec.push_back(entry);
        }
        cout << "Read in " << newVec.size() << " lines from the file." << endl; 
        file.close();
        vec.insert(vec.end(), newVec.begin(), newVec.end());
        return vec;
        }
    else if (file.is_open() != true){
        cout << "Could not find the database file." <<  endl;
    }
}


//loads current library out to a file
void outToFile(vector<string>vec){
    string filename;
    getline(cin, filename);
     ofstream file (filename);
     for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string bookTitle;
        string ISBN;
        string status;
        int hy1 = currLine.find("-");
        bookTitle = currLine.substr(0, hy1-1);
        ISBN = currLine.substr(hy1+4,17);
        status = currLine.substr(hy1+26);
        if (status == "In Library"){
            status = "0";
        }
        else if (status == "Checked Out"){
            status = "1";
        }
        else if (status == "On Loan"){
            status = "2";
        }
        else if (status == "Unknown State"){
            status = "3";
        }
        else if (status == "Invalid State"){
            status = "4";
        }
        currLine = bookTitle + ", " + ISBN + ", " + status;
        file << currLine << endl;
     }
     file.close();
}
//removes an entry from the library
vector<string> removeEntry(vector<string>vec){
    vector<string> updatedVec;
    string search;
    getline(cin,search);
    for (int i = 0; i < vec.size(); i++ ){
        string curString = vec.at(i);
        if (curString.find(',') != curString.npos){
            int commaPos = curString.find(',');
            string bookName = curString.substr(0, commaPos);
            string ISBN = curString.substr(commaPos,curString.length()-2);
            if (search != bookName || search != ISBN){
                updatedVec.push_back(curString);
                }
            }
        }
    return updatedVec;
    }
//searches for a specific entry within the library
void searchEntry(vector<string>vec){
    string search;
    getline(cin,search);
    for (int i = 0; i < vec.size(); i++ ){
        string curString = vec.at(i);
        int commaPos = curString.find(',');
        string bookName = curString.substr(0, commaPos);
        string ISBN = curString.substr(commaPos,curString.length()-2);
        if (search == bookName || search == ISBN){
            cout << curString << endl;
            i = vec.size();
        }
    }
}
//checks the current status of a book
int statCheck(vector <string> vec, string status){
    int num = 0;
    if (status == "insideLib"){
        for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string status;
        int hy1 = currLine.find("-");
        status = currLine.substr(hy1+26);
        if (status == "In Library"){
            status +=1;
            }  
        }
    }
    else if (status == "checkedOut"){
        for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string status;
        int hy1 = currLine.find("-");
        status = currLine.substr(hy1+26);
        if (status == "Checked Out"){
            status +=1;
            }  
        }
    }
    else if (status == "onLoan"){
        for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string status;
        int hy1 = currLine.find("-");
        status = currLine.substr(hy1+26);
        if (status == "On Loan"){
            status +=1;
            }  
        }
    }
    else if (status == "unknownStatus"){
        for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string status;
        int hy1 = currLine.find("-");
        status = currLine.substr(hy1+26);
        if (status == "Unknown"){
            status +=1;
            }  
        }
    }
    else if (status == "other"){
        for (int i = 0; i < vec.size(); i++){
        string currLine = vec.at(i);
        string status;
        int hy1 = currLine.find("-");
        status = currLine.substr(hy1+26);
        if (status == "Invalid State"){
            status +=1;
            }  
        }
    }
    return num;
}


//outputs current library checkout status
void checkoutStats(vector<string>vec){
    int length = vec.size();
    int inLibrary = statCheck(vec, "insideLib");
    int checked = statCheck(vec, "checkedOut");
    int loaned = statCheck(vec, "onLoan");
    int unknown = statCheck(vec,"unknownStatus");
    int other = statCheck(vec, "other");

    cout << "Your Current Library's Stats" << endl;
    cout << "----------------------------"  << endl;
    cout << "Total Books: " << length << endl;
    cout << "   In Library: "<< inLibrary <<endl;
    cout << "   Checked Out: " << checked << endl;
    cout << "   On Loan: " << loaned << endl;
    cout << "   Unknown: " << unknown << endl;
    cout << "   Other: " << other << endl;
}

///  print out the main menu of the management system
void printMenu()
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}
int main()
{
    
    string command;
    vector <string> entries;
    vector <string> emptyLibrary;


    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";
        
        getline(cin, command);
        cout << endl;


        if (command == "A" || command =="a"){
           entries.push_back(addEntry());
        }
        else if (command == "C" || command =="c"){
            entries = emptyLibrary;
            cout << "Your library is now empty." << endl;
        }
        else if (command == "D" || command =="d"){
            displayCurLibrary(entries);
        }
        else if (command == "I" || command =="i"){
            invalidEntries(entries);
        }
        else if (command == "L" || command =="l"){
            entries = loadFromData(entries);
        }
        else if (command == "O" || command =="o"){
            outToFile(entries);
        }
        else if (command == "P" || command =="p"){
            checkoutStats(entries);
        }
        else if (command == "R" || command =="r"){
            entries = removeEntry(entries);
        }
        else if (command == "S"|| command =="s"){
            searchEntry(entries);
        }

        cout << endl;
    } while (!(command == "x" || command == "X"));
    return 0;
}