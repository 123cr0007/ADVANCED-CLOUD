#pragma once

#include "DxLib.h"
#include "ObjectBase.h"

#include <string>
#include <vector>


// モード
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void CtrlMove(int trg);
	void ChangeScene();
	void DrawChoiceItem();

protected:

	int handle;
	int handle_title[2];
	int handle_exit[2];
	int handle_cont[2];
	int choice;//選んでいる番号
	int item_num;//項目数

	int time;
	
};
