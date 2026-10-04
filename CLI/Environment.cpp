/**
 * @file Environment.cpp
 * @author Bryn McKerracher
 **/
#include "Environment.h"
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
#include <Windows.h>
#endif

namespace TapeWorm {
    Environment::~Environment() {
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
        DWORD ids[2];
        DWORD idsToGet = 2;
        DWORD launchedByAssociation = GetConsoleProcessList((LPDWORD)ids, idsToGet);
        if (launchedByAssociation == 1) {
            std::getchar();
        }
#endif
    }
} // TapeWorm