#pragma once

//내부 메시지 구조체
typedef struct tagInterMsgData
{
	short sHeaderType;
	char tcInterMsg[256];
}INTERMSGDATA;