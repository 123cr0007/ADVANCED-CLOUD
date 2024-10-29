#pragma once
#include "appframe.h"
#include "Stage.h"
#include "LoadEnemy.h"
#include "ApplicationGlobal.h"

constexpr int STAGE_MAX = 5;
class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	static ApplicationMain* AppMainInstance;
	static ApplicationMain* GetMainInstance() { return(ApplicationMain*)AppMainInstance; }
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 1920; }
	virtual int DispSizeH() { return 1080; }

	int GetCountHeal() { return _cnt.heal; }
	int GetCountEnUP() { return _cnt.enUP; }
	int GetOldMaxEn() { return _cnt.oldMaxEn; }

	void SetOldMaxEn(int i) { _cnt.oldMaxEn = i; }
	void CountHeal() { _cnt.heal++; }
	void CountEnUP() { _cnt.enUP++; }

	void ResetStage();

protected:
	struct COUNT
	{
		int heal;
		int enUP;
		int oldMaxEn;
	};
	struct COUNT _cnt;
};

//���O���Ԃ�|�����߃l�[���X�y�[�X�Ɋi�[
namespace AMG_wp {

	extern VECTOR StartPos[STAGE_MAX];
	extern int StageNum;
	//�ǂ̃X�e�[�W�ŃX�g�[���[�Đ����邩�̃g���K�[
	//�Đ��������X�e�[�W�l��true�����
	extern bool ScenarioTrg[STAGE_MAX];

	extern bool CheckUseCol;
	extern bool CheckModeFhotoTitle;

	extern Stage* _st;
	extern LoadEnemy* _load;
}

