#include <iostream>
#include <thread>
#include <mutex>
#include "Queue.h"
#include "ConstSizeQueue.h"
using namespace std;

class Timer
{
public:
	Timer()
	{
		start = chrono::steady_clock::now();
	}
	
	float Get()
	{
		end = std::chrono::steady_clock::now();
		chrono::duration<float> time = end - start;
		return time.count();
	}
	~Timer() {
		end = std::chrono::steady_clock::now();
		chrono::duration<float> time = end - start;
		cout << endl << "Time " << time.count();
	}

private:
	chrono::time_point<chrono::steady_clock> start, end;
};
mutex m;
int ci = 1000;
int lock_ = 0;

void Inc1(int* arr) {
	while (lock_ < ci)
	{
		m.lock();
		arr[lock_]++;
		lock_++;
		m.unlock();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}


atomic<int> ai{0};
void Inc2(int* arr) {
	while (ai.load() < ci)
	{
		arr[ai.fetch_add(1)]++;
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}


void Start(int thrNum, int *arr, void f(int* arr)) {
	Timer t;
	thread* thr = new thread[thrNum];
	for (int i = 0; i < thrNum; i++)
	{
		thr[i] = thread(f, arr);
	}
	for (int i = 0; i < thrNum; i++)
	{
		thr[i].join();
	}
}

void Task1(int num)
{

}

//Task 2.1//

void Producer(int TaskNum, SafeQueue<int> &q)
{
	Timer t;
	for (int i = 0; i < TaskNum; i++)
	{
		//this_thread::sleep_for(chrono::milliseconds(1));
		q.push(1);
	}
	cout << "\np";
}
void Consumer(int TaskNum, SafeQueue<int> &q, int &total)
{
	Timer t;
	int value;
	while (true)
	{
		//this_thread::sleep_for(chrono::milliseconds(4));
		if (!q.pop(value)) {
			break;
		}
		lock_guard<mutex> lg(m);
		total += value;
	}
	cout << "\nc";
}

void Task21(int taskNum, int consumerNum, int producerNum) 
{
	float producerTime = 0, consumerTime = 0;
	int total = 0;
	SafeQueue<int> q;
	thread* thr = new thread[producerNum + consumerNum];
	Timer t;
	for (int i = 0; i < producerNum; i++)
	{
		thr[i] = thread([&]() {Producer(taskNum, q); });
	}

	for (int i = producerNum; i < producerNum + consumerNum; i++)
	{
		thr[i] = thread([&]() {Consumer(taskNum, q, total); });
	}
	for (int i = 0; i < producerNum + consumerNum; i++)
	{
		thr[i].join();
	}
	cout << endl << "total: "<<total;
}

void Producer(int TaskNum, SafeConstSizeQueue<int>& q)
{
	Timer t;
	for (int i = 0; i < TaskNum; i++)
	{
		//this_thread::sleep_for(chrono::milliseconds(1));
		q.push(1);
	}
	cout << "\np";
}
void Consumer(int TaskNum, SafeConstSizeQueue<int>& q, int& total)
{
	Timer t;
	int value;
	while (true)
	{
		//this_thread::sleep_for(chrono::milliseconds(4));
		if (!q.pop(value)) {
			break;
		}
		lock_guard<mutex> lg(m);
		total += value;
	}
	cout << "\nc";
}

void Task22(int taskNum, int consumerNum, int producerNum, int qSize)
{
	float producerTime = 0, consumerTime = 0;
	int total = 0;
	SafeConstSizeQueue<int> q(qSize);
	thread* thr = new thread[producerNum + consumerNum];
	Timer t;
	for (int i = 0; i < producerNum; i++)
	{
		thr[i] = thread([&]() {Producer(taskNum, q); });
	}

	for (int i = producerNum; i < producerNum + consumerNum; i++)
	{
		thr[i] = thread([&]() {Consumer(taskNum, q, total); });
	}
	for (int i = 0; i < producerNum + consumerNum; i++)
	{
		thr[i].join();
	}
	cout << endl << "total: " << total;
}


int main()
{
	//int* arr = new int[ci] {};
	//Start(4, arr, Inc2);
	//cout << "ready\n";
	//for (int i = 0; i < ci; i++)
	//{
	//	cout << arr[i] << " ";
	//	if (arr[i] != 1)
	//	{
	//		cout << "mistake!";
	//		//break;
	//	}

	//}

	//Task21(100*100, 8, 4);
	Task22(100 * 100, 8, 4, 16);

}
