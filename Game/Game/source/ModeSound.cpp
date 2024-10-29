#include "ModeSound.h"
#include "ApplicationMain.h"
#include "Player.h"
#include "EnergyGauge.h"
#include "EnemyManager.h"
#include "Camera.h"

int load_mode = 0;                                                                             

bool ModeSound::Initialize() {
	if (!base::Initialize()) { return false; }

	_lodsound = new SoundJson();

	// 音データファイルを読み込む
	_lodsound->Process();

	for (int i = 0; i < 3; i++) {
		jumpcount[i]=0;
	}

	return true;
}

bool ModeSound::Terminate() {
	base::Terminate();

	AMG_wp::StageNum = -1;

	gGlobal._sndServer.Get("BGM_Three.mp3")->Stop();

	gGlobal._sndServer.Get("BGM_BlueRadiance.mp3")->Stop();

	gGlobal._sndServer.Get("BGM_GreenHarmony.mp3")->Stop();
		
	gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->Stop();
	gGlobal._sndServer.Clear();
	delete _lodsound;//ロードjsonを削除

	return true;
}

bool ModeSound::Process()
{
	base::Process();
	if (Camera::GetCamInstance() != nullptr) {
		// カメラ情報をそのまま3Dサウンドのリスナーにセットする
		Set3DSoundListenerPosAndFrontPos_UpVecY(Camera::GetCamInstance()->GetCamPos(), Camera::GetCamInstance()->GetTgtPos());
	}

	
	switch(AMG_wp::StageNum)
	{
	case -1:
		gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->Play();
		break;
	case 0:
		gGlobal._sndServer.Get("BGM_Three.mp3")->Play();
		break;
	case 2:
		gGlobal._sndServer.Get("BGM_BlueRadiance.mp3")->Play();
		break;
	case 3:
		gGlobal._sndServer.Get("BGM_GreenHarmony.mp3")->Play();
		break;
	case 4:
		gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->Play();
		break;
	case 5:
		gGlobal._sndServer.Get("BGM_EmptyEchoes.mp3")->Stop();
		break;
	}
	

	//if(Camera::GetInstance() != nullptr) 
	//{
	//	// カメラ情報をそのまま3Dサウンドのリスナーにセットする
	//	Set3DSoundListenerPosAndFrontPos_UpVecY(Camera::GetInstance()->GetCamPos(), Camera::GetInstance()->GetTgtPos());
	//}

	//プレイヤー
	if(Player::GetPLInstance() != nullptr) {
		for (int i = 0; i < 4; i++) {
			jumpcount[i]++;
		}
		if(Player::GetPLInstance()->IsJump())
		{
			gGlobal._sndServer.Get("SE_JUMP.mp3")->Play();

			int ran = rand()%3;

			if(Player::GetPLInstance()->GetJumpType() == 0) {
				if (jumpcount[0] >= 300) 
				{
					if (ran == 0)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_7.wav")->Play();
						
					}
					if (ran == 1)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_8.wav")->Play();
						
					}
					if (ran == 2)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_9.wav")->Play();
						
					}
					jumpcount[0] = 0;
				}
			}

			if(Player::GetPLInstance()->GetJumpType() == 1) {
				if (jumpcount[1] >= 300)
				{
					if (ran == 0)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_4.wav")->Play();
					}
					if (ran == 1)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_5.wav")->Play();
					}
					if (ran == 2)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_6.wav")->Play();
					}
					jumpcount[1] = 0;
				}
			}

			if(Player::GetPLInstance()->GetJumpType() ==2) {
				if (jumpcount[2] >= 300)
				{
					if (ran == 0)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_1.wav")->Play();
					}
					if (ran == 1)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_2.wav")->Play();
					}
					if (ran == 2)
					{
						gGlobal._sndServer.Get("VOICE_ACT_PL_3.wav")->Play();
					}
					jumpcount[2] = 0;
				}
			}

			if (Player::GetPLInstance()->GetJumpType() == 3) {

				gGlobal._sndServer.Get("SE_COM_Shot.mp3")->Play();

				if (jumpcount[3] >= 300)
				{
					if (ran == 0)
					{
					gGlobal._sndServer.Get("VOICE_ACT_PL_10.wav")->Play();
					}
					if (ran == 1)
					{
						gGlobal._sndServer.Get("VOICE_ACT_COM_1.wav")->Play();
					}
					if (ran == 2)
					{
						gGlobal._sndServer.Get("VOICE_ACT_COM_2.wav")->Play();
					}

					jumpcount[3] = 0;
				}
			}

			Player::GetPLInstance()->SetIsJump(false);
		}

		if(Player::GetPLInstance()->IsLand())
		{
			gGlobal._sndServer.Get("SE_JUMP.mp3")->Play();

			gGlobal._sndServer.Get("VOICE_ACT_EN_3.wav")->Play();
			

			Player::GetPLInstance()->SetIsLand(false);
		}

		if(Player::GetPLInstance()->IsJust())
		{
			gGlobal._sndServer.Get("SE_ENP_Charge.mp3")->Play();
			gGlobal._sndServer.Get("SE_BestJump.mp3")->Play();

			Player::GetPLInstance()->SetIsJust(false);
		}

		if(Player::GetPLInstance()->IsWalk())
		{
			if(!gGlobal._sndServer.Get("SE_WALK.mp3")->IsPlay())
			{
				gGlobal._sndServer.Get("SE_WALK.mp3")->Play();
			}

			Player::GetPLInstance()->SetIsWalk(false);
		}

		if(!Player::GetPLInstance()->IsIdle())
		{
				if(gGlobal._sndServer.Get("SE_WALK.mp3")->IsPlay())
				{
					gGlobal._sndServer.Get("SE_WALK.mp3")->Stop();
				}

			Player::GetPLInstance()->SetIsIdle(false);
		}

		//if(Player::GetPLInstance()->IsComShot())
		//{


		//	Player::GetPLInstance()->SetIsComShot(false);
		//}

		if (Player::GetPLInstance()->IsDamage())
		{
			int ran = rand() % 4;
			if (ran == 0)
			{
				gGlobal._sndServer.Get("VOICE_ACT_COM_3.wav")->Play();
			}
			if (ran == 1)
			{
				gGlobal._sndServer.Get("VOICE_ACT_COM_4.wav")->Play();
			}
			if (ran == 2)
			{
				gGlobal._sndServer.Get("VOICE_ACT_PL_12.wav")->Play();
			}
			if (ran == 3)
			{
				gGlobal._sndServer.Get("VOICE_ACT_PL_13.wav")->Play();
			}
			Player::GetPLInstance()->SetIsDamage(false);
		}
	}

	/*エネルギー*/
	if(EnergyGauge::GetEPInstance() != nullptr)
	{
		bool a = EnergyGauge::GetEPInstance()->GetIsCharge();
		
		if(EnergyGauge::GetEPInstance()->GetIsCharge())
		{
			gGlobal._sndServer.Get("SE_ENP_Charge.mp3")->Play();

			EnergyGauge::GetEPInstance()->SetIsCharge(false);
		}

		if(EnergyGauge::GetEPInstance()->GetIsCancel())
		{
			gGlobal._sndServer.Get("SE_ENP_Cancel.mp3")->Play();

			EnergyGauge::GetEPInstance()->SetIsCancel(false);
		}
		
	}

	/*敵*/
	if(EnemyManager::GetInstance() != nullptr)
	{
		for(int i = 0; i < EnemyManager::GetInstance()->GetEnemy().size(); i++)
		{
			if(EnemyManager::GetInstance()->GetEnemy()[i]->GetIsPatrol())
			{
				int ran = rand() % 4;

				if (ran == 0)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_4.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_4.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				if (ran == 1)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_5.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_5.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				if (ran == 2)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_6.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_6.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				if (ran == 3)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_7.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_7.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}

				EnemyManager::GetInstance()->GetEnemy()[i]->SetIsPatrol(false);
			}

			if(EnemyManager::GetInstance()->GetEnemy()[i]->GetIsFind())
			{
				int ran = rand() % 3;

				if (ran == 0)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_1.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_1.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				if (ran == 1)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_2.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_2.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				if (ran == 2)
				{
					gGlobal._sndServer.Get("VOICE_ACT_EN_3.wav")->Play();
					gGlobal._sndServer.Get("VOICE_ACT_EN_3.wav")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				}
				
				EnemyManager::GetInstance()->GetEnemy()[i]->SetIsFind(false);
			}

			if(EnemyManager::GetInstance()->GetEnemy()[i]->GetIsShot())
			{
				gGlobal._sndServer.Get("SE_EN_Shot.mp3")->Play();
				gGlobal._sndServer.Get("SE_EN_Shot.mp3")->Set3DSound(EnemyManager::GetInstance()->GetEnemy()[i]->GetPos(), 20000.0f);
				EnemyManager::GetInstance()->GetEnemy()[i]->SetIsShot(false);
			}
		}
	}


	//ここやらない
	//システム音
	//if(IsSelect)
	//{
	//	gGlobal._sndServer.Get("SE_SELECT")->Play();
	// 
	//	SetIsSelect(false);
	//}

	//if(IsSelectBack)
	//{
	//	gGlobal._sndServer.Get("SE_SELECT_BACK")->Play();
	// 
	//	SetIsSelectBack(false);
	//}

	//if(IsSelectEnter)
	//{
	//	gGlobal._sndServer.Get("SE_SELECT_ENTER")->Play();
	// 
	//	SetIsSelectEnter(false);
	//}

	return true;
}




bool ModeSound::Render() {
	base::Render();



	
	return true;
}