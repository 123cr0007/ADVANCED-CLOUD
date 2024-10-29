#pragma once
#include "AppFrame.h"
#include"ModeTitle.h"

constexpr auto TUTORIAL_NUM = 3;
constexpr auto TUTORIAL_HANDLE_MAX = 9;


class ModeTutorial : public ModeBase
{
	typedef ModeBase base;
public:

	ModeTutorial();

	virtual bool Initalize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:

	int handleTutorial[TUTORIAL_NUM][TUTORIAL_HANDLE_MAX];
	
	int tutorialHandleNum;	//�������ڂ̉摜�ɂ��邩

	int trg;
	int key;
};