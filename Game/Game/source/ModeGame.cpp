
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "ModeEffekseer.h"
#include "ModeMenu.h"
#include "ModeGameOver.h"
#include "LoadEnemy.h"
#include "ModeStory.h"
#include "ModeTutorial.h"

#define SCREEN_W 1920
#define SCREEN_H 1080

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	BgWhite = LoadGraph("res/Scenario/WhiteBack.png");
	_cam = new Camera;
	_pl = new Player;
	_tab = new Fellow;
	_energy = new EnergyGauge;
	_bul = new BulletClass;
	_enemyM = new EnemyManager;
	AMG_wp::_load->GetInstance()->SetJson();
	_ui = new UI;
	_ef = new EffectManager;
	_item = new Item;
	_fade = new Fade;

	_pl->Initialize();
	_cam->Initialize();
	_tab->Initialize(_pl->GetPos(), _pl->GetDir());
	_ui->Initialize(SCREEN_W,SCREEN_H);

	_energy->GetEPInstance()->SetMaxEnergy(ApplicationMain::GetMainInstance()->GetOldMaxEn());

	ModeTitle::GetInstance()->LoadEnd();

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();

	_cam = nullptr;
	_pl = nullptr;
	_tab = nullptr;
	_energy = nullptr;
	_bul = nullptr;
	_enemyM = nullptr;
	_ui = nullptr;
	_ef = nullptr;
	_item = nullptr;
	_fade = nullptr;

	return true;
}

bool ModeGame::Respawn() {
	
	
	_pl = new Player;
	return true;
}

bool ModeGame::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	EnterStory = false;

	AMG_wp::_st->Process();

	_cam->Process();

	VECTOR pos = _pl->GetPos();

	_ui->Process(_pl->GetPos(), _cam->GetCamPos());

	_pl->Process();
	_item->Process();

	_enemyM->Process(_pl->GetPos());

	_ui->SetJumpStatus(_pl->GetReleaseJump(), _pl->GetJumpType());

	// カメラの位置/視点の移動を、プレイヤーキャラの移動量に追従
	VECTOR vPlayerMove;
	vPlayerMove = VSub(_pl->GetPos(), pos);
	_cam->SetCamPos(VAdd(_cam->GetCamPos(), vPlayerMove));
	_cam->SetTgtPos(VAdd(_cam->GetTgtPos(), vPlayerMove));

	//お供処理
	_tab->Process(_pl->GetPos(), _pl->GetDir(), vPlayerMove);



	//エフェクト
	VECTOR TMP = VGet(0, 0, 0);
	_ef->Process(_pl->GetPos(), _enemyM->GetEnPos(),TMP);

	
	//エネルギー関連
	if (_pl->GetJumpType() != JUMP_TYPE_NONE) {
		_energy->EnergyCanUse(_pl->GetUseEnergy());
	}
	_energy->EnergyCharge(_pl->GetChargeEnergy());



	_pl->SetUseEnergy(0);
	_pl->SetChargeEnergy(0);
	_pl->SetRestEnergy(_energy->GetHaveNowEnergy());
	_energy->process();
	

	//モード切り替え
	{
		// ESCキーでメニューを開く
		if (trg & PAD_INPUT_R && AMG_wp::StageNum != 0) {
			ModeMenu* modeMenu = new ModeMenu();
			// ModeGameより上のレイヤーにメニューを登録する
			ModeServer::GetInstance()->Add(modeMenu, 50, "menu");
		}

		if (_pl->GetPos().y <= -10000 || _pl->GetGameOver() == true) {
			ApplicationMain::GetMainInstance()->SetOldMaxEn(_energy->GetHaveMaxEnergy());
			ModeGameOver* modeGameOver = new ModeGameOver();
			// ModeGameより上のレイヤーにメニューを登録する
			ModeServer::GetInstance()->Add(modeGameOver, 1, "gameover");
			ModeServer::GetInstance()->Del(this);
		}
		
		if (_pl->GetChangeMap() == true || _pl->GetGameClear() == true) {
			_pl->SetChangeMap(false);
			_pl->SetIsOperate(true);

			//ストーリーに移る
			if (AMG_wp::ScenarioTrg[AMG_wp::StageNum] == true || AMG_wp::StageNum >= STAGE_MAX) {
				EnterStory = true;
				_fade->FadeModeChange(60, 1);
				ModeStory* modeStory = new ModeStory();
				// ModeGameより上のレイヤーにメニューを登録する
				ModeServer::GetInstance()->Add(modeStory, 99, "story");
			}

			if (AMG_wp::StageNum < STAGE_MAX - 1
				&& AMG_wp::StageNum >= 1) {
				ModeTutorial* modeTutorial = new ModeTutorial();
				//ModeStoryより上のレイヤーにメニューを登録する
				ModeServer::GetInstance()->Add(modeTutorial, 200, "Tutorial");
			}
		}

#ifdef _DEBUG


		if (trg & PAD_INPUT_10) {
			if (AMG_wp::CheckUseCol != true) {

				AMG_wp::CheckUseCol = true;
			}
			else {
				AMG_wp::CheckUseCol = false;
			}
	}

#endif _DEBUG

		if(EnterStory)
		{
			//EnterStory = false;
		}
	}
	return true;
}


bool ModeGame::Render() {
	base::Render();

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ライト設定
	SetUseLighting(TRUE);

	// Fog設定
	/*SetFogEnable(TRUE);
	SetFogColor(255, 255, 255);
	SetFogStartEnd(2000.0f, _cam->GetClipFar());*/

	VECTOR vLightDir = VGet(-1, -1, 0.5f);
#if 1	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(vLightDir);
#endif
#if 0	// ポイントライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos,VGet(0,50.f,0)), 1000.f, 0.f, 0.005f, 0.f);
#endif

	//カメラ設定更新
	_cam->Render();

	//ステージ描画
	AMG_wp::_st->Render();

	

	if (AMG_wp::CheckModeFhotoTitle != true) {
		// 0,0,0を中心に線を引く
		{
			float linelength = 1000.f;
			VECTOR v = { 0, 0, 0 };
			/*	DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
				DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
				DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
			}*/
		}
		//敵
		_enemyM->Render();

		//	プレイヤー描画
		_pl->Render(1);
		_tab->Render(1);


		//effekseer
		_ef->Render();
		//パーティクル
		//_PartServer->Render();
		_item->Render();
		//UI
		_ui->Render(_pl->GetPos(), _energy->GetHaveMaxEnergy(), _energy->GetHaveNowEnergy(), _pl->GetJumpTime(), _pl->GetJustTime());
	}
	if (EnterStory && AMG_wp::StageNum == 0)
	{
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fade->GetAlpha());
		DrawGraph(0, 0, BgWhite, TRUE);
	}
	if (AMG_wp::CheckUseCol == true) {
		DrawFormatString(0, 100, GetColor(0, 0, 0), "HEAL=%d", ApplicationMain::GetMainInstance()->GetCountHeal());
		DrawFormatString(0, 140, GetColor(0, 0, 0), "ENUP=%d", ApplicationMain::GetMainInstance()->GetCountEnUP());
	}
	return true;
}
