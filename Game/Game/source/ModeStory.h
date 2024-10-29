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
	bool playSound;		//�Đ����邩�ǂ���
	bool playInGame;	//�Q�[�����ōĐ����邩
	bool playCredit;	//�N���W�b�g���Đ����邩
	bool oldPlayCredit;	//�N���W�b�g���Đ����邩

	int Time;		//�摜��\�������鎞��
	int useScenarioNum;		//�����Ԗڂ̃V�i���I��
	int scenarioHandleNum;	//�������ڂ̉摜�ɂ��邩
	int useInGameNum;		//�����Ԗڂ̃V�i���I��
	int inGameHandleNum;	//�������ڂ̉摜�ɂ��邩

	bool gameEnd;

	int trg;
	int key;
	std::string modeName;
	Fade* _fade;
};