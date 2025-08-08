#pragma once
#include "LLNode.h"

template <class T>
class LinkedList
{
protected: 
	//Cabecera d la lista (?)
	LLNode<T>* head;
public:

	//añadir los nodos
	LLNode<T>* addNode(T* _data)
	{
		LLNode<T>* newNode = new LLNode<T>(_data);
		if (head == nullptr)
		{
			head = newNode;
		}
		else
		{
			LLNode<T>* iter = head;
			while (iter->Next != nullptr)
			{
				iter = iter->Next;
			}
			iter->Next = newNode;
			newNode->Prev = iter;
		}

		return newNode;
	}

	//Remover el ultimo nodo
	void RemoveLastNode()
	{
		if (head == nullptr) return;

		if (head->Next == nullptr)
		{
			delete head;
			head = nullptr;
			return;
		}

		LLNode<T>* iter = head;
		while (iter->Next->Next != nullptr)
		{
			iter = iter->Next;
		}

		delete iter->Next;
		iter->Next = nullptr;
	}

	
};

