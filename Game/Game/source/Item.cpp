#include "Item.h"
#include "ObjectBase.h"
#include "ApplicationMain.h"
#include "EnergyGauge.h"
#include "EffectManager.h"
// VC++ include�f�B���N�g���� $(ProjectDir)include �w��
#include <nlohmann/json.hpp>	
#include <fstream>

Item* Item::ItemInstance = nullptr;
Item::Item() {

	ItemInstance = this;

	// 3D�u���b�N�f�[�^
	HandleJson[0] = MV1LoadModel("res/Item/heal_ling/heal_ling.mv1");
	HandleJson[1] = MV1LoadModel("res/Item/daiya/daiya1.mv1");

	// .json�t�@�C���̓ǂݍ��݂Ɖ��
	{
		std::ifstream file("res/Item/Item.json");
		nlohmann::json json;
		file >> json;

		nlohmann::json Item = json.at("item");
		for (auto& data : Item) {	// �����f�[�^����
			ItemPos pos;
			
			data.at("objectName").get_to(pos.name);

			// UE�͍�����W�n/Zup ��������W�n/Yup �ɕϊ����擾
			data.at("translate").at("x").get_to(pos.x);
			data.at("translate").at("z").get_to(pos.y);
			data.at("translate").at("y").get_to(pos.z);

			// ���O����g�����f���ԍ������߂�
			pos.modelNo = -1;
			if (pos.name == "Sphere") { 
				pos.modelNo = 0; 
				pos.type = 0;
			}
			if (pos.name == "Cube") { 
				pos.modelNo = 1; 
				pos.type = 1;
			}

			//�J�E���g������
			pos.cnt = 0;

			pos.use = 1;
			// �f�[�^���R���e�i�ɒǉ��i���f���ԍ�������΁j
			if (pos.modelNo != -1) {
				_vItem.push_back(pos);
			}
		}
	}
}

Item::~Item() {

}

bool Item::Initialize() {

	return true;
}


bool Item::Terminate() {

	for (int i = 0; i < ITEM_MAX; i++) {
		DeleteGraph(HandleJson[i]);
	}

	return true;
}

bool Item::Wait(int second, int count)
{
	int fps = 60;
	
	//���Ԃ�����
	if (count >= second * fps)
	{
		return true;
	}
	return false;
}

bool Item::Process() {

	VECTOR PlPos = Player::GetPLInstance()->GetPos();

	for (auto& item : _vItem) {

		// item�ɑ΂��鏈��
		VECTOR ItemPos = VGet(item.x, item.y, -item.z);
		VECTOR len = VSub(PlPos, ItemPos);
		
		if (Wait(EFFECT_WAIT, item.cnt)) {
			// �G�t�F�N�g����
			if (item.type == 0) {
				EffectManager::GetEFInstance()->Add(EF_DIAMOND, ItemPos);
			}
			else {
				EffectManager::GetEFInstance()->Add(EF_DIAMOND, ItemPos);
			}
			item.cnt = 0;
		}
		else {
			item.cnt++;
		}

		CheckHitPointCapsule(Player::GetPLInstance()->GetCapsuleRow(), Player::GetPLInstance()->GetCapsuleTop(),
			ItemPos, Player::GetPLInstance()->GetRadius(),100);
		// �v���C���[�Ƃ̓����蔻��
		if (/*VSize(len) <= 100*/CheckHitPointCapsule(Player::GetPLInstance()->GetCapsuleRow(), Player::GetPLInstance()->GetCapsuleTop(),
			ItemPos, Player::GetPLInstance()->GetRadius(),100)) {
			if (item.type == 0) {

				EnergyGauge::GetEPInstance()->AddEnergy(2);
				ApplicationMain::GetMainInstance()->CountHeal();
			}
			else {
				EnergyGauge::GetEPInstance()->AddEnergy(2);
				ApplicationMain::GetMainInstance()->CountEnUP();
				VECTOR ObjPos = VGet(item.x, item.y+100, item.z);
				EffectManager::GetEFInstance()->Add(EF_TORNADO_MOVE, ObjPos);
				int ran = rand() % 3;
				if(ran == 0)
				{
					gGlobal._sndServer.Get("VOICE_ACT_PL_14.wav")->Play();
				}
				if(ran == 1)
				{
					gGlobal._sndServer.Get("VOICE_ACT_COM_5.wav")->Play();
				}
				if(ran == 2)
				{
					gGlobal._sndServer.Get("VOICE_ACT_COM_6.wav")->Play();
				}
			}
			// ���[�v�̖����̗v�f�����݂̗v�f�Œu��������
			item = _vItem.back();
			//�G�t�F�N�g�ɏ��Ŕ���𑗂�

			_vItem.pop_back();  // �����̗v�f���폜
		}
	}

	/*for (auto iteItem = _vItem.begin(); iteItem != _vItem.end(); ++iteItem) {


		VECTOR ItemPos = VGet((*iteItem).x, (*iteItem).y, -(*iteItem).z);
		VECTOR len = VSub(PlPos, ItemPos);
		check[i] = VSize(len);


		
		if (Wait(EFFECT_WAIT, (*iteItem).cnt)) {
			//�G�t�F�N�g����
			if ((*iteItem).type == 0) {

				EffectManager::GetEFInstance()->Add(EF_RECOVERY, ItemPos);
			}
			else {

				EffectManager::GetEFInstance()->Add(EF_DIAMOND, ItemPos);
			}

			(*iteItem).cnt = 0;
		}
		else {

			(*iteItem).cnt++;
		}


		//�v���C���[�Ƃ̓����蔻��
		if (VSize(len) <= 100) {
			if ((*iteItem).type == 0) {

				Player::GetPLInstance()->SetChargeEnergy(3);
				iteItem = _vItem.erase(iteItem);
			}
			else {

				Player::GetPLInstance()->SetChargeEnergy(1);
				iteItem = _vItem.erase(iteItem);
			}
		}
		i++;
	}
*/
	return true;
}

bool Item::Render() {

	//�傫���ύX�p
	VECTOR scale = VGet(1, 1, 1);

	int i = 0;
	// 3D�u���b�N��`�悷��
	for (auto ite = _vItem.begin(); ite != _vItem.end(); ++ite) {

		if ((*ite).use != 0) {
			MV1SetPosition(HandleJson[(*ite).modelNo], VGet((*ite).x, (*ite).y, -(*ite).z));
			//MV1DrawModel(HandleJson[(*ite).modelNo]);
			/*DrawBillboard3D(VGet((*ite).x, (*ite).y, (*ite).z),
				0.5f, 0.5f, 50, 0.0f, HandleJson[(*ite).modelNo], FALSE);*/
		}

		if (AMG_wp::CheckUseCol == true) {
			DrawFormatString(0, 800 + i * 20, GetColor(0, 255, 0), "pos.x = %f,pos.y = %f,pos.z = %f",
				(*ite).x, (*ite).y, (*ite).z);
			DrawFormatString(500, 800 + i * 20, GetColor(0, 255, 0), "use = %d",
				(*ite).use);
			if (check[i] != 0) {
				DrawFormatString(600, 800 + i * 20, GetColor(0, 0, 255), "check = %f",
					check[i]);
			}
		}

		i++;
	}

	return true;
}