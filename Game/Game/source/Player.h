#pragma once
#include "ObjectBase.h"
#include "appframe.h"

#include <string>
#include <vector>

const auto JUMP_CT = 30;

const auto JUMP_TYPE_NONE = -1;     //�W�����v���ĂȂ�
const auto JUMP_TYPE_NORMAL = 1;	//�ʏ�W�����v
const auto JUMP_TYPE_FLOAT =  2;	//�ӂ�W����
const auto JUMP_TYPE_AVOID = 3;	    //����W�����v
const auto JUMP_TYPE_ATK = 4;       //�U���W�����v

const auto CHANGE_GRV_NUM  = 0.25f; //������d�͒l
const auto JUMP_POWER = 10.0f;         //�W�����v��
const auto GRVADD_NUM = 12.0f;      //�ӂ�W�����̒ǉ�����W�����v��
const auto PL_MOVE_SPEED = 10.0f;   //�ړ���

const auto CUPSLE_DPOS = 50.f;      //�J�v�Z���̉��̍����i�v���C���[�̑������炠����ʒu�j
const auto CUPSLE_UPOS = 150.0f;    //�J�v�Z���̏�̍����i�v���C���[�̑������炠����ʒu�j

const auto FRAME_TO_MOVE = 15;      //����W�����v�̎��v���C���[�����t���[���ňړ����邩
const auto MOVIVG_HEIGHT = 200;     //����W�����v�̎��v���C���[���ǂꂾ�������ړ����邩
const auto MOVIVG_FRONT = 100;      //����W�����v�̎��v���C���[���ǂꂾ���O�Ɉړ����邩

const auto CIRCLE_CORNAR = 64;      //�����ɏo���ۉe�̕ӂ̐�

const auto IDLE_DANCE_TIME = 20 * 60;  //�A�C�h���̎��ǂ̂��炢�̎��Ԃœ��ꃂ�[�V�����Đ����邩
const auto ANIM_BREND_TIME = 30;    //���t���[���Ńu�����h�������邩

class Player : public ObjectBase {
public:
    Player();
    virtual ~Player();

    static Player* PLInstance;
    static Player* GetPLInstance() { return (Player*)PLInstance; }

    bool Initialize();  //������
    bool Process() override;//����
    bool Render(int type) override;//�`��
   
    bool ProcessForTitle(); //�^�C�g���p�̏���
    bool RenderForTitle();  //�^�C�g���p�̕`��

    int GetJumpType(){ return jumptype; }       //�W�����v�̎�ނ̃Q�b�^�[
    int GetReleaseJump(){ return release_jump; }//�W�����v�̃����[�X�̃Q�b�^�[
    int GetJumpTime(){ return jumpTime; }       //�W�����v�̎��Ԃ̃Q�b�^�[
    int GetJumpCast() { return JumpCast; }   //�W�����v�̃L���X�g�^�C���̃Q�b�^�[    
    int GetJustTime() { return justTimeMax; } //�W���X�g�̎��Ԃ̃Q�b�^�[
    int GetCheckAtack() { return CheckAtack; }; //�U�����Ă��邩�̃Q�b�^�[
    int GetUseEnergy() { return plUseEnergy; }; //�G�l���M�[���g�����̃Q�b�^�[
    int GetChargeEnergy() { return chargeEnergy; }; //�G�l���M�[���񕜂��邩�̃Q�b�^�[
    int GetHealEnergy() { return checkHealEnergy; }; //�G�l���M�[���񕜂��邩�̃Q�b�^�[
    bool GetChangeMap() { return changeMap; }; //�}�b�v��ς��邩�̃Q�b�^�[
    bool GetGameOver() { return gameOver; } ; //�Q�[���I�[�o�[���̃Q�b�^�[
    bool GetGameClear() { return gameClear; }   ; //�Q�[���N���A���̃Q�b�^�[
    bool GetPlayerDeath() { return dead; }  ; //�v���C���[������ł��邩�̃Q�b�^�[
    VECTOR GetCapsuleTop() {return CapsuleTop; }    //�J�v�Z���̏�̈ʒu�̃Q�b�^�[
    VECTOR GetCapsuleRow() { return CapsuleRow; }   //�J�v�Z���̉��̈ʒu�̃Q�b�^�[

    void SetRestEnergy(int Energy) { restEnergy = Energy; };//�G�l���M�[�̎c��̃Z�b�^�[
    void SetUseEnergy(int Energy) { plUseEnergy = Energy; };  //�G�l���M�[���g�����̃Z�b�^�[
    void SetChargeEnergy(int Energy) { chargeEnergy = Energy; };//�G�l���M�[���񕜂��邩�̃Z�b�^�[
    void SetCheckAtack(int atk) { CheckAtack = atk; };
    void SetChangeMap(bool change) { changeMap = change; };
    void SetIsOperate(bool Operate) { isOperate = Operate; };
    void SetIsStealth(bool Stealth) { isStealth = Stealth; };
    void SetIsComShot(bool iscomshot) { isComShot = iscomshot; };
    void SetCherOpacity(float Opacity) { cherOpacity = Opacity; };

    //���Ă邩�ǂ���
    bool IsJump() { return isJump; };
    bool IsLand() { return isLand; };
    bool IsJust() { return isJust; };
    bool IsWalk() { return isWalk; };
    bool IsIdle() { return isIdle; };
    bool IsComShot() { return isComShot; };
    bool IsDamage() { return isDamage; };

    void SetIsJump(bool isjump) { isJump = isjump; };
    void SetIsLand(bool island) { isLand = island; };
    void SetIsJust(bool isjust) { isJust = isjust; };
    void SetIsWalk(bool iswalk) { isWalk = iswalk; };
    void SetIsIdle(bool isidel) { isIdle = isidel; };
    void SetIsDamage(bool isdamage) { isDamage = isdamage; };
    
 
private:


    //�v���C���[���암��
    bool ProcessOperate();
    //�ړ�����
    bool ProcessMove();

    //����W�����v�̏�����̂Əo��
    bool Vanish(VECTOR GoalPos, VECTOR StartPos);
    bool arrival();
    
    //
    bool ProcessGrv();      //�d�͏���
    bool ProcessCapsule();  //�J�v�Z���̓����蔻�菈��
    bool ProcessRand();     //�n�ʂƂ̓����蔻�菈��

    //�A�j���[�V����
    bool Animetion();
    
    //�e
    bool MakeShadow();

    //�ړ��l���Z�b�g�p
    int grvNum;
    int muveNum;

    //�ړ��ʕۑ��p
    VECTOR MoveV;
    VECTOR oldMoveV;

    //�L�����̓����x
    float   cherOpacity;

    // �R���W�������莞��Y�ʒu(���ʒu�j
    float   _colSubY;

    //�e�֘A
    VECTOR ShadowPos;       //�e���o���ʒu
    VECTOR ShadowCircle[CIRCLE_CORNAR]; 
    bool CheckMakeShadow;   //�e���o�����̔���

    //�d�͊֘A
    float	gravity;        //���݂̏d�͒l
    bool	CheckGravity;   //�d�͂��g�����ǂ���
    bool    OldCheckGrv;    //��������O�̏d�͂��g�����ǂ���
    int		CheckJump;      //�����Ղ����Ă��邩
    int		Srow;           //�d�͂��y�����邩
    int     CheckAtack;     //�U�����Ă邩
    int     FloatingTime;   //�󒆂ɂ��鎞��

    //�J�����֘A
    VECTOR camPos;
    VECTOR camTgtPos;


	//�W�����v�p�ϐ�
    int     jumptype;       //�g�p���̃W�����v�̎��
    int     JumpCast;       //�W�����v�̃L���X�g�^�C��
    int     jumpTime;       //UI�p
    int     justTimeMax;    //������


    //Effekseer�p�n���h��
    int     ef_handle;      //���[�h�p�n���h��
    int     play_Test;      //�Đ��p�n���h��

   
    //��������
    int     CheckAvoid;     //�������Ƃ��̃t�F�[�Y����
    int     AvoidTime;      //���t���[�����������
    VECTOR  PlSize;         //�v���C���[�̃T�C�Y
    VECTOR  OldPos;         //���W�w�肷�邽�߂̂����Ƃ��Ƃ�

    
    //�G�l���M�[�֘A
    int     restEnergy;     //�c��̃G�l���M�[
    int     plUseEnergy;    //�g���G�l���M�[
    int     chargeEnergy;   //�G�l���M�[�񕜗�
    int     maxEnergy;      //���̍ő�G�l���M�[
    int     checkHealEnergy;//�G�l���M�[���񕜂��邩

	//�󋵊m�F�p
    //�T�E���h�Ŏg���悤
    bool    isJump;         
    bool    isLand;
    bool    isJust;
    bool    isWalk;
    bool    isDamage;
    bool    isFloat;
    bool    isIdle;
    bool    isComShot;
    bool    isStealth;
    bool    isHitWall;

    
    //�A�j���[�V�����֘A
    int     attachIndex;    //�A�^�b�`�p�C���f�b�N�X
    float   totalTime;      //�A�j���[�V�����̍��v����
    float   playTime;       //�A�j���[�V�����̍Đ�����
    // ���[�V�����u�����h�p
    float   animBrendRate;     //�u�����h��
    int     animBrendIndex;    //�u�����h�p�C���f�b�N�X
    float   animBrendPlayTime; //�u�����h�Đ�����
    float   animBrendTotalTime;//�u�����h���v����
    int     idleTime;		   //�A�C�h���̎���

    //�e�p�n���h��
    int     shadowHandle;

    //�A�j���[�V�����؂�ւ��p�X�e�[�^�X
     enum class STATUS {
        NONE,
        WAIT,       //1  
        FALL_R,       //2
        FALL_L,       //3
        
        RUN,        //4
        RUN_START,  //5
        RUN_END,    //6

        JUMP_START_R, //7
        JUMP_END_R,   //8
        JUMP_START_L, //7
        JUMP_END_L,   //8

        DOWN,
        IDLE_2,
        _EOT_       //
    };
    STATUS  _status;
    STATUS  oldStatus;
    STATUS  stateBrend;

    bool    ModePhoto;

    //�X�e�[�W�i�s�p
    bool        changeMap;  //�X�e�[�W�i�s���邩
    bool        stageTrg;   //�X�e�[�W���̃g���K�ۑ��p
    std::string mapName;    //�}�b�v�̖��O
    const char* String;     //���O�`��p
    bool        isOperate;  //�X�e�[�W�i�s�̍ۃv���C���[�ɑ��삳���Ȃ�����
    bool        gameOver;   //�Q�[���I�[�o�[�g���K
    bool        dead;       //�v���C���[������ł邩�̃g���K
    bool        gameClear;  //�Q�[���N���A�̃g���K
    int         stepClassroomNum;   //�����𓥂񂾂�

    //�f�o�b�O�p
    int check = 0;
    int CheckNum;
    VECTOR CP;
    unsigned int Cr = GetColor(0, 0, 0);

    VECTOR StPos;
    VECTOR StLen;
};
