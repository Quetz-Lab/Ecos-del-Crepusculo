#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>


#ifdef  MyLibrary_EXPORTS
#define MyLibrary_API __declspec(dllexport)
#else
#define MyLibrary_API __declspec(dllexport)
#endif
extern "C"
{
	MyLibrary_API void HelloWorld();
}

extern "C" MyLibrary_API int Add(int a, int b);
extern "C" MyLibrary_API int Subtract(int a, int b);
extern "C" MyLibrary_API int MyMessageBoxError(const wchar_t* message, const wchar_t* boxTitle);
extern "C" MyLibrary_API int MyMessageBoxWarning(const wchar_t* message, const wchar_t* boxTitle);