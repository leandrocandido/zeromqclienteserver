// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <iostream>
#include <zmq.hpp>

using namespace std::this_thread;
using namespace std;

static std::thread m_conn_thread;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void task_server_mq()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 1 };

    //// construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{ context, zmq::socket_type::rep };
    socket.bind("tcp://*:5555");

    // prepare some static data for responses
    const std::string data{ "eyJudW1lcmFjYW9DYXJ0ZWxhcyI6W1sxLDIsMyw0LDUsNiw3LDgsOSwxMCwxMSwxMiwxMywxNCwxNV0sWzEsMiwzLDQsNSw2LDcsOCw5LDEwLDExLDEyLDEzLDE0LDE1XSxbMSwyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTMsMTQsMTVdLFsxLDIsMyw0LDUsNiw3LDgsOSwxMCwxMSwxMiwxMywxNCwxNV1dfQ==" };

    for (;;)
    {
        zmq::message_t request;
        std::cout << "[server] Waiting for new messages "<< std::endl;
        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "[server] Received " << request.to_string() << std::endl;

        if (request.to_string() == "EXIT")
        {
            socket.send(zmq::buffer(data), zmq::send_flags::none);
            break;
        }

        // simulate work
        std::this_thread::sleep_for(1s);

        // send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    m_conn_thread.join();
}

extern "C" {
    __declspec(dllexport) void __stdcall start_zeromq_server()
    {
        //m_conn_thread = std::move(std::thread([=]() {task_server_mq(); return 1; }));
        m_conn_thread = std::thread(&task_server_mq);
    }

    __declspec(dllexport) int __stdcall sum(int a, int b)
    {
        return (a + b);
        return true;
    }
}
