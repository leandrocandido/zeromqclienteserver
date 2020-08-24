// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

extern "C"
{
    __declspec(dllexport) void __stdcall start_zeromq_server();

    __declspec(dllexport) int __stdcall sum(int a, int b);
}

void task_server_mq();

