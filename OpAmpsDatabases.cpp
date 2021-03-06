/*																								Aleksander Dosti - 12/02/2019
*/

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

// the length of the fixed array to be used for database - must be at least one
#define DATABASE_MAX 10

// file used for the database
#define DATABASE_FILENAME "database.txt"

// format of Class of type OpAmp
class OpAmps
{
public:	
	OpAmps() { cout << "Constructing base objects\n"; }
	~OpAmps();  //Destructor function for OpAmps class objects
	void AddElement();
	void Display() const;
	char* element_n() 
	{ return Name; } //Getter function
	double element_sr()
	{ return SlewRate; };   //Getter function
	friend ostream& operator << (ostream& out, OpAmps &Op);
	friend ifstream& operator >> (ifstream& in, OpAmps &Op);

protected:   // Database_memory class will inherit these private members
	char Name[20];
	unsigned int PinCount;
	double SlewRate;

};


//Definition of class OpAmpsContainers
class Database_Memory : public OpAmps {
public:
	Database_Memory() { cout << "Constructing derived objects\n"; }
	void Enter();
	void DisplayElements() const;
	void Save();
	void Load();
	void Sort();
	~Database_Memory(); //Destructor function for Database_memory objects
	unsigned long database_length = 0;

private:
	OpAmps OpAmp[DATABASE_MAX]; // an array of 10 OpAmp objects
	static int CompareName(const void*, const void*);
	static int CompareSlewRate(const void*, const void*);
	
};


/*
 Enter function checks if database has reached capacity, in which case it will return to menu,
 else AddElement is called. Database_length is inherited from the opamps class and is incremented 
 with each new entry.

 The arguments for Enter database length, the OpAmp Database (array of OpAmp objects).

Enter function is of type void and so will not return a value.
 */

void Database_Memory::Enter()
{

	if (database_length == DATABASE_MAX)
	{
		cout << "The database is full" << endl;
		return;
	}
	// else if database is not full, get the data from the user and alter the OpAmp database length
	else
	{
		cout << "Enter database item number: " << database_length + 1 << endl;
		OpAmp[database_length].AddElement();  //Run the OpAmp class Enter function
		database_length++;
	}
}



OpAmps :: ~OpAmps()
{
	cout << "Destructing Base Objects... " << endl;
}

Database_Memory :: ~Database_Memory()
{
	cout << "Destructing Database Objects..." << endl;
}
/*
Add element is used to add properties of the OpAmp to the database
,including OpAmp name, pincount and slew rate.

It's arguments include protected class object members, Name, Pincount and SlewRate.

Function is of type void and so it will not return a value.

 */

void OpAmps::AddElement()
{
	cout << "Enter op-amp name: ";
	cin >> Name;
	cout << "Enter number of pins: ";
	cin >> PinCount;
	cout << "Enter slew rate: ";
	cin >> SlewRate;
	cout << endl;
}


/*
Function DisplayElements is used to display elements of the database, once read from file or prior to saving.
It calls the display function, which is a member function of the OpAmps class, using the private objects inherited,
to incrementally display elements currently in memory.

The function is of type void and will not return a value.
*/

void Database_Memory::DisplayElements() const
{

	if (database_length == 0) // if the database is empty, generate message informing user.
	{
		cout << "Database is empty, no  items could be displayed" << endl;
		return;
	}
	// if the database is not full, Display all the data in the OpAmp database
	else
	{
		for (unsigned int i = 0; i < database_length; i++)
		{
			cout << "Database element number: " << i + 1 << ")" << endl << endl;
			OpAmp[i].Display(); //Run the OpAmp class Display function for every single OpAmp object
			cout << " " << endl;
		}
	}
}

void OpAmps::Display() const
{
	cout << " OpAmp Name =  " << Name;
	cout << ", OpAmp PinCount =  " << PinCount;
	cout << ", OpAmp SlewRate =  " << SlewRate << " V/s" << endl;
}


ifstream& operator >> (ifstream& in, OpAmps &opamp)
{
	in >> opamp.Name;
	in >> opamp.PinCount;
	in >> opamp.SlewRate;

	return in;
}


ostream& operator << (ostream& out, OpAmps &opamp)
{
	out << opamp.Name << endl;
	out << opamp.PinCount << endl;
	out << opamp.SlewRate << endl << endl;

	return out;
}

/*
The save function writes data from memory into the .txt file.

The function returns no value as a result of it being declared as type void.
*/
void Database_Memory::Save()
{
	fstream output_file;  // file stream for output    // open the file
	output_file.open(DATABASE_FILENAME, ios::out);    // write length information to file

	output_file << database_length << endl << endl;    // write data to file

	for (unsigned long i = 0; i < database_length; i++)
	{
		output_file << OpAmp[i];    // Read from memory into file
	}

	cout << "Database has been saved into .txt file" << endl;
	output_file.close();  // close file
}

/*
This function will be used to load the saved database.txt file and to
read the values from it.

Arguments of this function are the OpAmp database (array of OpAmp objects) which we use
to assign the values of input_file stream into the private class members.
The database length which is written to the file.

The function returns no value as a result of it being declared as type void.
*/

void Database_Memory::Load()
{
	ifstream input_file;  // file stream for input
	input_file.open(DATABASE_FILENAME, ios::in); // open the file

	if (!input_file.good()) {				// if file could not be opened

		cerr << "Error: " << DATABASE_FILENAME << " could not be found in local directory!" << endl;
		return;
	}

	input_file >> database_length;    // load length information from first line of file

	if (database_length == 0) //If file is empty, return to menu
	{
		cout << "No existing database to load, returning to menu" << endl;
		return;
	}
	else // Load data from file incrementally 
	{
		for (unsigned long i = 0; i < database_length; i++)
		{
			input_file >> OpAmp[i]; 
		}
		cout << "Database has been succesfully loaded" << endl;
		input_file.close(); // close the file
	}
}

/*
This function will be used to sort the OpAmp database according to the users choice.

Arguments of this function are the OpAmp database, length of the database, and the
OpAmp class private members which consequently will be used in SortName and SortSlewrate
*/

void Database_Memory::Sort()
{
	char UserInput;    // show the menu of options
	cout << endl;
	cout << "Sorting options" << endl;
	cout << "---------------" << endl;
	cout << "1. To sort by name" << endl;
	cout << "2. To sort by slew rate" << endl;
	cout << "3. No sorting" << endl << endl;
	cout << "Enter your option: ";
	cin >> UserInput;    // get the user's choice of sorting operation required
	cout << endl;        // act on the user's input
	switch (UserInput)
	{
	case '1':
		qsort(OpAmp, database_length, sizeof(OpAmps), CompareName); // sort database in alphabetical order
		cout << "Elements have been sorted by Name in alphabetical order" << endl;
		break;
	case '2':
		qsort(OpAmp, database_length, sizeof(OpAmps), CompareSlewRate); // Sort database in ascending order of slew rates
		cout << "Elements have been sorted by Slew Rate in ascending order" << endl;
		break;
	case '3':   // Exit case for if user no longer wishes to sort database
		return;
	default:
		cout << "Invalid entry" << endl << endl;
		break;
	}
}
/*
This compare function will be used to sort the data alphabetically
according to the OpAmp Names.

Arguments of this function include two variables of type void, which will be used to
compare elements in terms of size.

Function returns an integer to be used in q-sort function.
*/

int Database_Memory::CompareName(const void *a, const void* b)
{
	return strcmp(((OpAmps*)a)->element_n(), ((OpAmps*)b)->element_n());
}

/*
This function will be used to sort the data in ascending order
according to the OpAmp Slew Rates.

Arguments of this function include two variables of type void, which will be used to
compare elements in terms of size.

Function returns an integer to be used in q-sort function.
*/

int Database_Memory::CompareSlewRate(const void *OpAmpSR1, const void* OpAmpSR2)
{
	return (int)((((OpAmps*)OpAmpSR1)->element_sr() - ((OpAmps*)OpAmpSR2)->element_sr()));
}





/*
This is the main method of the program

In main method we will make class objects and then call the subsequent functions.

*/

int main()
{
	char UserInput;                          // loop until the user wishes to exit
	Database_Memory database;

	while (1)
	{          // show the menu of options
		cout << endl;
		cout << "Op-amp database menu" << endl;
		cout << "--------------------" << endl;
		cout << "1. Enter a new op-amp into the database" << endl;
		cout << "2. Save the database to disk" << endl;
		cout << "3. Load the database from disk" << endl;
		cout << "4. Sort the database" << endl;
		cout << "5. Display the database" << endl;
		cout << "6. Exit from the program" << endl << endl;
		cout << "Enter your option: ";
		cin >> UserInput;
		cout << endl;          // act on the user's input
		switch (UserInput)
		{
		case '1':
			database.Enter();
			break;
		case '2':
			database.Save();
			break;
		case '3':
			database.Load();
			break;
		case '4':
			database.Sort();
			break;
		case '5':
			database.DisplayElements();
			break;
		case '6':
			return 0;
		default:
			cout << "Invalid entry" << endl << endl;
			break;
		}
	}
}
















