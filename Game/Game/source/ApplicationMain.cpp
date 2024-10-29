
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeGame.h"
#include "ModeTitle.h"

// 実体
ApplicationMain				g_oApplicationMain;

namespace AMG_wp {
	// Stageオブジェクトの初期化
	Stage* _st = nullptr;
	LoadEnemy* _load = nullptr;

	VECTOR StartPos[STAGE_MAX] = {

		VGet(0, 0, 0),
		VGet(3905, -90, 221),
		VGet(8529, 11058, 67),
		VGet(25051, 32758, 3458),
		VGet(2813, 40990, 21219)
	};

	bool ScenarioTrg[STAGE_MAX] = {

		true,
		false,
		true,
		true,
		false
	};

	int StageNum = 0;
	bool CheckUseCol = false;
	bool CheckModeFhotoTitle = false;
}

ApplicationMain* ApplicationMain::AppMainInstance = nullptr;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	
	AppMainInstance = this;
	// アプリケーショングローバルの初期化
	gGlobal.Init();

	_cnt.heal = 0;
	_cnt.enUP = 0;
	_cnt.oldMaxEn = ENERGY_MAX;

	AMG_wp::_st = new Stage;
	AMG_wp::_load = new LoadEnemy;

	AMG_wp::StageNum = -1;

	// モードの登録
	//ModeServer::GetInstance()->Add(new ModeGame(), 0, "Game");
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");

	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	// サウンドサーバ更新
	gGlobal._sndServer.Update();

	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

void ApplicationMain::ResetStage(){

	int Num = 0;
	for (auto& ite : Stage::GetSTInstance()->GetBlockPositions()) {

		if (ite.modelNo == 21 && ite.AdvanceStage == false) {

			Stage::GetSTInstance()->SetAdvanceStage(Num, true);
		}

		if (ite.modelNo == 0) {

			Stage::GetSTInstance()->SetStepClassroom(Num, true);
		}
		Num++;
	}

	for (int i = 0; i < STAGE_MAX; i++) {

		switch (i)
		{
		case 0:
			AMG_wp::ScenarioTrg[i] = true;
			break;
		case 1:
			AMG_wp::ScenarioTrg[i] = false;
			break;
		case 2:
			AMG_wp::ScenarioTrg[i] = true;
			break;
		case 3:
			AMG_wp::ScenarioTrg[i] = true;
			break; 
		case 4:
			AMG_wp::ScenarioTrg[i] = false;
			break;
		}
	}

	Stage::GetSTInstance()->SetStepClassroomNum(0);
}

