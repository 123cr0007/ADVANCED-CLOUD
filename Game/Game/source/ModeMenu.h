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

constexpr auto PAUSE_ITEM_SLOT_ROT = 10;//�tⳂ̊p�x

// ���[�h
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

	//�m�[�g
	int handle_cont[4];
	//�t�
	int handle_conf[3];
	int handle_ctrl[3];
	int handle_mission[3];
	int handle_title[3];
	//�I����
	int handle_yes[3];
	int handle_no[3];
	//�ޏo
	int handle_exit[3];
	
	//�R���t�B�O�̃p�[�c
	int handle_bar[2];//�Q�[�W�o�[
	int handle_elem[3];//�ǂ̃o�[�ł��邩�̈ē�
	int handle_icon[2];//�����o�邩�̃A�C�R��
	int handle_arrow;//���
	
	
	int choice;//�I��ł���ԍ�
	int choice_vol;//�I��ł���ԍ�(�R���t�B�O�p)
	int vol_item_num;
	int bg_draw_num;

	bool isChoice;
	bool isChoice_vol;
	bool IsTitle;

	int draw_num;
	int oldCount;
	int item_num;//���ڐ�
	int count;

	int se_vol;
	int bgm_vol;
	int voice_vol;
private:
	
}; 
