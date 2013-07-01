#include<iostream>
using namespace std;
int main()
{

	int test1,test2;
	test1 = 1;
	test2 = 1;

	test2 <<= 1;
	
	cout << "test1 = " << test1 << "\n" << "test2 = " << test2;
	cin.get();
	return 0;
}