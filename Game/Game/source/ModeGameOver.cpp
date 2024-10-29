
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "ModeGameOver.h"
#include "ModeEffekseer.h"

#define SCREEN_W 1920
#define SCREEN_H 1080


bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }

	time = 0;
	
	return true;
}

bool ModeGameOver::Terminate() {
	base::Terminate();

	return true;
}

bool ModeGameOver::Process() {
	base::Process();

	if (time > 120) {
		ModeGame* modeGame = new ModeGame;
		ModeServer::GetInstance()->Add(modeGame, 1, "Game");
		ModeServer::GetInstance()->Del(this);
	}
	else {
		time++;
	}

	return true;
}


bool ModeGameOver::Render() {
	base::Render();

	return true;
}

void ModeGameOver::CtrlMove(int trg)
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

void ModeGameOver::ChangeScene()
{
	ModeTitle* modeTitle = new ModeTitle;
	ModeGame* modeGame = new ModeGame;
	switch(choice)
	{
	case 0:
		ModeServer::GetInstance()->Add(modeGame, 1, "Game");
		ModeServer::GetInstance()->Del(this);
		break;
	case 1:	
		ModeServer::GetInstance()->Add(modeTitle, 1, "Title");
		ModeServer::GetInstance()->Del(this);
		break;
	
	case 2:
		ApplicationMain * AppMain = new ApplicationMain();
		ModeServer::GetInstance()->Del(this);
		AppMain->Terminate();
		break;
	}
}

void ModeGameOver::DrawChoiceItem()
{
	int x = 720;
	int y = 360;
	DrawGraph(x, y, handle_cont[0], TRUE);
	if(choice == 0)
	{
		DrawGraph(x, y, handle_cont[1], TRUE);
	}
	y += 120;
	DrawGraph(x, y, handle_title[0], TRUE);
	if(choice == 1)
	{
		DrawGraph(x, y, handle_title[1], TRUE);
	}
	y += 120;
	DrawGraph(x, y, handle_exit[0], TRUE);
	if(choice == 2)
	{
		DrawGraph(x, y, handle_exit[1], TRUE);
	}
}

