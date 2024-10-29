#pragma once
#include "appframe.h"

#include <string>
#include <vector>

constexpr auto PAUSE_ITEM_SLOT_X = 600;
constexpr auto PAUSE_ITEM_SLOT_Y = 120;

constexpr auto PAUSE_CONTENTS_X = 0;
constexpr auto PAUSE_CONTENTS_Y = 0;

constexpr auto PAUSE_SETTING_X = 960;
constexpr auto PAUSE_SETTING_Y = 220;
constexpr auto PAUSE_SETTING_INTERVAL = 120;

constexpr auto PAUSE_TITLE_X = 600;
constexpr auto PAUSE_TITLE_Y = 700;
constexpr auto PAUSE_TITLE_INTERVAL = 400;

constexpr auto PAUSE_ITEM_SLOT_ROT = 10;//付箋の角度

// モード
class ModeMenu : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void CtrlMove(int trg);
	void ConfigMove(int trg);
	void ExitConfig();
	void IsTitleBack(int trg);

	void ChangeVolume(int key);
	void ChangeContents();
	void ChangeDrawPage();

	void DrawChoiceItem();
	void DrawArrow();
	void DrawSettingCont();
	void DrawSelectButton();
protected:

	//ノート
	int handle_cont[4];
	//付箋
	int handle_conf[3];
	int handle_ctrl[3];
	int handle_mission[3];
	int handle_title[3];
	//選択肢
	int handle_yes[3];
	int handle_no[3];
	//退出
	int handle_exit[3];
	
	//コンフィグのパーツ
	int handle_bar[2];//ゲージバー
	int handle_elem[3];//どのバーであるかの案内
	int handle_icon[2];//音が出るかのアイコン
	int handle_arrow;//矢印
	
	
	int choice;//選んでいる番号
	int choice_vol;//選んでいる番号(コンフィグ用)
	int vol_item_num;
	int bg_draw_num;

	bool isChoice;
	bool isChoice_vol;
	bool IsTitle;

	int draw_num;
	int oldCount;
	int item_num;//項目数
	int count;

	int se_vol;
	int bgm_vol;
	int voice_vol;
private:
	
}; 
