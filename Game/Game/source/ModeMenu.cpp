
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeMenu.h"
#include "ModeTitle.h"
#include "ModeSetting.h"
#include "ModeControl.h"
#include "ApplicationMain.h"

bool ModeMenu::Initialize() {
	if (!base::Initialize()) { return false; }

	handle_cont[0] = LoadGraph("res/Menu/Pause/MISSION.jpg");
	handle_cont[1] = LoadGraph("res/Menu/Pause/CONTROL.jpg");
	handle_cont[2] = LoadGraph("res/Menu/Pause/CONFIG.jpg");
	handle_cont[3] = LoadGraph("res/Menu/Pause/TITLE.jpg");
	
	handle_mission[0] = LoadGraph("res/Menu/Pause/UI_PO_MISSION_1.png");
	handle_mission[1] = LoadGraph("res/Menu/Pause/UI_PO_MISSION_2.png");
	handle_mission[2] = LoadGraph("res/Menu/Pause/UI_PO_MISSION_3.png");
	
	handle_ctrl[0] = LoadGraph("res/Menu/Pause/UI_PO_CONTROL_1.png");
	handle_ctrl[1] = LoadGraph("res/Menu/Pause/UI_PO_CONTROL_2.png");
	handle_ctrl[2] = LoadGraph("res/Menu/Pause/UI_PO_CONTROL_3.png");
	
	handle_conf[0] = LoadGraph("res/Menu/Pause/UI_PO_CONFIG_1.png");
	handle_conf[1] = LoadGraph("res/Menu/Pause/UI_PO_CONFIG_2.png");
	handle_conf[2] = LoadGraph("res/Menu/Pause/UI_PO_CONFIG_3.png");
	
	handle_title[0] = LoadGraph("res/Menu/Pause/UI_PO_TITLE_1.png");
	handle_title[1] = LoadGraph("res/Menu/Pause/UI_PO_TITLE_2.png");
	handle_title[2] = LoadGraph("res/Menu/Pause/UI_PO_TITLE_3.png");

	handle_yes[0] = LoadGraph("res/Menu/Pause/UI_PO_YES_1.png");
	handle_yes[1] = LoadGraph("res/Menu/Pause/UI_PO_YES_2.png");
	handle_yes[2] = LoadGraph("res/Menu/Pause/UI_PO_YES_3.png");

	handle_no[0] = LoadGraph("res/Menu/Pause/UI_PO_NO_1.png");
	handle_no[1] = LoadGraph("res/Menu/Pause/UI_PO_NO_2.png");
	handle_no[2] = LoadGraph("res/Menu/Pause/UI_PO_NO_3.png");

	handle_exit[0] = LoadGraph("res/Menu/Title/Exit1.png");
	handle_exit[1] = LoadGraph("res/Menu/Title/Exit2.png");
	handle_exit[2] = LoadGraph("res/Menu/Title/Exit3.png");

	handle_arrow = LoadGraph("res/Menu/Setting/Arrow.png");

	choice = 0;
	choice_vol = 0;

	isChoice = false;
	isChoice_vol = false;
	IsTitle = false;

	count = 0;
	bg_draw_num = 0;

	item_num = 3;
	vol_item_num = 3;

	se_vol = gGlobal._sndServer.GetSEVolume();
	bgm_vol = gGlobal._sndServer.GetBGMVolume();
	voice_vol = gGlobal._sndServer.GetVOICEVolume();

	return true;
}

bool ModeMenu::Terminate() {
	base::Terminate();
	return true;
}

bool ModeMenu::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// ESCキーでメニューを閉じる
	bool close = false;
	if (trg & PAD_INPUT_R) {
		close = true;
	}
	
	if(!isChoice) //何も選んでいない
	{
		CtrlMove(trg);

		if(trg & PAD_INPUT_1)
		{
			isChoice = true;
			oldCount = count;
			gGlobal._sndServer.Get("SE_SELECT_ENTER.mp3")->Play();
		}
	}
	else if(bg_draw_num != 4)//項目番号外でない
	{
		if(count - oldCount >= 5)//決定後の5f待ち
		{
			oldCount = 0;
			ChangeContents();
			if(bg_draw_num == 2)//コンフィグ
			{
				ConfigMove(trg);//コンフィグ内移動
				ChangeVolume(key);//音量変更
				if(choice_vol==3 && trg & PAD_INPUT_1)
				{
					ExitConfig();//コンフィグ退出＆
				}
			}
			if(bg_draw_num == 3)
			{
				IsTitleBack(trg);
			}
			else if(bg_draw_num < 2)
			{
				isChoice = false;
			}
		}
	}

	// メニューを閉じる
	if (close) {
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}
	count++;
	return true;
}

bool ModeMenu::Render() {
	base::Render();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	
	DrawGraph(0, 0, handle_cont[bg_draw_num], TRUE);
	
	if(bg_draw_num != 3) {
		DrawChoiceItem();
		if(bg_draw_num == 2)
		{
			DrawArrow();
			DrawSettingCont();
		}
	}
	else
	{
		DrawSelectButton();
	}
	if (AMG_wp::CheckUseCol == true) {
		DrawFormatString(0, 0, GetColor(0, 0, 0), "choice=%d", choice_vol);
	}
	return true;
}

void ModeMenu::CtrlMove(int trg)
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

void ModeMenu::ConfigMove(int trg)
{
	if(trg & PAD_INPUT_DOWN)
	{
		choice_vol += 1;
		if(choice_vol> vol_item_num)
		{
			choice_vol = 0;
		}
	}
	if(trg & PAD_INPUT_UP)
	{
		choice_vol -= 1;
		if(choice_vol < 0)
		{
			choice_vol = vol_item_num;
		}
	}
}

void ModeMenu::ExitConfig()
{
	gGlobal._sndServer.SetSEVolume(se_vol);
	gGlobal._sndServer.SetBGMVolume(bgm_vol);
	gGlobal._sndServer.SetVOICEVolume(voice_vol);
	gGlobal._sndServer.Get("SE_SELECT_ENTER.mp3")->Play();
	choice_vol = 0;
	isChoice = false;
}

void ModeMenu::IsTitleBack(int trg)
{
	ModeTitle* modeTitle = new ModeTitle();
	if(trg & PAD_INPUT_RIGHT || trg & PAD_INPUT_LEFT)
	{
		if(!IsTitle)
		{
			gGlobal._sndServer.Get("SE_SELECT.mp3")->Play();
			IsTitle = true;
		}
		else
		{
			gGlobal._sndServer.Get("SE_SELECT.mp3")->Play();
			IsTitle = false;
		}
	}
	if(trg & PAD_INPUT_1) 
	{
		if(IsTitle)
		{
			gGlobal._sndServer.Get("SE_SELECT_ENTER.mp3")->Play();
			ModeServer::GetInstance()->Clear();
			ModeServer::GetInstance()->Add(modeTitle, 2, "Title");
		}
		else if(!IsTitle)
		{
			isChoice = false;
		}
	}
	if (AMG_wp::CheckUseCol == true) {
		DrawFormatString(0, 30, GetColor(0, 0, 0), "title=%d", IsTitle);
	}
}

void ModeMenu::ChangeVolume(int key)
{
	if(choice_vol == 1)
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
	if(choice_vol == 0)
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
	if(choice_vol == 2)
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

void ModeMenu::ChangeContents()
{
	if(choice==0)
	{
		bg_draw_num = 0;
	}
	if(choice == 1)
	{
		bg_draw_num = 1;
	}
	if(choice == 2)
	{
		bg_draw_num = 2;
	}
	if(choice == 3)
	{
		bg_draw_num = 3;
	}
}

void ModeMenu::ChangeDrawPage()
{
	int x = PAUSE_CONTENTS_X;
	int y = PAUSE_CONTENTS_Y;
	if(choice == 0)
	{
		DrawGraph(x, y, handle_cont[choice], TRUE);
	}
	if(choice == 1)
	{
		DrawGraph(x, y, handle_cont[choice], TRUE);
	}
	if(choice == 2)
	{
		DrawGraph(x, y, handle_cont[choice], TRUE);
	}
	if(choice == 3)
	{
		DrawGraph(x, y, handle_cont[choice], TRUE);
	}
}

void ModeMenu::DrawChoiceItem()
{
	int x = PAUSE_ITEM_SLOT_X;
	int y = PAUSE_ITEM_SLOT_Y;
	
	if(choice == 0)
	{
		if(isChoice) {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT+5), handle_mission[draw_num], TRUE);
		}
		else{
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT+5), handle_mission[2], TRUE);
		}	
	}
	else
	{
		DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT), handle_mission[0], TRUE);
	}
	y += 240;
	if(choice == 1)
	{
		if(isChoice) {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT+5), handle_ctrl[draw_num], TRUE);
		}
		else {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT + 5), handle_ctrl[2], TRUE);
		}
	}
	else
	{
		DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT), handle_ctrl[0], TRUE);
	}
	y += 240;
	if(choice == 2)
	{
		if(isChoice) {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT + 5), handle_conf[draw_num], TRUE);
		}
		else {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT + 5), handle_conf[2], TRUE);
		}
	}
	else
	{
		DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT), handle_conf[0], TRUE);
	}
	y += 240;
	if(choice == 3)
	{
		if(isChoice) {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT + 5), handle_title[draw_num], TRUE);
		}
		else {
			DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT + 5), handle_title[2], TRUE);
		}
	}
	else
	{
		DrawRotaGraph(x, y, 1.0, DegToRad(PAUSE_ITEM_SLOT_ROT), handle_title[0], TRUE);
	}
}

void ModeMenu::DrawArrow()
{
	int x = PAUSE_SETTING_X;
	int y = PAUSE_SETTING_Y;
	int dist = PAUSE_SETTING_INTERVAL;
	y += dist * choice_vol;
	if(choice_vol != 3) {
		DrawGraph(x - 40, y, handle_arrow, TRUE);
	}
}

void ModeMenu::DrawSettingCont()
{
	int x = PAUSE_SETTING_X;
	int y = PAUSE_SETTING_Y;
	int dist = PAUSE_SETTING_INTERVAL;
	ModeSetting::GetInstance()->DrawBGMVolumeBar(x, y, bgm_vol);
	y += dist;
	ModeSetting::GetInstance()->DrawSEVolumeBar(x, y, se_vol);
	y += dist;
	ModeSetting::GetInstance()->DrawVOICEVolumeBar(x, y, voice_vol);
	y += dist;
	if(choice_vol == 3)
	{
		if(isChoice_vol) {
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

void ModeMenu::DrawSelectButton()
{
	if(IsTitle)
	{
		DrawGraph(PAUSE_TITLE_X, PAUSE_TITLE_Y, handle_yes[2], TRUE);
		DrawGraph(PAUSE_TITLE_X+PAUSE_TITLE_INTERVAL, PAUSE_TITLE_Y, handle_no[0], TRUE);
	}
	else
	{
		DrawGraph(PAUSE_TITLE_X, PAUSE_TITLE_Y, handle_yes[0], TRUE);
		DrawGraph(PAUSE_TITLE_X + PAUSE_TITLE_INTERVAL, PAUSE_TITLE_Y, handle_no[2], TRUE);
	}
}
