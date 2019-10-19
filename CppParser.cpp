/*
This piece of code was written for a previous job application. In the email, they provided three .cpp files and asked us to write
some code to parse the given files and print out a hierarchy tree. Overall, I am proud that I was able to complete this task
within a few days. I think the part I am most proud of is that I created my own struct variable node, which I was able to use as a
template when printing out the hierarchy tree. 

Assumptions:
1. There are only 3 files given as input
	- The problem only provided 3 input files.
2. Input files are located in the same folder as the program executable files.
	- I created the solution on Windows, functions for locating a file path may be different on Linux.
3. Code begins with the main() function
	- C++ requires main() as the first function
4. There are no '(', '{', or '}' characters used outside of a function definiton or within a comment.
	- Any of those characters may cause the program to crash or not function properly. The comments can be solved with additional code to ignore any code that is commented out
5. There are no recursive functions.
	- As there were no recursive functions given in the input files, i created a recursive function that would create an infinite loop if any input functions would call itself.


This code will parse 3 source files and print a hierarchy tree of all functions called from within main.cpp::main(). 
*/

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string file1 = "StandardHelper.hpp";
string file2 = "main.cpp";
string file3 = "HelloWorldHelper.hpp";

```
struct node {
	string functionName;
	string fileName;
	vector<node> calledFunctions;
};
```

void findFunctions(string file, node& allFunctions);
void scanFunction(string file, node& allFunctions);
void bracketCounter(int& bracket, bool& functionFound, char c);
void printHierarchy(node allFunctions);
void printCalled(node callingFunction, int indent);

void main()
{
	node allFunctions;
	allFunctions.functionName = "main"; // first function is main()
	allFunctions.fileName = "main.cpp";

	findFunctions(file1, allFunctions); // find all function names in all 3 files
	findFunctions(file2, allFunctions);
	findFunctions(file3, allFunctions);

	scanFunction(file1, allFunctions); // look through each function for calls to other functions
	scanFunction(file2, allFunctions);
	scanFunction(file3, allFunctions);

	printHierarchy(allFunctions);
	return;
}

//This function scans the given file word by word and stores all function names into the node allFunctions
void findFunctions(string file, node& allFunctions)
{
	string data;
	string lastStr;
	string name;

	int bracket = 0;
	bool functionFound = false; 

	ifstream inFile;
	inFile.open(file);
	if (!inFile) {
		cerr << "Unable to open file";
		exit(1);   // call system to stop
	}

	while (!inFile.eof())
	{
		inFile >> data;

		if (functionFound == true) // Once a function is found, only check for '{' or '}' for indication of the end of the function.
		{
			for (int i = 0; i <= data.size(); i++) 
			{
				bracketCounter(bracket, functionFound, data[i]);
			}

		}
		else // check for '(' to indicate the start of a function name
		{
			name = "";
			bool found = false;
			for (int i = 0; i <= data.size(); i++)
			{
				if (found == false)
				{

					if (data[i] == '(') // All function names have '(' in the definition.
					{
						if (i == 0) // if there are spaces between the function name and (), the last string is the function name.
						{
							name = lastStr;
						}
						found = true;

						node temp;
						temp.functionName = name;
						temp.fileName = file;
						allFunctions.calledFunctions.push_back(temp);

						continue;
					}
					name = name + data[i];  //name stores the characters in the word up to the '('.
				}
				if (data[i] == '{')
				{
					bracket++;
					functionFound = true;
				}

			}

			lastStr = data; // store previous string as function name if next string contains '(' 
		}

	}

	inFile.close();
	return;
}


//This function scans each file for all function calls within each function. 
void scanFunction(string file, node& allFunctions)
{
	string data;
	string lastStr;
	string name;
	int index = 0;

	int bracket = 0;
	bool functionFound = false;

	ifstream inFile;
	inFile.open(file);
	if (!inFile) {
		cerr << "Unable to open file";
		exit(1);   // call system to stop
	}
	while (!inFile.eof())
	{
		inFile >> data;

		if (functionFound == false) // when function not found, scan for brackets and find the function name
		{
			name = "";
			bool found = false;
			for (int i = 0; i <= data.size(); i++)
			{
				if (found == false)
				{

					if (data[i] == '(') // if there are spaces between the function name and (), the last string is the function name.
					{
						if (i == 0)
						{
							name = lastStr;
						}
						found = true;

						for (int i = 0; i < allFunctions.calledFunctions.size(); i++) // find index of calling function in function list
						{
							if (name == allFunctions.calledFunctions[i].functionName)
							{
								index = i; 
								break;
							}

						}

						continue;
					}
					name = name + data[i];
				}

				bracketCounter(bracket, functionFound, data[i]);

			}

			lastStr = data; // store previous string as function name if next string contains '(' 
		}
		else // scan for function calls and store called functions 
		{

			for (int i = 0; i < allFunctions.calledFunctions.size(); i++)
			{
				int loc = data.find(allFunctions.calledFunctions[i].functionName);	

				int len = allFunctions.calledFunctions[i].functionName.size();

				if (loc == 0) // ensures first character of data is the start of the keyword in question.
				{
					char next = data.at(loc + len);
					if (next == '(' || next == ' ') // determine if this is a function or another string that happens to contain the name( i.e. str might be falsley confirmed when the program reads in string)
					{
						node temp;
						temp.functionName = allFunctions.calledFunctions[i].functionName;
						temp.fileName = allFunctions.calledFunctions[i].fileName;
						allFunctions.calledFunctions[index].calledFunctions.push_back(temp);
						break;
						cout << "test1" << endl;
					}
				}

			}
			for (int i = 0; i <= data.size(); i++)
			{
				bracketCounter(bracket, functionFound, data[i]);
			}

		}

	}
	return;
}


//This function counts the number of '{' and '}' included in the code to determine the beginning and end of a function definition.
void bracketCounter(int&bracket, bool& functionFound, char c)
{
	if (bracket == 0)
	{
		functionFound = false;
	}
	else
	{
		functionFound = true;
	}
	if (c == '{')
	{
		bracket++;
	}
	if (c == '}')
	{
		bracket--;
	}
}

// This function sorts the node allFunctions into another node set up in the format of the hierarchy tree.
void printHierarchy(node allFunctions)
{
	string name;
	string file;
	node hierarchyTree;
	node callingFunction;
	int mainIndex;



	//set up functions in hierarchy format
	for (int i = 0; i < allFunctions.calledFunctions.size(); i++)
	{
		 name = allFunctions.calledFunctions[i].functionName;
		 file = allFunctions.calledFunctions[i].fileName;
		 if (name == "main") // assume main is first function, print out main first. 
		 {
			 mainIndex = i;

			 hierarchyTree.functionName = name;
			 hierarchyTree.fileName = file;

			 cout << name << " -- " << file << endl;  

		 }
	}

	//At this point, hierarchyTree has main, need to add the other functions from allFunctions to include called functions within them

	for (int i = 0; i < allFunctions.calledFunctions[mainIndex].calledFunctions.size(); i++)
	{
		for (int j = 0; j < allFunctions.calledFunctions.size(); j++)
		{
			if (allFunctions.calledFunctions[mainIndex].calledFunctions[i].functionName == allFunctions.calledFunctions[j].functionName)
			{
				hierarchyTree.calledFunctions.push_back(allFunctions.calledFunctions[j]);
			}
		}
	}

	printCalled(hierarchyTree, 0);
}


// This function prints a function and checks within the function for other function calls. If calls are found, it will call itself and print the functions called. 
void printCalled(node callingFunction, int indent)
{
	for (int i = 0; i < callingFunction.calledFunctions.size(); i++)
	{
		string name = callingFunction.calledFunctions[i].functionName;
		string file = callingFunction.calledFunctions[i].fileName;
		string spaces = "     ";

		if (indent == 0)
		{
			spaces = "";
		}
		for (int j = 1; j < indent; j++)
		{
			spaces = spaces + spaces;
		}

		cout << spaces <<  "|-- " << name << " -- " << file << endl;

		if (callingFunction.calledFunctions[i].calledFunctions.size() != 0)
		{
			indent++;
		    printCalled(callingFunction.calledFunctions[i], indent);
			indent--;
		}
	}
}
