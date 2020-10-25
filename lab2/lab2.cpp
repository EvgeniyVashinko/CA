#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Timer
{
public:
	Timer()
	{
		start = chrono::steady_clock::now();
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
const int ci = 1000;
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


int main()
{
	int* arr = new int[ci] {};
	Start(4, arr, Inc2);
	cout << "ready\n";
	for (int i = 0; i < ci; i++)
	{
		cout << arr[i] << " ";
		if (arr[i] != 1)
		{
			cout << "mistake!";
			//break;
		}

	}
}
