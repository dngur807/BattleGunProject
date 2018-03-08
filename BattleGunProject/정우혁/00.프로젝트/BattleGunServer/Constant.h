#pragma once

#include "Struct.h"
#include "Define.h"

const DWORD	MIN_STR = 64;
const DWORD MIDDLE_STR = 128;
const DWORD	MAX_STR = 256;

extern SERVERCONTEXT		g_Server;//서버 정보 구조체
extern ONTRANSFUNC			g_OnTransFunc[MAXTRANSFUNC];