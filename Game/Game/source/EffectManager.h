#pragma once
#include "AppFrame.h"
#include "ObjectBase.h"

class EffectManager
{
public:
	EffectManager();
	~EffectManager();
	
	static EffectManager* EFInstance;
	static EffectManager* GetEFInstance() { return (EffectManager*)EFInstance; }
	
	void Add(int Effect_type,VECTOR pos);	//�G�t�F�N�g�ǉ�
	void Add(Effect* ef);	//�R���e�i�֒ǉ�����
	void Del(Effect* ef);
	void Process(VECTOR pl_pos,VECTOR en_pos, VECTOR item_pos);
	void Render();

	void IsEnd();//�G�t�F�N�g���I���������ǂ���
	
private:
	bool isProcess;
	std::vector<Effect*> _vEffect;
	std::vector<Effect*> _vAdd;
	std::vector<Effect*> _vDel;
};
