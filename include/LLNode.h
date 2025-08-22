#pragma once
template <class T>
class LLNode
{
public:
	//Referencia que apunta al siguiente nodo de la lista
	LLNode<T>* Next;
	LLNode<T>* Prev;

	T* data;

	//Crear dos constructores para aprovechar el polimofismo de POO

	LLNode(T* _data) : data(_data), Next(nullptr), Prev(nullptr){}
	LLNode(T* d, LLNode<T>* nxt) : data(d), Next(nxt), Prev(nullptr) {}
};

