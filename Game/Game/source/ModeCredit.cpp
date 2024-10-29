#include "ModeCredit.h"


ModeCredit::ModeCredit()
{
	handle = LoadGraph("res/Movie/staffrole.mp4");
}

ModeCredit::~ModeCredit()
{
}

bool ModeCredit::Initialize()
{
	if(!base::Initialize()) { return false; }

	return false;
}

bool ModeCredit::Terminate()
{
	base::Terminate();
	return false;
}

bool ModeCredit::Process()
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

bool ModeCredit::Render()
{
	base::Render();
	PlayMovie("res/Movie/staffrole.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
	return false;
}

void ModeCredit::CtrlMove(int trg)
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

void ModeCredit::ChangeScene()
{
}

void ModeCredit::DrawChoiceItem()
{
}
