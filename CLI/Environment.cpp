/**
 * @file Environment.cpp
 * @author Bryn McKerracher
 **/
#include "Environment.h"

#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
#include <iostream>
#include <Windows.h>
#endif

namespace TapeWorm {
    Environment::~Environment() {
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
        HWND consoleWnd = GetConsoleWindow();
        DWORD dwProcessId;
        GetWindowThreadProcessId(consoleWnd, &dwProcessId);
        if (GetCurrentProcessId() == dwProcessId) {
            std::getchar();
        }
#endif
    }
} // TapeWorm