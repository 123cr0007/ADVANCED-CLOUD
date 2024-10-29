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
	int choice;//‘I‚ñ‚Å‚¢‚é”Ô†
	int item_num;//€–Ú”
};
