#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
 
using namespace std;
 
const int MAXBOOK = 7;
 
class Book 
{
    private:
        string title, author, publisher, dop;
        int colour;    
        
    public:
        Book();
        Book(string, string, string, string, int);
        void setTitle(string);
        void setAuthor(string);
        void setDOP(string);
        void setPublisher(string);
        void setColour(int);
        string getTitle() const;
        string getAuthor() const;
        string getDOP() const;
        string getPublisher() const;
        int getColour() const;
        
};

Book:: Book()
{
    title = "";
    author = "";
    dop = "";
    publisher = "";
    colour = 0;
}
 
Book:: Book(string title0, string author0, string dop0, string publisher0, int colour0)
{
    title = title0;
    author = author0;
    dop = dop0;
    publisher = publisher0;
    colour = colour0;
}
 
void Book:: setTitle(string title0)
{
    title = title0;
}
 
void Book:: setAuthor(string author0)
{
    author = author0;
}
 
void Book:: setDOP(string dop0)
{
    dop = dop0;
}
 
void Book:: setPublisher(string publisher0)
{
    publisher = publisher0;
}
 
void Book:: setColour(int colour0)
{
    colour = colour0;
}
 
string Book:: getTitle() const
{
    return title;
}        
 
string Book:: getAuthor() const
{
    return author;
}
 
string Book:: getDOP() const
{
    return dop;    
} 
 
string Book:: getPublisher() const
{
    return publisher;
}
 
int Book:: getColour() const
{
    return colour;
}
 

// Functions
 
string removeUnderscores(string text)
{
    int length = text.size();
    string fixedText;
    for (int count = 0; count < length; count++)
    {
        if (text[count] == '_')
        {
            fixedText = fixedText + ' ';
        }
        
        else
        {
            fixedText = fixedText + text[count];
        }
    }
    return fixedText;
}

string addUnderscores(string text)
{
	int length = text.size();
	string fixedText;
	for (int count = 0; count < length; count++)
	{
		if (text[count] == ' ')
		{
			fixedText = fixedText + '_';
		}
		
		else
		{
			fixedText = fixedText + text[count];
		}
	}
	return fixedText;
}
 

int storeBooks(ifstream & inputFile, Book catalogue[])
{
    string title = "", author = "", publisher = "", dop = "";
    int colour = 0, bookNum = 0;
    while (inputFile >> title >> author >> dop >> publisher >> colour && bookNum < MAXBOOK)
    {
        //store data read from file into the member variables of each book
        catalogue[bookNum].setTitle(removeUnderscores(title));
        catalogue[bookNum].setAuthor(removeUnderscores(author));
        catalogue[bookNum].setDOP(dop);
        catalogue[bookNum].setPublisher(removeUnderscores(publisher));
        catalogue[bookNum].setColour(colour);
        bookNum++;
    }    
    return bookNum; //returns the total number of books in the catalogue
}
 
int search(string searchType, string input, Book catalogue[], int numOfBooks, string & title) 
{
    int colour = -1;
    bool found = false;
 
    for (int index = 0; index < numOfBooks && !found; index++)
    {
        if (searchType == "Author" && catalogue[index].getAuthor() == input 
            || searchType == "Title" && catalogue[index].getTitle() == input 
            || searchType == "DOP" && catalogue[index].getDOP() == input 
            || catalogue[index].getPublisher() == input)
        {
            colour = catalogue[index].getColour();
            title = catalogue[index].getTitle();
            found = true;

            
            // Set member variables of a chosen book to blank strings to prevent reselection 
            catalogue[index].setAuthor("");
            catalogue[index].setTitle("");
            catalogue[index].setDOP("");
            catalogue[index].setPublisher("");
        }
    }
    return colour;
}
 
int main() 
{    
    ifstream fin ("catalogue.txt");
    
    if (!fin)
    {
        cout << "Unable to find file";
        return EXIT_FAILURE;
    }
            
    Book catalogue[MAXBOOK]; //initialization done by constructor
    int numOfBooks = storeBooks(fin, catalogue), numSearches = 0;
    bool nextBook = true;
    ofstream fout ("colour_assignment.txt");
    
    // Search for a book
    while (nextBook)
    {
        // Search type
        string category;
        cout << "Search by (Title, Author, DOP, Publisher): ";
        getline(cin, category);
        
        //keeps on asking for a new input if previous input was invalid
        while (!(category == "Title" || category == "Author" || category == "DOP" || category == "Publisher"))
        {
            cout << "INVALID! Please search by (Title, Author, DOP, Publisher): ";
            getline(cin, category);
        }
 
        // Search input
        string searchInfo;
        cout << "Please enter the information of the book you are looking for: ";
        getline(cin, searchInfo); 
        // getline used instead of cin to allow multiple words to be stored as one variable 
    
        string title = " ";
        int colour = search(category, searchInfo, catalogue, numOfBooks, title);
        
        // Write colours of chosen books to an output file
        if (colour == -1)
        {
            cout << "Cannot find book in catalogue." << endl;
        }
        else
        {
            cout << "You are searching for the book: " << title << endl;
            fout << addUnderscores(title) << setw(8) << colour << endl;
            numSearches++;
        }
 
        // Search for another book?
        string next = "N";
        cout << "Would you like to search for another book? (Y/N): ";
        getline(cin, next);
        if (next == "N")
        {
            nextBook = false;
        }
    }
 
    fin.close();
    fout.close();
    return 0;
}

/*
catalogue.txt

1984 George_Orwell 1949 Harvill_Secker 1
The_Great_Gatsby F._Scott_Fitzgerald 1925 Scribner's 2
The_Kite_Runner Khaled_Hosseini 2003 Riverhead_Books 3
Charlotte's_Web E.B._White 1952 Harper_&_Brothers 4
To_Kill_A_Mockingbird Harper_Lee 1960 J.B._Lippincott_&_Co. 5
The_Catcher_in_the_Rye J.D._Salinger 1951 Little,_Brown_and_Company 6
Pride_and_Prejudice Jane_Austen 1813 Thomas_Egerton 7
*/
