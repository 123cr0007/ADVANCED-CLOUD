#include "Item.h"
#include "ObjectBase.h"
#include "ApplicationMain.h"
#include "EnergyGauge.h"
#include "EffectManager.h"
// VC++ includeディレクトリに $(ProjectDir)include 指定
#include <nlohmann/json.hpp>	
#include <fstream>

Item* Item::ItemInstance = nullptr;
Item::Item() {

	ItemInstance = this;

	// 3Dブロックデータ
	HandleJson[0] = MV1LoadModel("res/Item/heal_ling/heal_ling.mv1");
	HandleJson[1] = MV1LoadModel("res/Item/daiya/daiya1.mv1");

	// .jsonファイルの読み込みと解析
	{
		std::ifstream file("res/Item/Item.json");
		nlohmann::json json;
		file >> json;

		nlohmann::json Item = json.at("item");
		for (auto& data : Item) {	// 複数データを回す
			ItemPos pos;
			
			data.at("objectName").get_to(pos.name);

			// UEは左手座標系/Zup →左手座標系/Yup に変換しつつ取得
			data.at("translate").at("x").get_to(pos.x);
			data.at("translate").at("z").get_to(pos.y);
			data.at("translate").at("y").get_to(pos.z);

			// 名前から使うモデル番号を決める
			pos.modelNo = -1;
			if (pos.name == "Sphere") { 
				pos.modelNo = 0; 
				pos.type = 0;
			}
			if (pos.name == "Cube") { 
				pos.modelNo = 1; 
				pos.type = 1;
			}

			//カウント初期化
			pos.cnt = 0;

			pos.use = 1;
			// データをコンテナに追加（モデル番号があれば）
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
	
	//時間が来た
	if (count >= second * fps)
	{
		return true;
	}
	return false;
}

bool Item::Process() {

	VECTOR PlPos = Player::GetPLInstance()->GetPos();

	for (auto& item : _vItem) {

		// itemに対する処理
		VECTOR ItemPos = VGet(item.x, item.y, -item.z);
		VECTOR len = VSub(PlPos, ItemPos);
		
		if (Wait(EFFECT_WAIT, item.cnt)) {
			// エフェクト発生
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
		// プレイヤーとの当たり判定
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
			// ループの末尾の要素を現在の要素で置き換える
			item = _vItem.back();
			//エフェクトに消滅判定を送る

			_vItem.pop_back();  // 末尾の要素を削除
		}
	}

	/*for (auto iteItem = _vItem.begin(); iteItem != _vItem.end(); ++iteItem) {


		VECTOR ItemPos = VGet((*iteItem).x, (*iteItem).y, -(*iteItem).z);
		VECTOR len = VSub(PlPos, ItemPos);
		check[i] = VSize(len);


		
		if (Wait(EFFECT_WAIT, (*iteItem).cnt)) {
			//エフェクト発生
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


		//プレイヤーとの当たり判定
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

	//大きさ変更用
	VECTOR scale = VGet(1, 1, 1);

	int i = 0;
	// 3Dブロックを描画する
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