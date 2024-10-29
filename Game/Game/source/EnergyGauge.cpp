#include"EnergyGauge.h"
#include"AppFrame.h"
#include"ApplicationMain.h"

#define ChargeWaitTime 0.1


EnergyGauge* EnergyGauge::EPInstance = nullptr;

EnergyGauge::EnergyGauge()
{
	EPInstance = this;

	energy.max_en = ENERGY_MAX;
	energy.now_en = energy.max_en;
	energy.use_en = 0;

	energy.max_sp = 3;
	energy.use_en = 0;

	waittime = 0;
	oldstatus = FALSE;
	CheckUseEnergy = 0;

	IsCharge = false;
	IsCancel = false;

	for (int i = 0; i < ENERGY_MAX; i++){

		EnergyIcon[i] = 0;
	}

	for (int i = 0; i < energy.max_sp; i++) {

		SpecialEnergyIcon[i] = 0;
	}

	_handle = LoadGraph("res/Energy.png");
}

EnergyGauge::~EnergyGauge()
{

}


bool EnergyGauge::process()
{
	//プレイヤーが回復できるエネルギーがある
	if (Player::GetPLInstance()->GetHealEnergy() == 1) {

		if (energy.now_en < energy.max_en) {
			waittime++;

			int i = energy.now_en;
			if (waittime >= ChargeWaitTime * 60)
			{
				//エネルギー回復
				IsCharge = true;
				energy.now_en += 1;
				EnergyIcon[i] = 1;
				waittime = 0;
			}
		}
	}

	MapChange();

	ChangeEnergy();//デバッグ用
	return true;
}


int EnergyGauge::EnergyCanUse(int use)	//エネルギーが使えるかどうか
{
	//エネルギーが使える
	if(energy.now_en - use >= 0)
	{
		energy.now_en -= use;
		return energy.now_en;
	}
	//エネルギーが使えない
	else
	{
		IsCancel = true;
		energy.now_en = 0;
		return 0;
	}
}


int EnergyGauge::EnergyCharge(int use)	//エネルギーをチャージ
{
	//エネルギーが最大値以下なら
	if (energy.now_en + use <= energy.max_en)
	{
		IsCharge = true;
		energy.now_en += use;
	}
	else
	{
		energy.now_en = energy.max_en;
	}

	return energy.now_en;
}

void EnergyGauge::AddEnergy(int i)
{
	AddMaxEnergy(i);
	AddNowEnergy(i);
}

void EnergyGauge::MapChange()
{
	//いったん全部ゼロに
	for (int i = 0; i < ENERGY_MAX; i++) {

		EnergyIcon[i] = 0;
	}

	int i = 0;
	//エネルギーの数だけ変動部分アイコンを表示
	for (i; i < energy.now_en; i++) {
		EnergyIcon[i] = 1;
	}
}

void EnergyGauge::ChangeEnergy()	//デバッグ用
{
	if (AMG_wp::CheckUseCol == true) 
	{
		int trg = ApplicationMain::GetInstance()->GetTrg();
		if (trg & PAD_INPUT_L)
		{
			energy.max_en++;
			energy.now_en++;
		}
		if (trg & PAD_INPUT_9)
		{
			energy.max_en = 1;
		}
	}
}

