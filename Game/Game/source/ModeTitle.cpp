
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeSound.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "ModeSetting.h"
#include "ModeControl.h"
#include "ModeCredit.h"
#include "ModeEffekseer.h"
#include "ModeStory.h"

#define SCREEN_W 1920
#define SCREEN_H 1080

int skip = 0;
ModeTitle* ModeTitle::_TitleInstance = nullptr;
bool ModeTitle::Initialize() {

	_TitleInstance = this;

	tCam = new Camera;
	tPl = new Player;

	if (!base::Initialize()) { return false; }
	fade = new Fade;
	choice = 0;
	item_num = 3;
	count = 0;
	isChoice = false;
	isLoading = false;
	back_num = rand() % 2;
	back_num = 0;

	AMG_wp::_st->TitleInitialize();

	ApplicationMain::GetMainInstance()->ResetStage();

	//俺がデストロンのニューリーダーだ！みんな俺に続けぇー！
	//何を言っておるか！この愚か者めが！
	
	if(skip != 1) {
		screen_type = 0;
	}
	else {
		screen_type = 8;
	}
	
	//タイトル背景
	{
		handle[0] = LoadGraph("res/Menu/Title/title_back.jpg");
		handle[1] = LoadGraph("res/Menu/Title/title2_back.jpg");
	}
	//タイトル前のアイテム
	{
		handle_logo = LoadGraph("res/Menu/Title/TitleLogo.png");
		handle_school = LoadGraph("res/Menu/Title/AMGlogo.png");
		handle_team = LoadGraph("res/Menu/Title/TeamLogo.png");
		handle_any = LoadGraph("res/Menu/Title/PressAnyButton.png");
		handle_load = LoadGraph("res/Menu/Loading.png");
	}
	//メニューバー「GameStart」
	{
		handle_game[0] = LoadGraph("res/Menu/Title/GameStart1.png");
		handle_game[1] = LoadGraph("res/Menu/Title/GameStart2.png");
		handle_game[2] = LoadGraph("res/Menu/Title/GameStart3.png");
	}
	//メニューバー「Settings」
	{
		handle_set[0] = LoadGraph("res/Menu/Title/Setting1.png");
		handle_set[1] = LoadGraph("res/Menu/Title/Setting2.png");
		handle_set[2] = LoadGraph("res/Menu/Title/Setting3.png");
	}
	//メニューバー「Credit」
	{
		handle_credit[0] = LoadGraph("res/Menu/Title/Staffroll1.png");
		handle_credit[1] = LoadGraph("res/Menu/Title/Staffroll2.png");
		handle_credit[2] = LoadGraph("res/Menu/Title/Staffroll3.png");
	}
	//メニューバー「Exit」
	{
		handle_exit[0] = LoadGraph("res/Menu/Title/Exit1.png");
		handle_exit[1] = LoadGraph("res/Menu/Title/Exit2.png");
		handle_exit[2] = LoadGraph("res/Menu/Title/Exit3.png");
	}

	ModeServer::GetInstance()->Add(new ModeSound(), 99999, "SoundLayer");

	tCam->Initialize();
	tPl->Initialize();

	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	for(int i = 0; i < 2; i++) {
		DeleteGraph(handle[i]);
	}
	DeleteGraph(handle_logo);
	for(int i = 0;i < 2;i++)
	{
		DeleteGraph(handle_game[i]);
		DeleteGraph(handle_set[i]);
		DeleteGraph(handle_ctrls[i]);
		DeleteGraph(handle_exit[i]);
	}
	tCam = nullptr;
	tPl = nullptr;
	ModeServer::GetInstance()->Del(this);
	Stage::GetSTInstance()->TitleTerminate();

	return true;
}

bool ModeTitle::Process() 
{
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	ModeServer::GetInstance()->SkipProcessUnderLayer();
	if(skip != 1) 
	{
		if(count == CHANGE_WAIT_TIME && screen_type < 7)
		{
			if (screen_type == 5) {
				if (AMG_wp::_st->GetFinLoad() == true) {
					++screen_type;
				}
			}
			else {
				++screen_type;
			}
			count = 0;
		}

		switch(screen_type)
		{
		case 0:
			//学院ロゴフェードイン
			fade->FadeModeChange(CHANGE_WAIT_TIME, 1);
			break;

		case 1:
			//表示
			fade->FadeModeChange(CHANGE_WAIT_TIME, 0);
			break;

		case 2:
			//フェードアウト
			fade->FadeModeChange(CHANGE_WAIT_TIME, 2);
			break;

		case 3:
			//チームロゴフェードイン
			fade->FadeModeChange(CHANGE_WAIT_TIME, 1);
			break;

		case 4:
			//表示
			fade->FadeModeChange(CHANGE_WAIT_TIME, 0);
			break;

		case 5:
			//フェードアウト
			fade->FadeModeChange(CHANGE_WAIT_TIME, 2);
			break;

		case 6:
			//タイトルフェードイン
			fade->FadeModeChange(CHANGE_WAIT_TIME, 1);
			break;
		}
	}
	
	//ロゴスキップ
	if(trg & PAD_INPUT_1){
		//学院ロゴスキップ
		if(0 <= screen_type && screen_type <= 2){
			screen_type = 3;
			count = 0;
		}
		//チームロゴスキップ
		else if(3 <= screen_type && screen_type <= 5){
			screen_type = 6;
			count = 0;
		}
	}

	//モード切り替え
	if(8 <= screen_type){
		skip = 1;
		
		if(!isChoice){	//何も選ばれていないなら
			//メニューバー移動
			CtrlMove(trg);
			//Aボタンで決定
			if(trg & PAD_INPUT_1){
				gGlobal._sndServer.Get("SE_SELECT_ENTER.mp3")->Play();
				isChoice = true;
				oldCount = count;
			}
		}
		else{
			//ボタン点滅
			if(count % 3 == 0) {
				draw_num = count % 2 + 1;
			}
			if(count-oldCount>=30){//30f待った
				isChoice = false;
				oldCount = 0;
				ChangeScene();//画面変更
			}
		}
	}

	//スキップしないなら画面を進行
	if(skip != 1) {
		if(screen_type == 7 && trg)	{
			++screen_type;
		}
	}

	++count;
	fade->Process();
	AMG_wp::_st->TitleProcess();
	tPl->ProcessForTitle();

	return true;
}


bool ModeTitle::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade->GetAlpha());
	tCam->Render();
	
	if(skip==1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255);
	}
	if(0 <= screen_type && screen_type <= 2)
	{
		DrawGraph(0, 0, handle_school, TRUE);
	}
	else if(3 <= screen_type && screen_type <= 5)
	{
		DrawGraph(365, 225, handle_team, TRUE);
	}
	
	if(screen_type>=6)
	{

		DrawGraph(0, 0, handle[back_num], FALSE);
		
		// 3D基本設定
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
		SetUseBackCulling(TRUE);

		AMG_wp::_st->RenderForTitle();
		tPl->RenderForTitle();

		// 3D基本設定
		SetUseZBuffer3D(FALSE);
		SetWriteZBuffer3D(FALSE);
		SetUseBackCulling(FALSE);

		DrawTitleLogo();
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if(screen_type==7)
	{
		DrawGraph(773, 800, handle_any, TRUE);
	}
	else if(8 <= screen_type) 
	{
		DrawChoiceItem();
	}

	if(isLoading)
	{
		DrawGraph(0, 0, handle_load, TRUE);
	}
	
	return true;
}

void ModeTitle::CtrlMove(int trg)
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

void ModeTitle::ChangeScene()
{
	ModeGame* modeGame = new ModeGame();
	ModeSetting* modeSet = new ModeSetting();
	ModeControl* modeCtrl = new ModeControl();
	ModeCredit* modeCredit = new ModeCredit();
	switch(choice)
	{
	case 0:
		
		gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->Stop();
		gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->SetVolume(0);
		ModeServer::GetInstance()->Add(modeGame, 0, "Game");
		PlayMovie("res/Movie/opening.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
		isLoading = true;
		AMG_wp::StageNum = 0;
		break;

	case 1:
		ModeServer::GetInstance()->Add(modeSet, 99, "Setting");
		break;

	case 2:
		ModeServer::GetInstance()->Add(modeCredit, 99, "Controls");
		break;

	case 3:
		ApplicationMain* AppMain = new ApplicationMain();
		ModeServer::GetInstance()->Del(this);
		AppMain->Terminate();
		break;
	};
}

void ModeTitle::LoadEnd()
{
	isLoading = false; 
	ModeServer::GetInstance()->Del(this);
}

void ModeTitle::DrawTitleLogo()
{
	int x = TITLE_LOGO_X;
	int y = TITLE_LOGO_Y;
	int size_x, size_y;
	GetGraphSize(handle_logo, &size_x, &size_y);
	x = x - size_x / 2;
	y = y - size_y / 2;
	DrawGraph(x, y, handle_logo, TRUE);
}

void ModeTitle::DrawChoiceItem()
{
	int x = TITLE_MENU_ITEM_BAR_X;
	int y = TITLE_MENU_ITEM_BAR_Y;
	int size_x, size_y;
	GetGraphSize(handle_game[0], &size_x, &size_y);
	x = x - size_x / 2;
	y = y - size_y / 2;
	
	if(choice==0)
	{
		if(isChoice)
		{
			DrawGraph(x, y, handle_game[draw_num], TRUE);
		}
		else
		{
			DrawGraph(x, y, handle_game[2], TRUE);
		}
	}
	else
	{
		DrawGraph(x, y, handle_game[0], TRUE);
	}

	y += TITLE_MENU_ITEM_BAR_DISTANCE;
	if(choice == 1){
		if(isChoice){
			DrawGraph(x, y, handle_set[draw_num], TRUE);
		}
		else{
			DrawGraph(x, y, handle_set[2], TRUE);
		}
	}
	else
	{
		DrawGraph(x, y, handle_set[0], TRUE);
	}
	y += TITLE_MENU_ITEM_BAR_DISTANCE;
	if(choice == 2)
	{
		if(isChoice) {
			DrawGraph(x, y, handle_credit[draw_num], TRUE);
		}
		else {
			DrawGraph(x, y, handle_credit[2], TRUE);
		}
	}
	else
	{
		DrawGraph(x, y, handle_credit[0], TRUE);
	}

	y += TITLE_MENU_ITEM_BAR_DISTANCE;
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

