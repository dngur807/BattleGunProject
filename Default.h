#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <MSWSock.h>
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <map>
#include <vector>
#include <string.h>
#include <stdlib.h>
using namespace std;


#pragma comment( lib, "ws2_32.lib" )
#pragma comment(lib,"mswsock.lib")  // AcceptEx()
#define _SEVER_DEBUG_
#define _FROM_CLIENT_
#define _TO_CLIENT_