#pragma once

#include "DxLib.h"
#include "ObjectBase.h"
#include "FadeInOut.h"
#include "Camera.h"
#include "Player.h"
#include <string>
#include <vector>

////�X�e�[�^�X�Ǘ��p
constexpr auto TITLE_LOGO_X = 960;
constexpr auto TITLE_LOGO_Y = 350;
constexpr auto TITLE_LOGO_SCALE = 1;

constexpr auto TITLE_MENU_ITEM_BAR_X = 960;
constexpr auto TITLE_MENU_ITEM_BAR_Y = 700;
constexpr auto TITLE_MENU_ITEM_BAR_DISTANCE = 100;

constexpr auto CHANGE_WAIT_TIME = 60;

// ���[�h
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

	void CtrlMove(int trg);//choice��ύX����֐�
	void ChangeScene();//��ʂ�ς���֐�
	void LoadEnd();
	void DrawTitleLogo();//�^�C�g�����S�`��p�֐�
	void DrawChoiceItem();//�I��ł��郁�j���[�o�[�����点��
protected:
	Fade* fade;

	int choice;//�I��ł���ԍ�
	int item_num;//���ڐ�
	int count; //�t���[���b
	int screen_type;//�X�N���[���̏��
	int black_clear;//���̓����x
	int back_num;//�w�i�̔ԍ�

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
extern int skip;//�^�C�g���ɓ���̂����߂Ă��H

