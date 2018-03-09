#pragma once
enum eProtocolType
{
	REQUEST_LOGIN = 0 ,

	ANSWER_LOGIN, // 결과

	NOTIFY_YOURINFO, // 인덱스 및 팀정보 등등

	// 로비 프로토콜
	NOTIFY_USERLIST
};

enum 
{
	// ANSWER_LOGIN
	SUCCESS_LOGIN
};