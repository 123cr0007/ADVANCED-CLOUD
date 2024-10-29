#pragma once

#include "ObjectBase.h"
#include "appframe.h"
#include "DxLib.h"
#include "EnergyGauge.h"
#include "Player.h"

#include "Bullet.h"

#include <string>
#include <vector>

//�v���C���[�̑�������ǂꂾ���グ�邩
const auto POS_UP = 130;
//�ǂꂾ���v���C���[�Ɨ�����
const auto DISTANCE = 100;

//class Enemy;
class Fellow : public ObjectBase {
public:

  static Fellow* FLInstance;
  static Fellow* GetFLInstance() { return (Fellow*)FLInstance; }

    Fellow();
    virtual ~Fellow();

    bool Initialize(VECTOR PlPos, VECTOR PlDir);
    bool Process(VECTOR PlPos, VECTOR PlDir, VECTOR MoveValue);
    bool Render(int type) override;

    void PhaseAttack(VECTOR PlPos, VECTOR PlDir);
    void PhaseMove(VECTOR PlPos, VECTOR PlDir);

    VECTOR GetBlPos(int i) { return BLPos[i]; }
    float GetBlR(int i){ return BlRadius[i]; }
    int GetCheckActive(int i) { return CheckBlActive[i]; }
private:

    //�^�[�Q�b�g�̂��
    int RockonHandle;

    //�A�j���[�V�����֘A
    int     _anim1;
    int     _anim2;
    int     _attach_index;
    float   _total_time;
    float   _play_time;

    //�v���C���[�̈ʒu
    VECTOR  CenterPos;

    //�~�܂��Ă鎞�̂����ƃv���C���[�̋���
    VECTOR  NormalLengh;
    float   NormalRad;

    //�ǂ������鏈���p
    int     time;
    int     CheckMove;
    VECTOR  MoveV;

    //�e���ˊ֘A
    int FireRate;
    int FiredNum;
    VECTOR Epos;  //�G�|�W�V����
   

    //�������甭�˂��ꂽ�ʂ̏��
    VECTOR BLPos[BULLET_MAX];
    float BlRadius[BULLET_MAX];
    bool CheckBlActive[BULLET_MAX];

    //�^�[�Q�b�g�̃n���h�����g�����ǂ���
    bool useHandle;

    //�f�o�b�O�p
    VECTOR CheckV;
    VECTOR CheckV2;

    //���N���X�֘A
    BulletClass bullet;
};