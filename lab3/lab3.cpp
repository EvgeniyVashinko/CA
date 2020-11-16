#include <iostream>
#include <omp.h>
#include <ctime>
#include <thread>
#include <vector>
using namespace std;

int SIZE = 99999999;
int* a = new int[SIZE];
int* b = new int[SIZE];
int result = 0;

int main()
{
	chrono::time_point<chrono::steady_clock> time1, time2;
	srand(time(0));

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = rand() % 100;
		b[i] = rand() % 100;
	}

	int i = 0, testRes = 0;;

	time1 = chrono::steady_clock::now();

	for (int i = 0; i < SIZE; i++)
	{
		testRes += a[i] * b[i];
	}

	time2 = chrono::steady_clock::now();

	cout << "TestRes = "<< testRes << endl;
	cout << "time1 = "<< ((chrono::duration<float>)(time2 - time1)).count() << endl;

	time1 = chrono::steady_clock::now();

#pragma omp parallel for private(i) shared(a,b) schedule(runtime) reduction(+:result)

	for (i = 0; i < SIZE; i++)
	{
		result += a[i] * b[i];
	}

	time2 = chrono::steady_clock::now();

	cout << "res = " << result << endl;
	cout << "time2 = " << ((chrono::duration<float>)(time2 - time1)).count() << endl;
	cout << "result == testRes --- " << (bool)(result == testRes);
}
