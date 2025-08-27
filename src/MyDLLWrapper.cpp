#include "MyDLLWrapper.h"
#include "MyLibrary.h"

int MyDLLWrapper::MBError(const wchar_t* message, const wchar_t* boxTitle)
{
    return  MyMessageBoxError(message, boxTitle);
}

int MyDLLWrapper::MBWarning(const wchar_t* message, const wchar_t* boxTitle)
{
    return MyMessageBoxWarning(message, boxTitle);
}
