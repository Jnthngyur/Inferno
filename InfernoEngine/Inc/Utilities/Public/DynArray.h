// DynArray.h
// Author: Jonathan Gyurkovics
// Purpose: An array that has the ability to grow and shrink with needs

#ifndef _DYNARRAY_H
#define _DYNARRAY_H

#include "Globals.h"
#include <ostream>

#define END_ITER 0xFFFFFFFF

template<class type>
class DynArray
{
	type* m_pFirst;	// First index in the array
	type* m_pLast;	// Last index in the logical array
	type* m_pEnd;	// Last index in the phyical array

public:
	class iterator
	{
	private:
		type* data;

		iterator(type* data) : data(data) {}
		~iterator() = default;
	public:

		type operator*() const { return *data; }
		type* operator->() const { return data; }
		iterator& operator++()
		{
			data++;
			return *this;
		}
		iterator& operator++(int)
		{
			DynArray<type>::iterator temp = *this;
			data++;
			return temp;
		}
		bool operator==(const iterator& rhs) { return data == rhs.data; }

		friend class DynArray<type>;
	};

	void Copy(type* from, type* to, uint32_t size);
	void ReverseCopy(type* from, type* to, uint32_t size);
	void Init(type* first, uint32_t size);
	void Destroy(type* first, uint32_t size);
	void ClearArray();

public:
	DynArray(uint32_t initialCapacity = 0);
	DynArray(const DynArray<type>& copy);
	~DynArray();

	bool Reserve(uint32_t size);
	void Resize(uint32_t size);

	void push_back(const type& obj);
	void push_front(const type& obj);
	type pop_back();
	type pop_front();

	bool Insert(const type& in, uint32_t index);
	bool Remove(uint32_t index);

	//iterator Begin() const;
	//iterator End() const;

	//iterator Emplace(const type& in, const iterator& place);
	//iterator Erase(const iterator& place);

	uint32_t Size() const;
	uint32_t Capacity() const;

	type& operator[](uint32_t index) const;
	DynArray<type>& operator=(const DynArray<type>& rhs) const;

	template<class type>
	friend std::ostream& operator<<(std::ostream& out, const DynArray<type>& arr);
};

template<class type>
DynArray<type>::DynArray(uint32_t initialCapacity)
{
	if (initialCapacity)
	{
		m_pFirst = (type*)::operator new(initialCapacity * sizeof(type));
		m_pLast = m_pFirst;
		m_pEnd = m_pFirst + initialCapacity;
	}
	else
	{
		m_pFirst = nullptr;
		m_pLast = nullptr;
		m_pEnd = nullptr;
	}
}
template<class type>
DynArray<type>::DynArray(const DynArray<type>& copy)
{
	uint32_t size = copy.Size();
	uint32_t cap = copy.Capacity();

	m_pFirst = (type*)::operator new(initialCapacity * sizeof(type));
	m_pLast = m_pFirst + size;
	m_pEnd = m_pFirst + cap;

	Copy(copy.m_pFirst, m_pFirst, size);
}
template<class type>
DynArray<type>::~DynArray()
{
	ClearArray();
	::delete ((void*)m_pFirst);
}

template<class type>
bool DynArray<type>::Reserve(uint32_t size)
{
	if (size <= Capacity())
		return false;

	if (size == 0)
		size = 1;

	type* temp = (type*)::operator new(size * sizeof(type));

	uint32_t oldSize = Size();
	Copy(m_pFirst, temp, oldSize);

	delete[] m_pFirst;
	m_pFirst = temp;
	m_pLast = m_pFirst + oldSize;
	m_pEnd = m_pFirst + size;

	return true;
}
template<class type>
void DynArray<type>::Resize(uint32_t size)
{
	uint32_t oldSize = Size();
	type* temp = nullptr;

	if (oldSize > size)
	{
		temp = (type*)::operator new(size * sizeof(type));
		Copy(m_pFirst, temp, size);

		delete m_pFirst;
		m_pFirst = temp;
		m_pLast = m_pFirst + size;
		m_pEnd = m_pLast;
	}
	else if (oldSize < size)
	{
		Reserve(size);
		Init(m_pLast, size - oldSize);

		m_pLast = m_pEnd;
	}
}

template<class type>
void DynArray<type>::push_back(const type& obj)
{
	if (m_pLast == m_pEnd)
		Reserve(Capacity() * 2);

	::new ((void *)m_pLast) type(obj);
	m_pLast++;
}
template<class type>
void DynArray<type>::push_front(const type& obj)
{
	if (m_pLast == m_pEnd)
		Reserve(Capacity() * 2);
	
	ReverseCopy(m_pLast - 1, m_pLast, Size());

	/*for (uint32_t i = Size(); i > 0; i--)
		::new ((void *)&m_pFirst[i]) type(m_pFirst[i - 1]);*/

	m_pLast++;
	*m_pFirst = obj;
}
template<class type>
type DynArray<type>::pop_back()
{
	return *--m_pLast;
}
template<class type>
type DynArray<type>::pop_front()
{
	type temp = *m_pFirst;
	
	Copy(m_pFirst + 1, m_pFirst, Size() - 1);

	/*for (uint32_t i = 0, size = Size() - 1; i < size; i++)
		m_pFirst[i] = m_pFirst[i + 1];*/

	m_pLast--;
	return temp;
}

template<class type>
bool DynArray<type>::Insert(const type& in, uint32_t index)
{
	uint32_t size = Size();
	if (index >= size)
		return false;

	if (m_pLast == m_pEnd)
		Reserve(Capacity() * 2);

	ReverseCopy(m_pLast - 1, m_pLast, size - index);

	/*for (uint32_t i = size; i > index; i--)
		m_pFirst[i] = m_pFirst[i - 1];*/

	m_pFirst[index] = in;
	m_pLast++;
	return true;
}
template<class type>
bool DynArray<type>::Remove(uint32_t index)
{
	uint32_t size = Size();
	if (index >= size)
		return false;

	Copy(m_pFirst + (index + 1), m_pFirst + index, (size - 1) - index);

	Destroy(--m_pLast, 1);
	return true;
}

//template<class type>
//DynArray<type>::iterator DynArray<type>::Begin() const
//{
//	return iterator(m_pArray);
//}
//template<class type>
//DynArray<type>::iterator DynArray<type>::End() const
//{
//	return iterator(&m_pArray[m_uiSize]);
//}
//
//template<class type>
//DynArray<type>::iterator DynArray<type>::Emplace(const type& in, const iterator& place)
//{
//	auto End = End();
//	auto Current = place;
//	
//	while (Current != End)
//	{
//		auto Prev = Current++;
//		(*Current) = (*Prev);
//	}
//
//
//}
//template<class type>
//DynArray<type>::iterator DynArray<type>::Erase(const iterator& place)
//{
//
//}

template<class type>
uint32_t DynArray<type>::Size() const { return m_pLast - m_pFirst; }
template<class type>
uint32_t DynArray<type>::Capacity() const { return m_pEnd - m_pFirst; }

template<class type>
type& DynArray<type>::operator[](uint32_t index) const
{
	return m_pFirst[index];
}
template<class type>
DynArray<type>& DynArray<type>::operator=(const DynArray<type>& rhs) const
{
	if (&rhs == this)
		return *this;

	uint32_t size = rhs.Size();
	uint32_t rcap = rhs.Capacity();

	if (Capacity() != rcap)
	{
		delete[] m_pFirst;
		m_pFirst = (type*)::operator new(size * sizeof(type));
	}
	
	Copy(rhs.m_pFirst, m_pFirst, size);

	m_pLast = m_pFirst + size;
	m_pEnd = m_pFirst + rcap;

	return *this;
}

template<class type>
void DynArray<type>::Copy(type* from, type* to, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		::new ((void *)(to + i)) type(from[i]);
}
template<class type>
void DynArray<type>::ReverseCopy(type* from, type* to, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		::new ((void *)(to - i)) type(*(from - i));
}
template<class type>
void DynArray<type>::Init(type* first, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		::new ((void *)(first + i)) type();
}
template<class type>
void DynArray<type>::Destroy(type* first, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		(first + i)->~type();
}
template<class type>
void DynArray<type>::ClearArray()
{
	Destroy(m_pFirst, Size());
	m_pLast = m_pFirst;
}

template<class type>
std::ostream& operator<<(std::ostream& out, const DynArray<type>& arr)
{
	out << '[';
	for (uint32_t i = 0, size = arr.Size() - 1; i <= size; i++)
	{
		out << '\'' << arr[i] << '\'';

		(i < size) ? out << ',' : out << ']';
	}

	return out;
}

#undef END_ITER

#endif