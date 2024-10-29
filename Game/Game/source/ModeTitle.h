#pragma once

#include "DxLib.h"
#include "ObjectBase.h"
#include "FadeInOut.h"
#include "Camera.h"
#include "Player.h"
#include <string>
#include <vector>

////ステータス管理用
constexpr auto TITLE_LOGO_X = 960;
constexpr auto TITLE_LOGO_Y = 350;
constexpr auto TITLE_LOGO_SCALE = 1;

constexpr auto TITLE_MENU_ITEM_BAR_X = 960;
constexpr auto TITLE_MENU_ITEM_BAR_Y = 700;
constexpr auto TITLE_MENU_ITEM_BAR_DISTANCE = 100;

constexpr auto CHANGE_WAIT_TIME = 60;

// モード
class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	static ModeTitle* _TitleInstance;
	static ModeTitle* GetInstance() { return (ModeTitle*)_TitleInstance; }

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void CtrlMove(int trg);//choiceを変更する関数
	void ChangeScene();//画面を変える関数
	void LoadEnd();
	void DrawTitleLogo();//タイトルロゴ描画用関数
	void DrawChoiceItem();//選んでいるメニューバーを光らせる
protected:
	Fade* fade;

	int choice;//選んでいる番号
	int item_num;//項目数
	int count; //フレーム秒
	int screen_type;//スクリーンの状態
	int black_clear;//黒の透明度
	int back_num;//背景の番号

protected:
	int handle[2];
	int handle_school;
	int handle_team;
	int handle_logo;
	int handle_any;
	int handle_game[3];
	int handle_set[3];
	int handle_ctrls[3];
	int handle_credit[3];
	int handle_exit[3];
	int handle_load;
private:
	bool isChoice; 
	bool isLoading;
	int draw_num;
	int oldCount;

	Camera* tCam;
	Player* tPl;
};
extern int skip;//タイトルに入るのが初めてか？

