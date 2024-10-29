#include"ModeSetting.h"
#include "ApplicationMain.h"

ModeSetting* ModeSetting::_SetInstance = nullptr;

ModeSetting::ModeSetting()
{
	_SetInstance = this;

	handle[0] = LoadGraph("res/Menu/Pause/CONFIG.jpg");
	handle[1] = LoadGraph("res/Menu/Title/title2_back.jpg");

	handle_bar[0] = LoadGraph("res/Menu/Setting/bar1.png");//ƒx[ƒX•”•ª
	handle_bar[1] = LoadGraph("res/Menu/Setting/bar2.png");//•Ï“®•”•ª

	handle_button = LoadGraph("res/Menu/Setting/button.png");

	handle_cont[0] = LoadGraph("res/Menu/Setting/SE.png");
	handle_cont[1] = LoadGraph("res/Menu/Setting/BGM.png");
	handle_cont[2] = LoadGraph("res/Menu/Setting/VOICE.png");

	handle_icon[0] = LoadGraph("res/Menu/Setting/icon.png");
	handle_icon[1] = LoadGraph("res/Menu/Setting/icon2.png");

	handle_arrow = LoadGraph("res/Menu/Setting/arrow.png");

	handle_exit[0] = LoadGraph("res/Menu/Title/Exit1.png");
	handle_exit[1] = LoadGraph("res/Menu/Title/Exit2.png");
	handle_exit[2] = LoadGraph("res/Menu/Title/Exit3.png");

	choice = 0;
	item_num = 3;
	count = 0;
	draw_num = 0;

	back_num = rand() % 2;

	se_vol = gGlobal._sndServer.GetSEVolume();
	bgm_vol = gGlobal._sndServer.GetBGMVolume();
	voice_vol = gGlobal._sndServer.GetVOICEVolume();

	isChoice = false;
}

ModeSetting::~ModeSetting()
{
}

bool ModeSetting::Initalize()
{
	if(!base::Initialize()) { return false; }

	return false;
}

bool ModeSetting::Terminate()
{
	base::Terminate();
	return false;
}

bool ModeSetting::Process()
{
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	if(!isChoice)
	{
		CtrlMove(trg);
		if(trg & PAD_INPUT_1)
		{
			isChoice = true;
			oldCount = count;
			gGlobal._sndServer.Get("SE_SELECT_ENTER.mp3")->Play();
		}
	}
	else
	{
		if(count % 3 == 0) {
			draw_num = count % 2 + 1;
		}
		if(count - oldCount >= 30)
		{
			isChoice = false;
			oldCount = 0;
			ChangeScene();
		}
	}
	
	ChangeVolume(key);
	count++;
	return true;
}

bool ModeSetting::Render()
{
	base::Render();

	DrawGraph(0, 0, handle[0], FALSE);
	DrawArrow();
	DrawContents();

	if (AMG_wp::CheckUseCol == true) {
		DrawFormatString(0, 750, GetColor(255, 0, 0), "choice=%d", choice);
		DrawFormatString(0, 800, GetColor(255, 0, 0), "se_vol=%d", se_vol);
		DrawFormatString(0, 850, GetColor(255, 0, 0), "bgm_vol=%d", bgm_vol);
		DrawFormatString(0, 900, GetColor(255, 0, 0), "voice_vol=%d", voice_vol);
	}
	return true;
}

void ModeSetting::CtrlMove(int trg)
{
	if(trg & PAD_INPUT_DOWN)
	{
		gGlobal._sndServer.Get("SE_SELECT.mp3")->Play();
		choice += 1;
		if(choice > item_num)
		{
			choice = 0;
		}
	}
	if(trg & PAD_INPUT_UP)
	{
		gGlobal._sndServer.Get("SE_SELECT.mp3")->Play();
		choice -= 1;
		if(choice < 0)
		{
			choice = item_num;
		}
	}
}

void ModeSetting::ChangeVolume(int key)
{
	if(choice == 1)
	{
		if(key & PAD_INPUT_LEFT)
		{
			se_vol--;
			if(se_vol <= 0)
			{
				se_vol = 0;
			}
		}
		if(key & PAD_INPUT_RIGHT)
		{
			se_vol++;
			if(se_vol >= 100)
			{
				se_vol = 100;
			}
		}
	}
	if(choice == 0)
	{
		if(key & PAD_INPUT_LEFT)
		{
			bgm_vol--;
			if(bgm_vol <= 0)
			{
				bgm_vol = 0;
			}
		}
		if(key & PAD_INPUT_RIGHT)
		{
			bgm_vol++;
			if(bgm_vol >= 100)
			{
				bgm_vol = 100;
			}
		}
	}
	if(choice==2)
	{
		if(key & PAD_INPUT_LEFT)
		{
			voice_vol--;
			if(voice_vol <= 0)
			{
				voice_vol = 0;
			}
		}
		if(key & PAD_INPUT_RIGHT)
		{
			voice_vol++;
			if(voice_vol >= 100)
			{
				voice_vol = 100;
			}
		}
	}
}

void ModeSetting::ChangeScene()
{
	switch(choice)
	{
	case 3:
		gGlobal._sndServer.SetSEVolume(se_vol);
		gGlobal._sndServer.SetBGMVolume(bgm_vol);
		gGlobal._sndServer.SetVOICEVolume(voice_vol);
		ModeServer::GetInstance()->Del(this);
		break;
	}
}

void ModeSetting::DrawContents()
{
	int x = SETTING_MENU_X;
	int y = SETTING_MENU_Y;
	int dist = SETTING_MENU_INTERVAL;

	DrawBGMVolumeBar(x, y, bgm_vol);
	y += dist;
	DrawSEVolumeBar(x, y, se_vol);
	y += dist;
	DrawVOICEVolumeBar(x, y, voice_vol);
	y += dist;
	DrawExit(x, y);
}

void ModeSetting::DrawExit(int x, int y)
{
	if(choice == 3)
	{
		if(isChoice) {
			DrawGraph(x, y, handle_exit[draw_num], TRUE);
		}
		else {
			DrawGraph(x, y, handle_exit[2], TRUE);
		}
	}
	else
	{
		DrawGraph(x, y, handle_exit[0], TRUE);
	}
}

void ModeSetting::DrawArrow()
{
	int x = SETTING_MENU_X;
	int y = SETTING_MENU_Y;
	int dist = SETTING_MENU_INTERVAL;
	y += dist * choice;
	if (choice != 3) {
		DrawGraph(x - 40, y, handle_arrow, TRUE);
	}
}

void ModeSetting::DrawSEVolumeBar(int x,int y,int volume)
{
	DrawGraph(x, y, handle_cont[0], TRUE);
	DrawGraph(x, y, handle_bar[0], TRUE);
	float MoveX = 215 + (600 / 100) * volume;
	DrawExtendGraphF(x + 215, y, x + MoveX, y + 78, handle_bar[1], TRUE);
	if(volume<=0){
		DrawGraph(x + 100, y, handle_icon[1], TRUE);
	}
	else{
		DrawGraph(x + 100, y, handle_icon[0], TRUE);
	}
	DrawGraph(x + MoveX-20, y, handle_button, TRUE);
}

void ModeSetting::DrawBGMVolumeBar(int x, int y, int volume)
{
	DrawGraph(x, y, handle_cont[1], TRUE);
	DrawGraph(x, y, handle_bar[0], TRUE);
	float MoveX = 215 + (600 / 100) * volume;
	DrawExtendGraphF(x + 215, y, x + MoveX, y + 78, handle_bar[1], TRUE);
	if(volume <= 0) {
		DrawGraph(x + 100, y, handle_icon[1], TRUE);
	}
	else {
		DrawGraph(x + 100, y, handle_icon[0], TRUE);
	}
	DrawGraph(x + MoveX - 20, y, handle_button, TRUE);
}

void ModeSetting::DrawVOICEVolumeBar(int x, int y, int volume)
{
	DrawGraph(x, y, handle_cont[2], TRUE);
	DrawGraph(x, y, handle_bar[0], TRUE);
	float MoveX = 215 + (600 / 100) * volume;
	DrawExtendGraphF(x + 215, y, x + MoveX, y + 78, handle_bar[1], TRUE);
	if(volume <= 0) {
		DrawGraph(x + 100, y, handle_icon[1], TRUE);
	}
	else {
		DrawGraph(x + 100, y, handle_icon[0], TRUE);
	}
	DrawGraph(x + MoveX - 20, y, handle_button, TRUE);
}
