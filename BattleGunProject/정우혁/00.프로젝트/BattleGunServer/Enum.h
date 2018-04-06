#pragma once
enum		//Overlapped 확장 구조체의 모드 설정 값   
{
	RECVEOVTCP = 0, SENDEOVTCP,
};

enum eTeamType
{
	TEAM_A , 
	TEAM_B , 
	TEAM_END,
};
enum eMapType
{
	MAP_GESTALT,
	MAP_CONVOY,
	MAP_END,
};
enum eCharType
{
	CHAR_DEFAULT,
	CHAR_END,
};
enum eServState
{
	SERVSTATE_INGAME_WAITING,
	SERVSTATE_INGAME_PLAYING,
	SERVSTATE_GAMEEND,
	SERVSTATE_END,
};

