//Effekseerを使ったエフェクト個体の処理
#include"Effect.h"
#include"EffectManager.h"
#include"AppFrame.h"

Effect::Effect(int Effect,VECTOR pos)
{
	efPos = pos;
	handle = -1;
	isMove = false;
	isPlay = false;
	ef_controller = -1;

	//エフェクトの種類によって読み込むエフェクトを変える
	switch(Effect)
	{
	case EF_JUMP_FLOAT:
		handle = LoadEffekseerEffect("res/Effect/effect_jump_cloud.efkefc", EF_SCALE * 9);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_JUMP_NORMAL:
		handle = LoadEffekseerEffect("res/Effect/effect_jump_star.efkefc", EF_SCALE * 9);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_JUMP_AVOID:
		handle = LoadEffekseerEffect("res/Effect/effect_jump_moon.efkefc", EF_SCALE * 9);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_JUMP_ATTACK:
		handle = LoadEffekseerEffect("res/Effect/effect_jump_sun.efkefc", EF_SCALE * 9);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_ENEMY_BOMB:
		handle = LoadEffekseerEffect("res/Effect/effect_destroy.efkefc", EF_SCALE*2);
		isMove = false;
		ef_controller = EFFECT_ENEMY;
		break;

	case EF_CRYSTAL:
		handle = LoadEffekseerEffect("res/Effect/crystal.efkefc", EF_SCALE);
		isMove = false;
		ef_controller = EFFECT_OTHER;
		break;

	case EF_DIAMOND:
		handle = LoadEffekseerEffect("res/Effect/daiya.efkefc", EF_SCALE * 0.3);
		isMove = false;
		ef_controller = EFFECT_OTHER;
		break;

	case EF_RECOVERY:
		handle = LoadEffekseerEffect("res/Effect/ef_recovery.efkefc", EF_SCALE);
		isMove = false;
		ef_controller = EFFECT_OTHER;
		break;

	case EF_DIFFUSE:
		handle = LoadEffekseerEffect("res/Effect/invincible.efkefc", EF_SCALE);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_TORNADO_MOVE:
		handle = LoadEffekseerEffect("res/Effect/invincible_2.efkefc", EF_SCALE);
		isMove = true;
		ef_controller = EFFECT_PLAYER;
		break;

	case EF_TORNADO:
		handle = LoadEffekseerEffect("res/Effect/invincible_2.efkefc", EF_SCALE);
		isMove = false;
		ef_controller = EFFECT_OTHER;
		break;

	case EF_GATHER:
		handle = LoadEffekseerEffect("res/Effect/invincible_3.efkefc", EF_SCALE);
		isMove = false;
		ef_controller = EFFECT_PLAYER;
		break;
	}

	//エフェクトの読み込みが成功したら再生
	if(handle!=-1)
	{
		isPlay = true;
	}

	//エフェクトの再生用ハンドルへエフェクトのハンドルを渡す
	play_handle = PlayEffekseer3DEffect(handle);
}

Effect::~Effect()
{
	
}

void Effect::Process(VECTOR pl_pos, VECTOR en_pos,VECTOR item_pos)
{
	//エフェクトのハンドルが正常に読み込まれているか
	if(handle >= 0)
	{
		//エフェクトの準備ができているか、再生は終わっているか
		if(IsEffekseer3DEffectPlaying(play_handle) == -1)
		{
			isPlay = false;
		}
		//エフェクト再生可能
		if(isPlay)
		{
			//エフェクトはどれが使うか
			switch(ef_controller)
			{
			case EFFECT_PLAYER:
				//エフェクトは追従するか
				if(isMove) {
					efPos = pl_pos;
				}
				break;
			case EFFECT_ENEMY:
				break;

			case EFFECT_OTHER:
				break;
			}
			//エフェクトの座標をセット
			SetPosPlayingEffekseer3DEffect(play_handle, efPos.x, efPos.y, efPos.z);
		}

	}
}

void Effect::Render()
{
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}
