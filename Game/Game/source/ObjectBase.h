#pragma once
#pragma once
// ObjectBase �N���X�̒�`(Load �֐�)�� TCHAR �^��錾(�g�p)���Ă���̂ŃC���N���[�h
#include <tchar.h>
#include <unordered_map>
#include <tuple>
#include <string>

#include "DxLib.h"
#include "Effect.h"
#include "EffectManager.h"

// Game �N���X�ƍ��킹����
class ObjectBase {
public:
    ObjectBase();
    virtual ~ObjectBase();

    static ObjectBase* Instance;
    static ObjectBase* GetInstance() { return (ObjectBase*)Instance; }

    // �L���b�V�����Ă���n���h�����������
    virtual bool DeleteLoadType();

    // ���@���G���摜�̓ǂݍ��݂͂���͂��Ȃ̂ŋ��ʂ̏����Ƃ��Ď����B
    // type�ɓǂݍ��݃t�@�C���ɉ������ԍ�������B0:�摜�t�@�C�� 1:MV1�t�@�C��
    int Load(const TCHAR* fileName,int type);
    virtual bool Rend(int type);

    // ��������@�ƓG�̃N���X���� override ���āA���ꂼ��N���X��p�̊֐������������肷��
    virtual bool Initialize();
    virtual bool Process();
    // type=0�̎�2D��`��,type=1�̎�3D��`��
    virtual bool Render(int type);

    virtual void End() { valid = false; }

    // �Q�b�^�[
    int GetX() const { return x; }
    int GetY() const { return y; }
    VECTOR GetPos() { return _vPos; };
    VECTOR GetDir() { return _vDir; };
    float GetRadius() const { return _collision_r; };
    bool IsValid() const { return valid; }
    int GetEffect() { return ef_type;}
    int GetRelJump() { return release_jump; }


    //�Z�b�^�[
    void SetPos(VECTOR pos) { _vPos = pos; };
    void SetRelJump(int RelJump) { release_jump = RelJump; };

protected:
    // ��荇�����`��ɕK�v�ȍŒ���̕ϐ�
    // (DrawGraph ���K�v�Ƃ��Ă���)
    int handle;
    int ef_type;
    //�����ȂƂ���Ŏg����ėp�ϐ�
    int x;
    int y;
    VECTOR _vPos;       //
    VECTOR _vDir;       //
    VECTOR CapsuleTop;
    VECTOR CapsuleRow;

    float _collision_r;     //�L�����̔��a
    float ChHight;          //�L�����̍����i�J�v�Z�����Ƃ��́j

    int     release_jump; //�W�����v�̉���� 0:�����A1:3�ډ��(��)

    bool valid;

    std::string fileName;
    
    static std::unordered_map<std::string, std::tuple<int>> handleCache;
};