#pragma once
#include "appframe.h"
class Fade
{
public:
	Fade();
	~Fade();
	void Process();
	void Render();
	void ColorMask();	//�J���[�}�X�N
	bool FadeModeChange(int frame,int type);	//�t�F�[�h�̎d����ς���
	bool NotChangeFade(int frame);	//�t�F�[�h���Ȃ�
	bool BlackFadeIn(int frame);	//����ʂ���t�F�[�h�C��
	bool BlackFadeOut(int frame);	//����ʂ���t�F�[�h�A�E�g
	void ResetFadeValue();	//�t�F�[�h�̒l�����Z�b�g
	bool IsFading();	//�t�F�[�h�����ǂ���
	int GetAlpha() { return a; }	//���ߓx���擾
private:
	int r, g, b, a;	//3�F�{���ߓx
	int strR, strG, strB, strA;	//�t�F�[�h�J�n���̃X�e�[�^�X
	int endR, endG, endB, endA;	//�t�F�[�h�I�����̃X�e�[�^�X
	int fadeType;	//0:�ʏ��ʁA1:�t�F�[�h�C���A�Q�t�F�[�h�A�E�g
	int fadeFrame;	//�t�F�[�h�ɂ�����t���[����
	int count;	//�t���[���J�E���g
	int num;	//�����l
};
