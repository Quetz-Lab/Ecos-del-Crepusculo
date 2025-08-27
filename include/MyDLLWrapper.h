#pragma once
class MyDLLWrapper
{

	//Funciones estaticas
	static int MBError(const wchar_t* message, const wchar_t* boxTitle);
	static int MBWarning(const wchar_t* message, const wchar_t* boxTitle);

};

