#include "ModeTutorial.h"

ModeTutorial::ModeTutorial(){

	//いったん画像リセット
	for (int i = 0; i < TUTORIAL_NUM; i++) {
		for (int j = 0; j < TUTORIAL_HANDLE_MAX; j++) {
			handleTutorial[i][j] = -1;
		}
	}
	tutorialHandleNum = 0;



	//画像回り
	handleTutorial[0][0] = LoadGraph("res/Tutorial/description_01.png");
	handleTutorial[0][1] = LoadGraph("res/Tutorial/description_02.png");
	handleTutorial[0][2] = LoadGraph("res/Tutorial/description_03.png");
	handleTutorial[0][3] = LoadGraph("res/Tutorial/description_04.png");
	handleTutorial[0][4] = LoadGraph("res/Tutorial/description_05.png");
	handleTutorial[0][5] = LoadGraph("res/Tutorial/description_06.png");
	handleTutorial[0][6] = LoadGraph("res/Tutorial/description_07.png");
	handleTutorial[0][7] = LoadGraph("res/Tutorial/description_08.png");
	handleTutorial[0][8] = LoadGraph("res/Tutorial/description_09.png");
										 
	handleTutorial[1][0] = LoadGraph("res/Tutorial/description_21.png");
	handleTutorial[1][1] = LoadGraph("res/Tutorial/description_22.png");
										 
	handleTutorial[2][0] = LoadGraph("res/Tutorial/description_31.png");
	handleTutorial[2][1] = LoadGraph("res/Tutorial/description_32.png");
	handleTutorial[2][2] = LoadGraph("res/Tutorial/description_33.png");

}

bool ModeTutorial::Initalize(){
	if (!base::Initialize()) { return false; }
	
	return true;
}

bool ModeTutorial::Terminate(){
	base::Terminate();

	//画像リセット
	for (int i = 0; i < TUTORIAL_NUM; i++) {
		for (int j = 0; j < TUTORIAL_HANDLE_MAX; j++) {
			DeleteGraph(handleTutorial[i][j]);
		}
	}
	return true;
}

bool ModeTutorial::Process(){
	base::Process();

	//下のレイヤー止める
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	//入力関連
	key = ApplicationMain::GetInstance()->GetKey();
	trg = ApplicationMain::GetInstance()->GetTrg();

	//Aボタンで次の画像へ
	if (trg & PAD_INPUT_1) {
		tutorialHandleNum++;
	}

	if (handleTutorial[AMG_wp::StageNum - 1][tutorialHandleNum] == -1
		|| tutorialHandleNum >= TUTORIAL_HANDLE_MAX) {

		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModeTutorial::Render(){
	base::Render();

	if (handleTutorial[AMG_wp::StageNum - 1][tutorialHandleNum] != -1) {
		DrawGraph(0,0, handleTutorial[AMG_wp::StageNum - 1][tutorialHandleNum],TRUE);
	}
	return true;
}


