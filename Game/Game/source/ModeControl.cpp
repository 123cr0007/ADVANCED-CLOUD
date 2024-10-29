#include "ModeControl.h"
#include "ApplicationMain.h"

ModeControl::ModeControl()
{
	handle = LoadGraph("res/Menu/Control/Controls_bg.png");
}

ModeControl::~ModeControl()
{
}

bool ModeControl::Initialize()
{
	if(!base::Initialize()) { return false; }
	return true;
}

bool ModeControl::Terminate()
{
	return true;
}

bool ModeControl::Process()
{
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	ModeServer::GetInstance()->SkipProcessUnderLayer();
	
	if(trg & PAD_INPUT_1)
	{
		ModeServer::GetInstance()->Del(this);
	}
	return true;
}

bool ModeControl::Render()
{
	base::Render();
	DrawGraph(0, 0, handle, TRUE);
	return true;
}

void ModeControl::CtrlMove(int trg)
{
	if(trg & PAD_INPUT_DOWN)
	{
		choice += 1;
		if(choice > item_num)
		{
			choice = 0;
		}
	}
	if(trg & PAD_INPUT_UP)
	{
		choice -= 1;
		if(choice < 0)
		{
			choice = item_num;
		}
	}
}

void ModeControl::ChangeScene()
{
}

void ModeControl::DrawChoiceItem()
{
}
