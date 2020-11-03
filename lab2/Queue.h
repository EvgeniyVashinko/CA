#include <mutex>


template<typename T>
class Queue {
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
public:
	Queue() {
		Head = nullptr;
		Tail = nullptr;
		size = 0;
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
			Node<T>* current = this->Head;
			Head = new Node<T>(data, Head);
			current->prew = Head;
			size++;
		}
	}

	T Pop() {
		if (size == 0) return 0;
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

	int Size() { return size; }
};


template<class T>
class SafeQueue{
private:
	std::mutex mtx;
	Queue<T> q;
public:
	void push(T value) {
		std::lock_guard<std::mutex> g(mtx);
		q.Push(value);
	}
	bool pop(T& value) {
		std::unique_lock<std::mutex> g(mtx);
		if (q.IsEmpty())
		{
			//g.unlock();
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			//g.lock();
			if (q.IsEmpty())
			{
				return false;
			}
		}
		value = q.Pop();
		return true;
	}
};





