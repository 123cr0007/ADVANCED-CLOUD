#pragma once
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
class ModeCredit :public ModeBase
{
	typedef ModeBase base;
public:
	ModeCredit();
	~ModeCredit();
	bool Initialize();
	bool Terminate();
	bool Process();
	bool Render();

	void CtrlMove(int trg);
	void ChangeScene();
	void DrawChoiceItem();
private:
	int handle;
	int choice;//選んでいる番号
	int item_num;//項目数
};