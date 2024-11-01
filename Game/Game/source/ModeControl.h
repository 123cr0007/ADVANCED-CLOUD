#pragma once
#include"AppFrame.h"
#include "ModeTitle.h"
class ModeControl:public ModeBase
{
	typedef ModeBase base;
public:
	ModeControl();
	~ModeControl();
	bool Initialize();
	bool Terminate();
	bool Process();
	bool Render();

	void CtrlMove(int trg);
	void ChangeScene();
	void DrawChoiceItem();
protected:
	int handle;
	int choice;//選んでいる番号
	int item_num;//項目数
};
