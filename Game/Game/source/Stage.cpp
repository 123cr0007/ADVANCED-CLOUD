#include "Stage.h"
#include "ObjectBase.h"
#include "ApplicationMain.h"
#include "EffectManager.h"
// VC++ includeディレクトリに $(ProjectDir)include 指定
#include <nlohmann/json.hpp>	
#include <fstream>

Stage* Stage::STInstance = nullptr;
Stage::Stage() {

	STInstance = this;
	finLoad = false;
	SaveNumCount = 0;
	//非同期設定オン
	SetUseASyncLoadFlag(TRUE);

	// スカイドーム
	_handleSkySphere = MV1LoadModel("res/SkySphere/S_Skysphere.mv1");


	//　マップ
	// 3Dブロックデータ
	HandleJson[0] = MV1LoadModel("res/stage/kyousitu/kyousitu.mv1");//教室
	HandleJson[1] = MV1LoadModel("res/stage/bg_duct/bg_duct.mv1");//ダクト
	HandleJson[2] = MV1LoadModel("res/stage/tetu_asiba/tetu_asiba.mv1");
	HandleJson[3] = MV1LoadModel("res/stage/bg_grating/bg_grating.mv1");
	HandleJson[4] = MV1LoadModel("res/stage/bg_escalator/bg_escalator.mv1");
	HandleJson[5] = MV1LoadModel("res/stage/bg_traffic_light/bg_traffic_light.mv1");
	HandleJson[6] = MV1LoadModel("res/stage/bg_steel_stairs/bg_steel_stairs.mv1");
	HandleJson[7] = MV1LoadModel("res/stage/bg_steel_footing/bg_steel_footing.mv1");
	HandleJson[8] = MV1LoadModel("res/stage/bg_steel_frame_cross/bg_steel_frame_cross.mv1");
	HandleJson[9] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_red.mv1");
	HandleJson[10] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_blue.mv1");
	HandleJson[11] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_green.mv1");
	HandleJson[12] = MV1LoadModel("res/stage/bg_floating_island_lighthouse/bg_floating_island_lighthouse.mv1");
	HandleJson[13] = MV1LoadModel("res/stage/bg_floating_island_small/bg_floating_island_small1.mv1");
	HandleJson[14] = MV1LoadModel("res/stage/bg_telephone_pole/bg_telephone_pole.mv1");
	HandleJson[15] = MV1LoadModel("res/stage/bg_telephone_pole_2/bg_telephone_pole_2.mv1");
	HandleJson[16] = MV1LoadModel("res/stage/bg_huusya_hasira/bg_huusya_hasira.mv1");
	HandleJson[17] = MV1LoadModel("res/stage/bg_huusya_hane/bg_huusya_hane.mv1");//風車のプロペラ
	HandleJson[18] = MV1LoadModel("res/stage/dodai/dodai.mv1");
	HandleJson[19] = MV1LoadModel("res/stage/haguruma/haguruma.mv1");
	HandleJson[20] = MV1LoadModel("res/stage/bg_pedestrian_bridge/bg_pedestrian_bridge.mv1");
	HandleJson[21] = MV1LoadModel("res/stage/savepoint/savepoint.mv1");//セーブポイント
	HandleJson[22] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_coldcolor.mv1");
	HandleJson[23] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_colorful.mv1");
	HandleJson[24] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_watercolor.mv1");
	HandleJson[25] = MV1LoadModel("res/stage/bg_color_cube/bg_color_cube_warmcolor.mv1");

	CollisionJson[0] = MV1SearchFrame(HandleJson[0], "UCX_kyousitu");
	CollisionJson[1] = MV1SearchFrame(HandleJson[1], "UCX_bg_duct");
	CollisionJson[2] = MV1SearchFrame(HandleJson[2], "UCX_tetu_asiba");
	CollisionJson[3] = MV1SearchFrame(HandleJson[3], "UCX_bg_grating");
	CollisionJson[4] = MV1SearchFrame(HandleJson[4], "UCX_bg_escalator");
	CollisionJson[5] = MV1SearchFrame(HandleJson[5], "UCX_bg_traffic_light");
	CollisionJson[6] = MV1SearchFrame(HandleJson[6], "UCX_bg_steel_stairs1");
	CollisionJson[7] = MV1SearchFrame(HandleJson[7], "UCX_bg_steel_footing");
	CollisionJson[8] = MV1SearchFrame(HandleJson[8], "UCX_bg_steel_frame_cross");
	CollisionJson[9] = MV1SearchFrame(HandleJson[9], "UCX_Cube");
	CollisionJson[10] = MV1SearchFrame(HandleJson[10], "UCX_Cube");
	CollisionJson[11] = MV1SearchFrame(HandleJson[11], "UCX_Cube");
	CollisionJson[12] = MV1SearchFrame(HandleJson[12], "UCX_bg_floating_island_lighthouse");
	CollisionJson[13] = /*MV1SearchFrame(HandleJson[13], "UCX_bg_floating_island_small")*/-1;
	CollisionJson[14] = MV1SearchFrame(HandleJson[14], "UCX_bg_telephone_pole_");
	CollisionJson[15] = MV1SearchFrame(HandleJson[15], "UCX_bg_telephone_pole_2");
	CollisionJson[16] = MV1SearchFrame(HandleJson[16], "UCX_bg_huusya_hasira");
	CollisionJson[17] = MV1SearchFrame(HandleJson[17], "UCX_bg_huusya_hane");
	CollisionJson[18] = MV1SearchFrame(HandleJson[17], "UCX_dodai");
	CollisionJson[19] = MV1SearchFrame(HandleJson[17], "UCX_haguruma");
	CollisionJson[20] = MV1SearchFrame(HandleJson[19], "UCX_bg_pedestrian_bridge");
	CollisionJson[21] = -1;
	CollisionJson[22] = MV1SearchFrame(HandleJson[17], "UCX_Cube");
	CollisionJson[23] = MV1SearchFrame(HandleJson[17], "UCX_Cube");
	CollisionJson[24] = MV1SearchFrame(HandleJson[19], "UCX_Cube");
	CollisionJson[25] = MV1SearchFrame(HandleJson[19], "UCX_Cube");

	//酔いこみ
	MainInitialize();
	
	
	// 同期読み込み解除
	SetUseASyncLoadFlag(FALSE);
}

Stage::~Stage() {

	if(_vBlockPos.size() != 0){
		
		_vBlockPos.clear();
	}
}

bool Stage::MainInitialize() {

	// .jsonファイルの読み込みと解析
	{
		//std::ifstream file("res/stage/map_windmill.json");
		std::ifstream file("res/stage/map.json");
		nlohmann::json json;
		file >> json;

		nlohmann::json stage[2];


		stage[0] = json.at("stage1");	//1

		for (auto& data : stage[0]) {	// 複数データを回す
			BLOCKPOS pos;

			pos.AdvanceStage = false;
			pos.stepClassroom = false;
			pos.SaveNum = -1;
			data.at("objectName").get_to(pos.name);
			// UEは左手座標系/Zup →左手座標系/Yup に変換しつつ取得
			data.at("translate").at("x").get_to(pos.x);
			data.at("translate").at("z").get_to(pos.y);
			data.at("translate").at("y").get_to(pos.z);
			pos.z *= -1.f;				// 座標の変換
			data.at("rotate").at("x").get_to(pos.rx);
			data.at("rotate").at("z").get_to(pos.ry);
			data.at("rotate").at("y").get_to(pos.rz);
			pos.rx = DEG2RAD(pos.rx);	// 回転はdegree→radianに
			pos.ry = DEG2RAD(pos.ry);
			pos.rz = DEG2RAD(pos.rz);
			data.at("scale").at("x").get_to(pos.sx);
			data.at("scale").at("z").get_to(pos.sy);
			data.at("scale").at("y").get_to(pos.sz);

			// 名前から使うモデル番号を決める bg_steel_footing
			pos.modelNo = -1;
			if (pos.name == "bg_duct") { pos.modelNo = 1; }
			if (pos.name == "tetu_asiba") { pos.modelNo = 2; }
			if (pos.name == "bg_grating") { pos.modelNo = 3; }
			if (pos.name == "bg_escalator") { pos.modelNo = 4; }
			if (pos.name == "bg_traffic_light") { pos.modelNo = 5; }
			if (pos.name == "bg_steel_stairs") { pos.modelNo = 6; }
			if (pos.name == "bg_steel_footing") { pos.modelNo = 7; }
			if (pos.name == "bg_steel_frame_cross") { pos.modelNo = 8; }
			if (pos.name == "bg_color_cube_r") { pos.modelNo = 9; }
			if (pos.name == "bg_color_cube_b") { pos.modelNo = 10; }
			if (pos.name == "bg_color_cube_g") { pos.modelNo = 11; }
			if (pos.name == "bg_floating_island_lighthouse") { pos.modelNo = 12; }
			if (pos.name == "bg_floating_island_small") { pos.modelNo = 13; }
			if (pos.name == "bg_telephone_pole_") { pos.modelNo = 14; }
			if (pos.name == "bg_telephone_pole_2") { pos.modelNo = 15; }
			if (pos.name == "huusya_hasira") { pos.modelNo = 16; }
			if (pos.name == "huusya_hane") { pos.modelNo = 17; }
			if (pos.name == "dodai") { pos.modelNo = 18; }
			if (pos.name == "haguruma") { pos.modelNo = 19; }
			if (pos.name == "bg_pedestrian_bridge") { pos.modelNo = 20; }
			if (pos.name == "bg_color_cube_coldcolor") { pos.modelNo = 22; }
			if (pos.name == "bg_color_cube_colorful") { pos.modelNo = 23; }
			if (pos.name == "bg_color_cube_watercolor") { pos.modelNo = 24; }
			if (pos.name == "bg_color_cube_warmcolor") { pos.modelNo = 25; }
			/*if (pos.name == "savepoint") {
				pos.modelNo = 21; }*/

			//教室とセーブポイントだけそれぞれトリガーオン 
			if (pos.name == "Newkyo") { 
				pos.modelNo = 0; 
				pos.stepClassroom = true;
			}
			if (pos.name == "kyousitu") {
				pos.modelNo = 0;
				pos.stepClassroom = true;
			}
			if (pos.name == "savepoint") {
				pos.modelNo = 21;
				pos.AdvanceStage = true;
				
				pos.SaveNum = SaveNumCount + 1;
				++SaveNumCount;
			}

			// データをコンテナに追加（モデル番号があれば）
			if (pos.modelNo != -1) {

				//モデルのコピー作る
				pos.handle = MV1DuplicateModel(HandleJson[pos.modelNo]);
				pos.Collision = CollisionJson[pos.modelNo];
				//コリジョン作成＆見えなくする
				MV1SetupCollInfo(pos.handle, pos.Collision, 32, 32, 32);
				MV1SetFrameVisible(pos.handle, pos.Collision, FALSE);
				_vBlockPos.push_back(pos);
			}
		}
	}

	return true;
}

bool Stage::TitleInitialize() {
	// .jsonファイルの読み込みと解析
	{
		std::ifstream file("res/stage/title_stage.json");
		nlohmann::json json;
		file >> json;

		nlohmann::json stage[2];


		stage[0] = json.at("stage1");	//1

		for (auto& data : stage[0]) {	// 複数データを回す
			BLOCKPOS pos;

			pos.AdvanceStage = false;
			pos.stepClassroom = false;
			data.at("objectName").get_to(pos.name);
			// UEは左手座標系/Zup →左手座標系/Yup に変換しつつ取得
			data.at("translate").at("x").get_to(pos.x);
			data.at("translate").at("z").get_to(pos.y);
			data.at("translate").at("y").get_to(pos.z);
			pos.z *= -1.f;				// 座標の変換
			data.at("rotate").at("x").get_to(pos.rx);
			data.at("rotate").at("z").get_to(pos.ry);
			data.at("rotate").at("y").get_to(pos.rz);
			pos.rx = DEG2RAD(pos.rx);	// 回転はdegree→radianに
			pos.ry = DEG2RAD(pos.ry);
			pos.rz = DEG2RAD(pos.rz);
			data.at("scale").at("x").get_to(pos.sx);
			data.at("scale").at("z").get_to(pos.sy);
			data.at("scale").at("y").get_to(pos.sz);

			// 名前から使うモデル番号を決める bg_steel_footing
			pos.modelNo = -1;
			if (pos.name == "bg_duct") { pos.modelNo = 1; }
			if (pos.name == "tetu_asiba") { pos.modelNo = 2; }
			if (pos.name == "bg_grating") { pos.modelNo = 3; }
			if (pos.name == "bg_escalator") { pos.modelNo = 4; }
			if (pos.name == "bg_traffic_light") { pos.modelNo = 5; }
			if (pos.name == "bg_steel_stairs") { pos.modelNo = 6; }
			if (pos.name == "bg_steel_footing") { pos.modelNo = 7; }
			if (pos.name == "bg_steel_frame_cross") { pos.modelNo = 8; }
			if (pos.name == "bg_color_cube_r") { pos.modelNo = 9; }
			if (pos.name == "bg_color_cube_b") { pos.modelNo = 10; }
			if (pos.name == "bg_color_cube_g") { pos.modelNo = 11; }
			if (pos.name == "bg_floating_island_lighthouse") { pos.modelNo = 12; }
			if (pos.name == "bg_floating_island_small") { pos.modelNo = 13; }
			if (pos.name == "bg_telephone_pole_") { pos.modelNo = 14; }
			if (pos.name == "bg_telephone_pole_2") { pos.modelNo = 15; }
			if (pos.name == "huusya_hasira") { pos.modelNo = 16; }
			if (pos.name == "huusya_hane") { pos.modelNo = 17; }
			if (pos.name == "dodai") { pos.modelNo = 18; }
			if (pos.name == "haguruma") { pos.modelNo = 19; }
			if (pos.name == "bg_pedestrian_bridge") { pos.modelNo = 20; }
			if (pos.name == "bg_color_cube_coldcolor") { pos.modelNo = 22; }
			if (pos.name == "bg_color_cube_colorful") { pos.modelNo = 23; }
			if (pos.name == "bg_color_cube_watercolor") { pos.modelNo = 24; }
			if (pos.name == "bg_color_cube_warmcolor") { pos.modelNo = 25; }
		
				//教室とセーブポイントだけそれぞれトリガーオン 
			if (pos.name == "Newkyo") {
				pos.modelNo = 0;
				pos.stepClassroom = true;
			}
			if (pos.name == "kyousitu") {
				pos.modelNo = 0;
				pos.stepClassroom = true;
	}
			if (pos.name == "savepoint") {
				pos.modelNo = 21;
				pos.AdvanceStage = true;
}


			// データをコンテナに追加（モデル番号があれば）
			if (pos.modelNo != -1) {

				//モデルのコピー作る
				pos.handle = MV1DuplicateModel(HandleJson[pos.modelNo]);
				pos.Collision = CollisionJson[pos.modelNo];
				//コリジョン作成＆見えなくする
				MV1SetupCollInfo(pos.handle, pos.Collision, 32, 32, 32);
				MV1SetFrameVisible(pos.handle, pos.Collision, FALSE);
				_vTitlePos.push_back(pos);
			}
		}
	}

	finLoad = true;
	return true;
}


bool Stage::Terminate() {

	for (int i = 0 ; i < OBJ_MAX; i++) {
		HandleJson[i] = -1;
		CollisionJson[i] = -1;
	}

	return true;
}

bool Stage::TitleTerminate() {

	_vTitlePos.clear();

	return true;
}

bool Stage::Process() {

	for (auto& ite : _vBlockPos) {

		//jsonで指定された位置に移動させた後コリジョン情報更新
		MV1SetPosition((ite).handle, VGet((ite).x, (ite).y, (ite).z));
		MV1SetRotationXYZ((ite).handle, VGet(-(ite).rx, (ite).ry, (ite).rz));
		MV1SetScale((ite).handle, VGet((ite).sx, (ite).sy, (ite).sz));
		//ここで更新
		MV1RefreshCollInfo((ite).handle, (ite).Collision);

		//モデルが風車の羽部分なら回転処理を加える
		if ((ite).modelNo == 17) {

			//X軸で回転
			MATRIX rotateX = MGetRotZ(DegToRad(ratate));
			MV1SetFrameUserLocalMatrix((ite).handle, 0, rotateX);

			// 回転角度を更新
			ratate += static_cast<float>(6) / RATATE_RATE;
		}
		if((ite).modelNo == 21)
		{
			if (AMG_wp::StageNum == (ite).SaveNum && EfUseCount >= 90)
			{
				EfUseCount = 0;
				VECTOR ObjPos = VGet(ite.x, ite.y, ite.z);
				EffectManager::GetEFInstance()->Add(EF_CRYSTAL, ObjPos);
			}
		}
	}
	EfUseCount++;
	return true;
}

bool Stage::TitleProcess() {

	for (auto& ite : _vTitlePos) {

		//jsonで指定された位置に移動させた後コリジョン情報更新
		MV1SetPosition((ite).handle, VGet((ite).x, (ite).y, (ite).z));
		MV1SetRotationXYZ((ite).handle, VGet(-(ite).rx, (ite).ry, (ite).rz));
		MV1SetScale((ite).handle, VGet((ite).sx, (ite).sy, (ite).sz));
		//ここで更新
		MV1RefreshCollInfo((ite).handle, (ite).Collision);

		//モデルが風車の羽部分なら回転処理を加える
		if ((ite).modelNo == 17) {

			//X軸で回転
			MATRIX rotateX = MGetRotZ(DegToRad(ratate));
			MV1SetFrameUserLocalMatrix((ite).handle, 0, rotateX);

			// 回転角度を更新
			ratate += static_cast<float>(6) / RATATE_RATE;
		}
	}

	return true;
}

bool Stage::Render() {

	//大きさ変更用
	VECTOR scale = VGet(50, 50, 50);

	// マップモデルを描画する
	// 先にスカイドーム
	MV1SetScale(_handleSkySphere, scale);
	MV1DrawModel(_handleSkySphere);

	int i = 0;
	for (auto& ite : _vBlockPos) {

		MV1DrawModel((ite).handle);
		if (AMG_wp::CheckUseCol == true) {
			if ((ite).name == "SavePoint") {
				DrawFormatString(1800, i * 20, GetColor(255, 0, 0), "save = %d", (ite).AdvanceStage);
				i++;
			}
		}
	}

	return true;
}

bool Stage::RenderForTitle(){

	for (auto& ite : _vTitlePos) {
		MV1DrawModel((ite).handle);
	}
	return false;
}
