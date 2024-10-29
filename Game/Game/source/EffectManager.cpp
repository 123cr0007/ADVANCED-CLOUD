//エフェクトコンテナのマネージャー
#include"EffectManager.h"
#include "Effect.h"

EffectManager* EffectManager::EFInstance = nullptr;
EffectManager::EffectManager()
{
	EFInstance = this;
	isProcess = false;
}

EffectManager::~EffectManager()
{

}

void EffectManager::Add(int Effect_type, VECTOR pos)
{
	Effect* _ef = new Effect(Effect_type,pos);

	if(!isProcess)
	{
		_vEffect.push_back(_ef);
	}
	else
	{
		_vAdd.push_back(_ef);
	}
}

void EffectManager::Add(Effect* ef)
{
	if(!isProcess)
	{
		_vEffect.push_back(ef);
	}
	else
	{
		_vAdd.push_back(ef);
	}
}

void EffectManager::Del(Effect* ef)
{
	if(isProcess)
	{
		for(auto&& ite:_vEffect )
		{
			if(ite == ef)
			{

			}
			else
			{
				++ite;
			}
		}
	}
	else
	{
		_vDel.push_back(ef);
	}
}

void EffectManager::Process(VECTOR pl_pos, VECTOR en_pos, VECTOR item_pos)
{
	for(auto&& ite : _vAdd)
	{
		Add(ite);
	}

	_vAdd.clear();
	isProcess = true;

	for(auto&& ite : _vEffect)
	{
		if(!(ite)->isPlay)
		{
			
		}
		else
		{
			(ite)->Process(pl_pos, en_pos, item_pos);
			
			
		}
	}

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	isProcess = false;

	for(auto ite = _vDel.begin(); ite != _vDel.end(); ++ite)
	{
		Del((*ite));
	}
	_vDel.clear();
}

void EffectManager::Render()
{
	for(auto ite : _vEffect)
	{
		if(ite->isPlay)
		{
			(ite)->Render();
		}
	}
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void EffectManager::IsEnd()
{
	for(auto ite = _vEffect.begin(); ite != _vEffect.end(); ++ite)
	{
		if((*ite)->isPlay)
		{
			Del(*ite);
		}
	}
}

