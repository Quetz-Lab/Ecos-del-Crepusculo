#pragma once
template <typename T>
class Singleton
{
public:
	//accesor principal del singleton
	static T& instance()
	{
		static T instance;
		return instance;
	}
	//desactivar los constructores de copia y asignación
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	Singleton() {}
	~Singleton() {}
};

