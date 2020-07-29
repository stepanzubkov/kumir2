#ifndef STACK_HPP
#define STACK_HPP

#include <cstdlib>
#include <vector>

namespace VM
{

template <class T> class Stack
{
public:
	void push(const T &t)
	{
		currentIndex_ ++;
		if (currentIndex_ >= (int)data_.size()) {
			data_.resize(data_.size() + deltaSize_);
		}
		data_[currentIndex_] = t;
	}

	T pop()
	{
		currentIndex_--;
		return data_[currentIndex_ + 1];
	}

	T &top()
	{
		return data_[currentIndex_];
	}

	const T &top() const
	{
		return data_[currentIndex_];
	}

	T &at(int index)
	{
		return data_[index];
	}

	const T &at(int index) const
	{
		return data_[index];
	}

	int size() const
	{
		return currentIndex_ + 1;
	}

	int reservedSize() const
	{
		return data_.size();
	}

	void reset()
	{
		data_ = std::vector<T>(initialSize_);
		currentIndex_ = -1;
	}

	Stack()
	{
		initialSize_ = deltaSize_ = 100;
		currentIndex_ = 0;
		data_ = std::vector<T>(100);
	}

	~Stack()
	{
		data_.clear();
	}

private:
	int initialSize_;
	int deltaSize_;
	int currentIndex_;
	std::vector<T> data_;
};

}

#endif
