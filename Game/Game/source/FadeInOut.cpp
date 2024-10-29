//�t�F�[�h�C���E�t�F�[�h�A�E�g�����i�������j
#include"FadeInOut.h"
Fade::Fade()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
	fadeType = 0;
	fadeFrame = 0;
	count = 0;
	num = 0;
}

Fade::~Fade()
{

}

void Fade::Process()
{
	switch(fadeType)
	{

	case 1:
		/*strR += num;
		strG += num;
		strB += num;
		strA += num;*/
		a += num;
		break;
	case 2:
		/*strR -= num;
		strG -= num;
		strB -= num;
		strA -= num;*/
		a -= num;
		break;
	}
	if(IsFading())
	{
		++count;
	}
}

void Fade::Render()
{
	
}

void Fade::ColorMask()
{

}

bool Fade::FadeModeChange(int frame,int type)
{
	//�t�F�[�h�̃^�C�v�ŏ����𕪂���
	switch(type)
	{
		//�t�F�[�h���Ȃ�
	case 0:
		NotChangeFade(frame);
		break;

		//�t�F�[�h�C��
	case 1:
		BlackFadeIn(frame);
		break;

		//�t�F�[�h�A�E�g
	case 2:
		BlackFadeOut(frame);
		break;
	}
	return false;
}

bool Fade::NotChangeFade(int frame)
{
	//�O���t�F�[�h�C��������
	if(fadeType == 1)
	{
		a = 255;//�t�F�[�h�C���������̓����x
		fadeType = 0;
		fadeFrame = 0;
		count = 0;
	}
	//�O���t�F�[�h�A�E�g������
	if(fadeType == 2)
	{
		a = 0;//�t�F�[�h�A�E�g�������̓����x
		fadeType = 0;
		fadeFrame = 0;
		count = 0;
	}
	return false;
}

bool Fade::BlackFadeIn(int frame)
{
	//�X�^�[�g���̃X�e�[�^�X�ݒ�
	strR = 0;
	strG = 0;
	strB = 0;
	strA = 255;
	//�t�F�[�h�C��������
	endR = 255;
	endG = 255;
	endB = 255;
	endA = 0;
	//�J�E���g������
	fadeFrame = frame;
	num = 255 / frame;
	count = 0;
	fadeType = 1;
	return true;
}	

bool Fade::BlackFadeOut(int frame)
{
	//�X�^�[�g���̃X�e�[�^�X�ݒ�
	strR = 255;
	strG = 255;
	strB = 255;
	strA = 0;
	//�t�F�[�h�A�E�g������
	endR = 0;
	endG = 0;
	endB = 0;
	endA = 255;
	//�J�E���g������
	fadeFrame = frame;
	num = 255 / frame;
	count = 0;
	fadeType = 2;
	return true;
}

void Fade::ResetFadeValue()	//�t�F�[�h�l���Z�b�g
{
	a = 0;
	fadeType = 0;
	fadeFrame = 0;
	count = 0;
}

bool Fade::IsFading()
{
	//�t�F�[�h��
	if(count < fadeFrame) {
		return true;
	}
	//�t�F�[�h���ĂȂ�
	else
	{
		return false;
	}
}