#include <iostream>
#include <thread>
#include <mutex>
#include "Queue.h"
#include "ConstSizeQueue.h"
#include <vector>
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
int taskNum = 1024;
int lock_ = 0, threadNum = 1;
int cNum = 1, pNum = 1;

// FIXME remove array access from critical section
void Inc1(vector<int>& arr) {
	int temp = 0;
	while(true)
	{
		m.lock();
		temp = lock_;
		lock_++;
		m.unlock();
		if (temp >= taskNum)
		{
			break;
		}
		arr.at(temp)++;

		//this_thread::sleep_for(chrono::milliseconds(10));
	}
}


atomic<int> ai{0};
// FIXME subsitute int* with std::vector<int> and operator [] with int at();
void Inc2(vector<int>& arr) {
	int temp = 0;
	while (true)
	{
		temp = ai.fetch_add(1);
		if (temp >= taskNum)
		{
			break;
		}
		arr.at(temp)++;
		//this_thread::sleep_for(chrono::milliseconds(10));
	}
}


void Start(int thrNum, vector<int> &arr, void f(vector<int> &arr)) {
	Timer t;
	thread* thr = new thread[thrNum];
	for (int i = 0; i < thrNum; i++)
	{
		thr[i] = thread([&]() {f(arr); });
	}
	for (int i = 0; i < thrNum; i++)
	{
		thr[i].join();
	}
}

void Task1(int thrNum, void f(vector<int>& arr))
{
	taskNum = 1024*1024;
	lock_ = 0;
	ai = 0;
	threadNum = thrNum;
	vector<int> arr(taskNum);
	Start(thrNum, arr, f); // FIXME Inc2 or Inc1 shall be an arg
	cout <<"  --  " << thrNum <<  " threads ready\n";
	for (int i = 0; i < taskNum; i++)
	{
		//cout << arr[i] << " ";
		if (arr[i] != 1)
		{
			cout << "mistake!";
			break;
		}

	}
}


template<typename T>
void Producer(int TaskNum, T& q)
{
	Timer t;
	for (int i = 0; i < TaskNum; i++)
	{
		//this_thread::sleep_for(chrono::milliseconds(1));
		q.push(1);
	}
	lock_guard<mutex> lg(m);
	cout << "\np";
}

template<typename T>
void Consumer(int TaskNum, T& q, int& total)
{
	Timer t;
	int value = 0;
	while (true)
	{

		//this_thread::sleep_for(chrono::milliseconds(4));
		if (q.pop(value))
		{
			lock_guard<mutex> lg(m);
			total += value;
		}
		if (total >= TaskNum * pNum) {
			break;
		}
	}
	lock_guard<mutex> lg(m);
	cout << "\nc";
}

void Task21(int taskNum, int consumerNum, int producerNum) 
{
	float producerTime = 0, consumerTime = 0;
	int total = 0;
	cNum = consumerNum;
	pNum = producerNum;
	SafeQueue<int> q;
	thread* thr = new thread[producerNum + consumerNum];
	Timer t;

	for (int i = producerNum; i < producerNum + consumerNum; i++)
	{
		thr[i] = thread([&]() {Consumer(taskNum, q, total); });
	}
	for (int i = 0; i < producerNum; i++)
	{
		thr[i] = thread([&]() {Producer(taskNum, q); });
	}
	for (int i = 0; i < producerNum + consumerNum; i++)
	{
		thr[i].join();
	}
	cout << endl << "total: "<<total;
}


void Task22(int taskNum, int consumerNum, int producerNum, int qSize)
{
	float producerTime = 0, consumerTime = 0;
	int total = 0;
	cNum = consumerNum;
	pNum = producerNum;
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
	////T1
	Task1(4, Inc2);
	Task1(8, Inc2);
	Task1(16, Inc2);
	Task1(32, Inc2);
	
	////T2_1
	//Task21(1024 * 100, 1, 1);
	//cout << endl << "__________________";
	//Task21(1024 * 100, 2, 2);
	//cout << endl << "__________________";
	//Task21(1024*100, 4, 4);
	//cout << endl << "__________________";

	////T2_2
	//Task22(1024 * 1024, 1, 1, 4);
	//cout << endl << "__________________";
	//Task22(1024 * 1024, 1, 1, 8);
	//cout << endl << "__________________";
	//Task22(1024 * 1024, 1, 1, 16);
	//cout << endl << "__________________";
	//Task22(1024 * 1024, 2, 2, 16);
	//cout << endl << "__________________";
	//Task22(1024*1024 , 4, 4, 16);
	//cout << endl << "__________________";

}
