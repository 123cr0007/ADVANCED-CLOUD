#pragma once
#include "AppFrame.h"
#include "Player.h"
#include "ApplicationMain.h"
constexpr auto ENERGY_MAX = 4;//�G�l���M�[�����l

class EnergyGauge
{
public:

	static EnergyGauge* EPInstance;
	static EnergyGauge* GetEPInstance() { return (EnergyGauge*)EPInstance; }

	EnergyGauge();
	~EnergyGauge();

	bool process();

	int EnergyCanUse(int use);//use�Ɏg�p�ʂ���
	int EnergyCharge(int size);//size�ɉ񕜗ʂ���

	int GetHaveNowEnergy() { return energy.now_en; }	//���݂̃G�l���M�[�ʂ��擾
	int GetHaveMaxEnergy() { return energy.max_en; }	//�ő�̃G�l���M�[�ʂ��擾
	bool GetIsCharge() { return IsCharge; };	//�`���[�W�����ǂ���
	bool GetIsCancel() { return IsCancel; }; //�L�����Z�������ǂ���	
	int GetUseEnergy() { return CheckUseEnergy; };	//�g�p�ʂ��擾

	void SetUseEnergy(int CheckUse) { CheckUseEnergy = CheckUse; };	//�g�p�ʂ�ݒ�
	void SetIsCharge(bool Charge) { Charge = IsCharge; };	//�`���[�W�����ǂ�����ݒ�
	void SetIsCancel(bool Cancel) { Cancel = IsCancel; };	//�L�����Z�������ǂ�����ݒ�
	void SetMaxEnergy(int i) { energy.max_en = i; }	//�ő�̃G�l���M�[�ʂ�ݒ�
	void AddMaxEnergy(int i) { energy.max_en += i; }	//�ő�̃G�l���M�[�ʂ�ǉ�
	void AddNowEnergy(int i) { energy.now_en += i; }	//���݂̃G�l���M�[�ʂ�ǉ�

	void AddEnergy(int i);//�G�l���M�[��ǉ�
	


	void MapChange();//�}�b�v�`�F���W���̏���
	void ChangeEnergy();	//�f�o�b�O�p�G�l���M�[�ύX

	

protected:
	struct ENERGY
	{
		int max_en;//�ő�l
		int now_en;//���ݒl
		int use_en;//�g�p��

		int max_sp;//�W���X�g�W�����v�ŃX�g�b�N�ł���ő�l
		int now_sp;//�W���X�g�W�����v�ŃX�g�b�N���ꂽ��
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

