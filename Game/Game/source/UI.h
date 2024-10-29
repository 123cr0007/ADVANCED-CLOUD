#pragma once
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "EnergyGauge.h"
#include "Player.h"
#include "Camera.h"
#define UI_MAX 20
#define ENERGY_UI_MAX 30
#define JUMP_TYPE_A 1
#define UI_GRAPH_W 20
#define UI_GRAPH_H 58
class UI
{
public:
	UI();
	~UI();
	void Initialize(int Screen_W, int Screen_H);
	void Process(VECTOR player_pos, VECTOR camera_pos);
	void SetJumpStatus(int pl_status,int pl_jumptype);
	VECTOR SetPlayerUIPos(VECTOR player_pos, VECTOR camera_pos);
	void SetEnergyUIChip();
	void SetJumpUIPos(int Screen_W, int Screen_H);
	void Render(VECTOR player_pos, int max_en, int now_en,int Jumptime,int JustJumpTime);
	void DrawEnergyIcon();
	void DrawJumpIcon(int i,int status);
	void DrawTimekeeper(int x,int y, int Jumptime, int JustJumpTime);
protected:
	
	int ui_energy[4];//�G�i�W�[�A�C�R��
	int ui_jump_lock[4];//���b�N��� 
	int ui_jump_off[4];//OFF��� 
	int ui_jump_on[4];//ON���
	int ui_tk_bar[4];//�^�C���L�[�p�[
	int ui_tk_icon[3];
	int ui_tk_base[3];
	int ui_button[4];
	
	VECTOR playerUIPos;//�v���C���[�pUI����W
	VECTOR playerUIPos2D;//�v���C���[�pUI����W
	VECTOR jumpUICenterPos;//�W�����v�pUI��z�u���钆�S�_
	VECTOR jumpUIPos[4];//�W�����vUI�\���n�_
	VECTOR ButtonUIPos[4];//�{�^��UI�\���n�_
	VECTOR playerEnUIPos;//�G�l���M�[�|�C���g����W
	
	int size_x, size_y;
	int status[4];//�W�����v�A�C�R���̃��b�N���
	int jumptype;//�ǂ̃W�����v���L����
	int now, max;//�G�l���M�[�p
private:
	EnergyGauge* _en;
};