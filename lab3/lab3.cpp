#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

const int SIZE = 10000;
int* a = new int[SIZE];
int* b = new int[SIZE];
int result = 0;
int main()
{
	srand(time(0));
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = rand() % 100;
		b[i] = rand() % 100;
	}
	int i = 0, testRes = 0;;

	for (int i = 0; i < SIZE; i++)
	{
		testRes += a[i] * b[i];
	}
	cout << "TestRes = "<< testRes << endl;


#pragma omp parallel for private(i) shared(a,b) reduction(+:result)

	for (i = 0; i < SIZE; i++)
	{
		result += a[i] * b[i];
	}

	cout << "res = " << result << endl << (bool)(result == testRes);
}
