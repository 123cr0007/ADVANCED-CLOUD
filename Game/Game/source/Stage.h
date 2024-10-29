#pragma once
#pragma once
#include <tchar.h>
#include <unordered_map>
#include <tuple>
#include <string>

#define OBJ_MAX 26

#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )

constexpr auto RATATE_RATE = 3;	//n�b�Ԃŉ���]���邩

class Stage {
public:

	static Stage* STInstance;
	static Stage* GetSTInstance() { return (Stage*)STInstance; }

	Stage();
	virtual ~Stage();

	bool MainInitialize();
	bool Terminate();
	bool Process();
	bool Render();

	bool TitleInitialize();
	bool TitleTerminate();
	bool TitleProcess();
	bool RenderForTitle();

	//�Q�b�^�[

	int GetSHandle() { return _handleMap; };
	int GetSCol() { return _frameMapCollision; };

	int GetJHandle(int i) { return HandleJson[i]; };
	bool GetFinLoad() { return finLoad; };

protected:

	//�}�b�v����Ƃ��n
	//�X�J�C�h�[��
	int _handleSkySphere;
	//�}�b�v�ƒn�ʗp�R���W����
	int _handleMap;
	int _frameMapCollision;

	// 3D�u���b�N�`��p
	int HandleJson[OBJ_MAX];	// ABC
	int CollisionJson[OBJ_MAX];	// ABC

	
	bool ChangeAdvanceStage;
	float ratate;

	// 3D�u���b�N���
	struct BLOCKPOS {
		std::string		name;
		float		x, y, z;	// ���W
		float		rx, ry, rz;	// ��]
		float		sx, sy, sz;	// �g��
		int			modelNo;	// ���f���ԍ�(name�������)
		int			handle;		// �n���h��
		int			Collision;	// �R���W����
		bool		AdvanceStage;	//�X�e�[�W��i�s���邩�̃g���K�[
		bool        stepClassroom;	//�X�^�[�g�ƃS�[���������Ȃ̂Ŕ��肷��悤
		int			SaveNum;	//�Z�[�u�|�C���g�̔ԍ�
	};
	std::vector<BLOCKPOS>	_vBlockPos;
	std::vector<BLOCKPOS>	_vTitlePos;

	int         stepClassroomNum;
	bool finLoad;		//�ǂݍ��݂��I��������ǂ���
	int EfUseCount = 0;	//�G�t�F�N�g�̎g�p��
	int	SaveNumCount;	//�Z�[�u�|�C���g�̔ԍ��m�F�p
public:

	// �Q�b�^�[
	const std::vector<BLOCKPOS>& GetBlockPositions() {

		return _vBlockPos;
	}
	
	int GetStepClassroomNum() {
		return stepClassroomNum;
	}
	

	//�Z�b�^�[
	void SetAdvanceStage(int index, bool newAdvanceStage) {
		if (index >= 0 && index < _vBlockPos.size()) {
			_vBlockPos[index].AdvanceStage = newAdvanceStage;
		}
	}
	void SetStepClassroom(int index, bool newStepClassroom) {
		if (index >= 0 && index < _vBlockPos.size()) {
			_vBlockPos[index].stepClassroom = newStepClassroom;
		}
	}
	void SetAddStepClassroomNum(int addNum) {
		stepClassroomNum += addNum;
	}
	void SetStepClassroomNum(int addNum) {
		stepClassroomNum = addNum;
	}
};