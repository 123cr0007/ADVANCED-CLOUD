#pragma once
#include "AppFrame.h"
#include "Player.h"
#include "ApplicationMain.h"
constexpr auto ENERGY_MAX = 4;//エネルギー初期値

class EnergyGauge
{
public:

	static EnergyGauge* EPInstance;
	static EnergyGauge* GetEPInstance() { return (EnergyGauge*)EPInstance; }

	EnergyGauge();
	~EnergyGauge();

	bool process();

	int EnergyCanUse(int use);//useに使用量を代入
	int EnergyCharge(int size);//sizeに回復量を代入

	int GetHaveNowEnergy() { return energy.now_en; }	//現在のエネルギー量を取得
	int GetHaveMaxEnergy() { return energy.max_en; }	//最大のエネルギー量を取得
	bool GetIsCharge() { return IsCharge; };	//チャージ中かどうか
	bool GetIsCancel() { return IsCancel; }; //キャンセル中かどうか	
	int GetUseEnergy() { return CheckUseEnergy; };	//使用量を取得

	void SetUseEnergy(int CheckUse) { CheckUseEnergy = CheckUse; };	//使用量を設定
	void SetIsCharge(bool Charge) { Charge = IsCharge; };	//チャージ中かどうかを設定
	void SetIsCancel(bool Cancel) { Cancel = IsCancel; };	//キャンセル中かどうかを設定
	void SetMaxEnergy(int i) { energy.max_en = i; }	//最大のエネルギー量を設定
	void AddMaxEnergy(int i) { energy.max_en += i; }	//最大のエネルギー量を追加
	void AddNowEnergy(int i) { energy.now_en += i; }	//現在のエネルギー量を追加

	void AddEnergy(int i);//エネルギーを追加
	


	void MapChange();//マップチェンジ時の処理
	void ChangeEnergy();	//デバッグ用エネルギー変更

	

protected:
	struct ENERGY
	{
		int max_en;//最大値
		int now_en;//現在値
		int use_en;//使用量

		int max_sp;//ジャストジャンプでストックできる最大値
		int now_sp;//ジャストジャンプでストックされた数
	};
	struct ENERGY energy;
	
	bool oldstatus;
	int waittime;

	bool IsCharge;
	bool IsCancel;

private:

	int _handle;
	int CheckUseEnergy;
	int EnergyIcon[10];
	int SpecialEnergyIcon[3];
};

