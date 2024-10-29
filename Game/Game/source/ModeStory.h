#pragma once
#include "AppFrame.h"
#include"ModeTitle.h"
#include "FadeInOut.h"

constexpr auto SCENARIO_NUM = 4;
constexpr auto SCENARIO_HANDLE_MAX = 15;
constexpr auto INGAME_HANDLE_MAX = 13;

constexpr auto SCENARIO_WAIT_TIME = 60;


class ModeStory : public ModeBase
{
	typedef ModeBase base;
public:

	ModeStory();

	virtual bool Initalize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:

	bool Sceneario();
	bool InGame();
	bool Credit();
	bool GameFin();

	int handleScenario[SCENARIO_NUM][SCENARIO_HANDLE_MAX];
	int soundScenario[SCENARIO_NUM][SCENARIO_HANDLE_MAX];

	int handleInGame[SCENARIO_NUM][INGAME_HANDLE_MAX];
	int soundInGame[SCENARIO_NUM][INGAME_HANDLE_MAX];

	int handleCredit;
	int handleSkip;
	int handleFin;
	int handleBack[SCENARIO_NUM + 1];
	bool playSound;		//再生するかどうか
	bool playInGame;	//ゲーム内で再生するか
	bool playCredit;	//クレジットを再生するか
	bool oldPlayCredit;	//クレジットを再生するか

	int Time;		//画像を表示させる時間
	int useScenarioNum;		//今何番目のシナリオか
	int scenarioHandleNum;	//今何枚目の画像にいるか
	int useInGameNum;		//今何番目のシナリオか
	int inGameHandleNum;	//今何枚目の画像にいるか

	bool gameEnd;

	int trg;
	int key;
	std::string modeName;
	Fade* _fade;
};