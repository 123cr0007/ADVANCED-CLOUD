#include "ModeStory.h"
#include "ApplicationMain.h"
#include "ModeGame.h"

ModeStory::ModeStory() {

	_fade = new Fade;

	//とりあえず全部-1入れる
	for (int i = 0; i < SCENARIO_NUM; i++) {
		for (int j = 0; j < SCENARIO_HANDLE_MAX; j++) {

			handleScenario[i][j] = -1;
			soundScenario[i][j] = -1;
		}
		handleBack[i] = -1;
	}
	for (int i = 0; i < SCENARIO_NUM; i++) {
		for (int j = 0; j < INGAME_HANDLE_MAX; j++) {

			handleInGame[i][j] = -1;
			soundInGame[i][j] = -1;
		}
	}

	Time = 0;
	useScenarioNum = 0;
	scenarioHandleNum = 0;
	useInGameNum = 0;
	inGameHandleNum = 0;
	playSound = false;
	playInGame = false;
	playCredit = false;
	oldPlayCredit = false;
	gameEnd = false;
	trg = 0;
	key = 0;

	//オープニング
	handleScenario[0][0] = LoadGraph("res/Scenario/Scenario_E_1.png");
	handleScenario[0][1] = LoadGraph("res/Scenario/Scenario_E_2.png");
	handleScenario[0][2] = LoadGraph("res/Scenario/Scenario_E_3.png");
	handleScenario[0][3] = LoadGraph("res/Scenario/Scenario_E_4.png");
	handleScenario[0][4] = LoadGraph("res/Scenario/Scenario_E_5.png");
	handleScenario[0][5] = LoadGraph("res/Scenario/Scenario_E_6.png");

	//サウンド
	soundScenario[0][0] = LoadSoundMem("sound/amg02V/Voice_E_01.wav");
	soundScenario[0][1] = LoadSoundMem("sound/amg02V/Voice_E_02.wav");
	soundScenario[0][2] = LoadSoundMem("sound/amg02V/Voice_E_03.wav");
	soundScenario[0][3] = LoadSoundMem("sound/amg02V/Voice_E_04.wav");
	soundScenario[0][4] = LoadSoundMem("sound/amg02V/Voice_E_05.wav");
	soundScenario[0][5] = LoadSoundMem("sound/amg02V/Voice_E_06.wav");


	//イベントスチル1
	handleScenario[1][0] = LoadGraph("res/Scenario/Scenario_E_15.png");
	handleScenario[1][1] = LoadGraph("res/Scenario/Scenario_E_16.png");

	//サウンド
	soundScenario[1][0] = LoadSoundMem("sound/amg02V/Voice_E_15.wav");
	soundScenario[1][1] = LoadSoundMem("sound/amg02V/Voice_E_16.wav");

	//イベントスチル2
	handleScenario[2][0] = LoadGraph("res/Scenario/Scenario_E_17.png");
	handleScenario[2][1] = LoadGraph("res/Scenario/Scenario_E_18.png");
	handleScenario[2][2] = LoadGraph("res/Scenario/Scenario_E_19.png");
	handleScenario[2][3] = LoadGraph("res/Scenario/Scenario_E_20.png");
	handleScenario[2][4] = LoadGraph("res/Scenario/Scenario_E_21.png");
	handleScenario[2][5] = LoadGraph("res/Scenario/Scenario_E_22.png");
	handleScenario[2][6] = LoadGraph("res/Scenario/Scenario_E_23.png");
	handleScenario[2][7] = LoadGraph("res/Scenario/Scenario_E_24.png");
	handleScenario[2][8] = LoadGraph("res/Scenario/Scenario_E_25.png");
	handleScenario[2][9] = LoadGraph("res/Scenario/Scenario_E_26.png");
	handleScenario[2][10] = LoadGraph("res/Scenario/Scenario_E_27.png");
	handleScenario[2][11] = LoadGraph("res/Scenario/Scenario_E_28.png");
	handleScenario[2][12] = LoadGraph("res/Scenario/Scenario_E_29.png");
	handleScenario[2][13] = LoadGraph("res/Scenario/Scenario_E_30.png");
	handleScenario[2][14] = LoadGraph("res/Scenario/Scenario_E_31.png");

	//サウンド
	soundScenario[2][0] = LoadSoundMem("sound/amg02V/Voice_E_17.wav");
	soundScenario[2][1] = LoadSoundMem("sound/amg02V/Voice_E_18.wav");
	soundScenario[2][2] = LoadSoundMem("sound/amg02V/Voice_E_19.wav");
	soundScenario[2][3] = LoadSoundMem("sound/amg02V/Voice_E_20.wav");
	soundScenario[2][4] = LoadSoundMem("sound/amg02V/Voice_E_21.wav");
	soundScenario[2][5] = LoadSoundMem("sound/amg02V/Voice_E_22.wav");
	soundScenario[2][6] = LoadSoundMem("sound/amg02V/Voice_E_23.wav");
	soundScenario[2][7] = LoadSoundMem("sound/amg02V/Voice_E_24.wav");
	soundScenario[2][8] = LoadSoundMem("sound/amg02V/Voice_E_25.wav");
	soundScenario[2][9] = LoadSoundMem("sound/amg02V/Voice_E_26.wav");
	soundScenario[2][10] = LoadSoundMem("sound/amg02V/Voice_E_27.wav");
	soundScenario[2][11] = LoadSoundMem("sound/amg02V/Voice_E_28.wav");
	soundScenario[2][12] = LoadSoundMem("sound/amg02V/Voice_E_29.wav");
	soundScenario[2][13] = LoadSoundMem("sound/amg02V/Voice_E_30.wav");
	soundScenario[2][14] = LoadSoundMem("sound/amg02V/Voice_E_31.wav");


	//エンディング後
	handleScenario[3][0] = LoadGraph("res/Scenario/Scenario_E_32.png");
	handleScenario[3][1] = LoadGraph("res/Scenario/Scenario_E_33.png");
	handleScenario[3][2] = LoadGraph("res/Scenario/Scenario_E_34.png");
	handleScenario[3][3] = LoadGraph("res/Scenario/Scenario_E_35.png");


	//サウンド
	soundScenario[3][0] = LoadSoundMem("sound/amg02V/Voice_E_32.wav");
	soundScenario[3][1] = LoadSoundMem("sound/amg02V/Voice_E_33.wav");
	soundScenario[3][2] = LoadSoundMem("sound/amg02V/Voice_E_34.wav");
	soundScenario[3][3] = LoadSoundMem("sound/amg02V/Voice_E_35.wav");

	//以下ゲーム内テキスト

	//オープニング後

	handleInGame[0][0] = LoadGraph("res/Scenario/Scenario_G_14.png");
	handleInGame[0][1] = LoadGraph("res/Scenario/Scenario_G_15.png");
	handleInGame[0][2] = LoadGraph("res/Scenario/Scenario_G_16.png");
	handleInGame[0][3] = LoadGraph("res/Scenario/Scenario_G_17.png");
	handleInGame[0][4] = LoadGraph("res/Scenario/Scenario_G_18.png");
	handleInGame[0][5] = LoadGraph("res/Scenario/Scenario_G_19.png");
	handleInGame[0][6] = LoadGraph("res/Scenario/Scenario_G_20.png");
	handleInGame[0][7] = LoadGraph("res/Scenario/Scenario_G_21.png");
	handleInGame[0][8] = LoadGraph("res/Scenario/Scenario_G_1.png");
	handleInGame[0][9] = LoadGraph("res/Scenario/Scenario_G_2.png");
	handleInGame[0][10] = LoadGraph("res/Scenario/Scenario_G_3.png");
	handleInGame[0][11] = LoadGraph("res/Scenario/Scenario_G_4.png");
	handleInGame[0][12] = LoadGraph("res/Scenario/Scenario_G_5.png");

	//サウンド
	soundInGame[0][0] = LoadSoundMem("sound/amg02V/Voice_E_07.wav");
	soundInGame[0][1] = LoadSoundMem("sound/amg02V/Voice_E_08.wav");
	soundInGame[0][2] = LoadSoundMem("sound/amg02V/Voice_E_09.wav");
	soundInGame[0][3] = LoadSoundMem("sound/amg02V/Voice_E_10.wav");
	soundInGame[0][4] = LoadSoundMem("sound/amg02V/Voice_E_11.wav");
	soundInGame[0][5] = LoadSoundMem("sound/amg02V/Voice_E_12.wav");
	soundInGame[0][6] = LoadSoundMem("sound/amg02V/Voice_E_13.wav");
	soundInGame[0][7] = LoadSoundMem("sound/amg02V/Voice_E_14.wav");
	soundInGame[0][8] = LoadSoundMem("sound/amg02V/Voice_G_01.wav");
	soundInGame[0][9] = LoadSoundMem("sound/amg02V/Voice_G_02.wav");
	soundInGame[0][10] = LoadSoundMem("sound/amg02V/Voice_G_03.wav");
	soundInGame[0][11] = LoadSoundMem("sound/amg02V/Voice_G_04.wav");
	soundInGame[0][12] = LoadSoundMem("sound/amg02V/Voice_G_05.wav");


	//セーブポイント1後
	handleInGame[1][0] = LoadGraph("res/Scenario/Scenario_G_10.png");
	handleInGame[1][1] = LoadGraph("res/Scenario/Scenario_G_11.png");

	//サウンド
	soundInGame[1][0] = LoadSoundMem("sound/amg02V/Voice_G_10.wav");
	soundInGame[1][1] = LoadSoundMem("sound/amg02V/Voice_G_11.wav");


	//セーブポイント2後
	handleInGame[2][0] = LoadGraph("res/Scenario/Scenario_G_6.png");
	handleInGame[2][1] = LoadGraph("res/Scenario/Scenario_G_7.png");
	handleInGame[2][2] = LoadGraph("res/Scenario/Scenario_G_8.png");
	handleInGame[2][3] = LoadGraph("res/Scenario/Scenario_G_9.png");

	//サウンド
	soundInGame[2][0] = LoadSoundMem("sound/amg02V/Voice_G_06.wav");
	soundInGame[2][1] = LoadSoundMem("sound/amg02V/Voice_G_07.wav");
	soundInGame[2][2] = LoadSoundMem("sound/amg02V/Voice_G_08.wav");
	soundInGame[2][3] = LoadSoundMem("sound/amg02V/Voice_G_09.wav");

	handleCredit = LoadGraph("res/Scenario/staffrole.mp4");
	handleSkip = LoadGraph("res/Scenario/skip.png");
	handleFin = LoadGraph("res/Scenario/End_Font_5.png");

	//ストーリー背景
	handleBack[0] = LoadGraph("res/Scenario/event_１.png");
	handleBack[2] = LoadGraph("res/Scenario/event_2.png");
	handleBack[3] = LoadGraph("res/Scenario/kuro.png");
	handleBack[4] = LoadGraph("res/Scenario/End_Font_5.png");

}

bool ModeStory::Initalize() {
	if (!base::Initialize()) { return false; }

	return true;
}

bool ModeStory::Terminate() {
	base::Terminate();

	//全てのハンドル削除
	for (int i = 0; i < SCENARIO_NUM; i++) {
		for (int j = 0; j < SCENARIO_HANDLE_MAX; j++) {
			if (handleScenario[i][j] != -1) {

				DeleteGraph(handleScenario[i][j]);
				DeleteSoundMem(soundScenario[i][j]);
			}
		}
	}
	for (int i = 0; i < SCENARIO_NUM; i++) {
		for (int j = 0; j < SCENARIO_HANDLE_MAX; j++) {
			if(handleInGame[i][j] != -1){
				DeleteGraph(handleInGame[i][j]);
			}
			if(soundInGame[i][j] != -1){
				DeleteSoundMem(soundInGame[i][j]);
			}
		}
	}
	return true;
}

bool ModeStory::Process() {
	base::Process();

	//キー取得
	key = ApplicationMain::GetInstance()->GetKey();
	trg = ApplicationMain::GetInstance()->GetTrg();

	//ストーリー再生したトリガON
	if (AMG_wp::ScenarioTrg[AMG_wp::StageNum] == true) {
		AMG_wp::ScenarioTrg[AMG_wp::StageNum] = false;
	}
	

	//ゲームオーバーがあるなら削除
	if (ModeServer::GetInstance()->Get("gameover") != NULL) {
		
		ModeServer::GetInstance()->Del(this);
	}

	//何番目のシナリオ使うか
	if (oldPlayCredit == true && playCredit == true) {
		//ここクレジット
		if (GetMovieStateToGraph(handleCredit) == 0) {

			playCredit = false;
			handleCredit = -1;
			useScenarioNum++;
			AMG_wp::StageNum++;
		}
	}
	else {
		
		//ここシナリオ
		if (AMG_wp::StageNum < STAGE_MAX && AMG_wp::StageNum > 0) {

			//ゲーム内テキスト
			playInGame = true;
			switch (AMG_wp::StageNum)
			{
			case 2:
				useInGameNum = 1;
				break;
			case 3:
				useInGameNum = 2;
				break;
			}
		}
		else {
			//シナリオ
			if (AMG_wp::StageNum < STAGE_MAX) {
				useScenarioNum = 0;
			}
			else{
				if(useScenarioNum < 2){
					useScenarioNum = 2;
				}
			}
		}
	}

	//ゲーム中再生するモードじゃなければ下のレイヤー止める
	if (playInGame != true) {

		ModeServer::GetInstance()->SkipProcessUnderLayer();
	}
	
	//処理する前にクレジット再生するかのトリガー保存
	oldPlayCredit = playCredit;

	//ゲームクリア
	if (gameEnd == true) {
		_fade->FadeModeChange(60, 2);
		GameFin();
		return true;
	}

	//シナリオ再生
	if (playCredit == false) {

		if (playInGame == true) {
			InGame();
		}
		else {
			//フェードイン
			_fade->FadeModeChange(60, 1);
			Sceneario();
		}
	}
	else {
		
		//クレジット再生
		Credit();
	}

	//フェードアウト
	if(!playInGame){
		_fade->Process();
	}
	return true;
}

bool ModeStory::Render() {
	base::Render();

	// 3D基本設定
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	SetUseBackCulling(FALSE);

	//フェードイン
	if (!playInGame) {
		//SetBlendの場所
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fade->GetAlpha());
	}
	if (gameEnd == true) {
		_fade->FadeModeChange(60, 2);
		DrawGraph(0, 0, handleFin, TRUE);
		
		return true;
	}

	//クレジット再生中じゃなければ描画
	if (playCredit == false){
		if (playInGame == false) {
			//メインストーリー
			DrawGraph(0, 0, handleBack[useScenarioNum], TRUE);
			DrawGraph(0, 0, handleScenario[useScenarioNum][scenarioHandleNum], TRUE);
		}
		else {
		
			//ゲーム内テキストボックス
			DrawGraph(0, 0, handleInGame[useInGameNum][inGameHandleNum], TRUE);	
		}
	}
	else {

		DrawGraph(0, 0, handleCredit, FALSE);
	}

	//とっても便利なSTARTスキップ
	if (playInGame == false) {

		DrawGraph(0, 0, handleSkip, TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	return true;
}

bool ModeStory::Sceneario() {

	//今のハンドル番号でボイス再生されてなければ再生
	if (playSound == false) {
		playSound = true;
		PlaySoundMem(soundScenario[useScenarioNum][scenarioHandleNum], DX_PLAYTYPE_BACK, TRUE);
	}

	//ボイス再生し終わってたらカウントスタート
	if (CheckSoundMem(soundScenario[useScenarioNum][scenarioHandleNum]) == 0) {
		Time++;
	}

	//Aボタンが推されるか、ボイス終わって一秒たったら次のボイスへ
	if (trg & PAD_INPUT_1 || Time > SCENARIO_WAIT_TIME)
	{
		playSound = false;
		StopSoundMem(soundScenario[useScenarioNum][scenarioHandleNum]);
		scenarioHandleNum++;
		Time = 0;
	}

	//次の画像がなければ次のフェーズへ
	if (scenarioHandleNum > SCENARIO_HANDLE_MAX - 1 || handleScenario[useScenarioNum][scenarioHandleNum] == -1
		 || trg & PAD_INPUT_R) {

		//変数初期化
		scenarioHandleNum = 0;
		Time = 0;
		playSound = false;
		
		//モード切り替え
		if (AMG_wp::StageNum > STAGE_MAX) {
			gameEnd = true;
		}
		else {
			
			if (AMG_wp::StageNum < STAGE_MAX) {
				playInGame = true;
			}
			else {
				//クレジット再生ON＆モードゲーム削除
				playCredit = true;
				ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("Game"));
			}
		}
	}

	return true;
}

bool ModeStory::InGame() {

	//今のハンドル番号でボイス再生されてなければ再生
	if (playSound == false) {
		playSound = true;
		PlaySoundMem(soundInGame[useInGameNum][inGameHandleNum], DX_PLAYTYPE_BACK, TRUE);
	}

	//ボイス再生し終わってたらカウントスタート
	if (CheckSoundMem(soundInGame[useInGameNum][inGameHandleNum]) == 0) {
		Time++;
	}

	//Aボタンが推されるか、ボイス終わって一秒たったら次のボイスへ
	if ( Time > SCENARIO_WAIT_TIME )
	{
		playSound = false;
		StopSoundMem(soundInGame[useInGameNum][inGameHandleNum]);
		inGameHandleNum++;
		Time = 0;
	}

	//次の画像がなければ次のフェーズへ
	if (AMG_wp::CheckUseCol == true && trg & PAD_INPUT_L) {

		playSound = false;
		StopSoundMem(soundInGame[useInGameNum][inGameHandleNum]);
		inGameHandleNum++;
		Time = 0;
	}

	//次の画像がなければ次のフェーズへ
	if (inGameHandleNum > INGAME_HANDLE_MAX - 1 || handleInGame[useInGameNum][inGameHandleNum] == -1) {
		
		Time = 0;
		inGameHandleNum = 0;
		ModeServer::GetInstance()->Del(this);
	}
	return true;
}


bool ModeStory::Credit() {

	//クレジット再生
	PlayMovieToGraph(handleCredit);

	return true;
}

bool ModeStory::GameFin(){
	
	//ゲーム終了
	if (trg & PAD_INPUT_1) {
		_fade->FadeModeChange(60, 2);
		//すべてのシナリオ終わってたらタイトルに戻す
		ModeTitle* modeTitle = new ModeTitle;
		ModeServer::GetInstance()->Clear();
		ModeServer::GetInstance()->Add(modeTitle, 1, "Title");
	}
	return false;
}
