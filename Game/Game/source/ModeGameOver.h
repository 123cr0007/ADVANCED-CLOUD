#pragma once

#include "DxLib.h"
#include "ObjectBase.h"

#include <string>
#include <vector>


// ƒ‚[ƒh
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
	int choice;//‘I‚ñ‚Å‚¢‚é”Ô†
	int item_num;//€–Ú”

	int time;
	
};
