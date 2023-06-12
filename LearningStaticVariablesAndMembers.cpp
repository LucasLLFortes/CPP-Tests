// LearningStaticVariablesAndMembers
// compile With:
// g++ -Wall -pthread -std=c++11 -o LearningStaticVariablesAndMembers LearningStaticVariablesAndMembers.cpp

// C++ program to demonstrate
// the use of static Static
// variables in a Function
#include <iostream>
#include <string>
using namespace std;

static int count_global = 0;

void demo()
{
	// static variable
	static int count = 5;
	
	// value is updated and
	// will be carried to next
	// function calls
	count++;
	count_global++;
	cout << count << " - " << count_global << "\n";
}

void demo2()
{
	// static variable
	static int count = 0;
	
    // value is updated bu 
	// will be carried to next
	// function calls
	count += 10;
	count_global += 10;

	cout << count << " - " << count_global << "\n";
	
}

int main()
{
	for (int i=0; i<8; i++)	
		demo();
    cout << "\n";
    
    for (int i=0; i<8; i++)	
        demo2();
    cout << "\n";

	return 0;
}
