#include <mutex>


template<typename T>
class ConstSizeQueue {
protected:
	template <typename T>
	class Node {
	public:
		T data;
		Node* next;
		Node* prew;
		Node(T data = T(), Node* next = nullptr, Node* prew = nullptr) {
			this->data = data;
			this->next = next;
			this->prew = prew;
		}
	};
	Node<T>* Head;
	Node<T>* Tail;
	int size;
	int maxSize;
public:
	ConstSizeQueue() {
		Head = nullptr;
		Tail = nullptr;
		size = 0;
		maxSize = 16;
	}
	ConstSizeQueue(int maxSize) {
		Head = nullptr;
		Tail = nullptr;
		size = 0;
		this->maxSize = maxSize;
	}
	void Push(T data) {
		if (Head == nullptr)
		{
			Head = new Node<T>(data);
			Tail = Head;
			size++;
		}
		else
		{
			if (IsFull())
			{
				return;
			}
			Node<T>* current = this->Head;
			Head = new Node<T>(data, Head);
			current->prew = Head;
			size++;
		}
	}

	T Pop() {
		if (size == 0) return -1;
		Node<T>* temp = this->Tail;
		if (size == 1) {
			Head = nullptr;
			Tail = nullptr;
		}
		else {
			Tail = Tail->prew;
			Tail->next = nullptr;
		}
		T val = temp->data;
		delete temp;
		size--;
		return val;
	}

	void Clear() {
		while (size)
		{
			Pop();
		}
	}

	bool IsEmpty() {
		if (size == 0) {
			return true;
		}
		else return false;
	}

	bool IsFull() {
		return maxSize == size;
	}

	int Size() { return size; }

	void setSize(int s) {
		this->maxSize = s;
	}
};


template<class T>
class SafeConstSizeQueue {
private:
	std::mutex mtx;
	ConstSizeQueue<T> q;
	std::condition_variable cv;
public:
	SafeConstSizeQueue() {}
	SafeConstSizeQueue(int size) {
		q.setSize(size);
	}
	void push(T value) {
		std::unique_lock<std::mutex> g(mtx);
		if (q.IsFull())
		{
			cv.wait(g, [&]() {return !q.IsFull(); });
			q.Push(value);
		}
		else { q.Push(value); }
	}
	bool pop(T& value) {
		std::unique_lock<std::mutex> g(mtx);
		if (q.IsEmpty())
		{
			g.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			g.lock();
			if (q.IsEmpty())
			{
				cv.notify_one();
				return false;
			}
			else
			{
				value = q.Pop();
				cv.notify_one();
				return true;
			}
		}
		else
		{
			value = q.Pop();
			cv.notify_one();
			return true;
		}
	}
};





