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
	
	int ui_energy[4];//エナジーアイコン
	int ui_jump_lock[4];//ロック状態 
	int ui_jump_off[4];//OFF状態 
	int ui_jump_on[4];//ON状態
	int ui_tk_bar[4];//タイムキーパー
	int ui_tk_icon[3];
	int ui_tk_base[3];
	int ui_button[4];
	
	VECTOR playerUIPos;//プレイヤー用UI基準座標
	VECTOR playerUIPos2D;//プレイヤー用UI基準座標
	VECTOR jumpUICenterPos;//ジャンプ用UIを配置する中心点
	VECTOR jumpUIPos[4];//ジャンプUI表示地点
	VECTOR ButtonUIPos[4];//ボタンUI表示地点
	VECTOR playerEnUIPos;//エネルギーポイント基準座標
	
	int size_x, size_y;
	int status[4];//ジャンプアイコンのロック状態
	int jumptype;//どのジャンプが有効か
	int now, max;//エネルギー用
private:
	EnergyGauge* _en;
};