#include "DxLib.h"
#include "GameMain.h"
#include "WinMain.h"


char KeyBuffer[256];
int KeyFrame[256];
int MouseLeftFrame;
int MouseRightFrame;

int NowMouseX;
int NowMouseY;
int BeforeMouseX;
int BeforeMouseY;


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Time;

	SetOutApplicationLogValidFlag(FALSE);	//Log��\�����Ȃ�

	//ChangeWindowMode(TRUE);		//�E�B���h�E���[�h
	ChangeWindowMode(FALSE);		//�S��ʃ��[�h

	SetMainWindowText("SHADOW RUNNER");
	SetBackgroundColor(0, 0, 0);	//���̐F����

	SetDoubleStartValidFlag(TRUE);	//��d�N�����\���ǂ���
	SetAlwaysRunFlag(TRUE);	//��A�N�e�B�u���ł�������

	SetGraphMode(SCREEN_W, SCREEN_H, 32);	//�E�B���h�E�T�C�Y�ݒ�(x,y,32bit�J���[)



	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);	// �`����ʂ�ݒ�
	SetTransColor(255, 0, 255);	// �쐬����O���t�B�b�N�n���h���ɓK�p���铧�ߐF��ݒ�
	srand(GetNowCount() % RAND_MAX);

	//�I�v�V���������Ɉړ�
	//SetMouseDispFlag(TRUE);	//�}�E�X�J�[�\���̕\��(�S���)
	//SetMouseDispFlag(FALSE);	//�}�E�X�J�[�\���̕\��(�S���)

	for (int i = 0; i < 256; i++) {
		KeyFrame[i] = 0;
	}
	MouseLeftFrame = 0;
	MouseRightFrame = 0;

	SetCameraNearFar(0.1f, 3000.0f);	// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ�
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// ���ߖ@�J�������Z�b�g�A�b�v����

	SetUseZBuffer3D(TRUE);	// �y�o�b�t�@���g�p���邩�ǂ�����ݒ�
	SetWriteZBuffer3D(TRUE);	// �y�o�b�t�@�ɏ������݂��s�����ǂ�����ݒ�
	//ChangeLightTypeDir(VGet(0.8f, -1.2f, 1.0f));	// �f�t�H���g���C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���
	ChangeLightTypeDir(VGet(1.0f, 1.0f, 0.0f));	// �f�t�H���g���C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���

	GameInit();


	//�G�X�P�[�v���A�E�B���h�E�̕���{�^���Ȃǂ�������Ă��Ȃ��Ƃ�main���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_F4) == 0)
	{
		Time = GetNowCount();
		ClearDrawScreen();	//�`����N���A

		GetHitKeyStateAll(KeyBuffer);	// ���ׂẴL�[�̉�����Ԃ��擾����

		for (int i = 0; i < 256; i++) {
			if (KeyBuffer[i])	KeyFrame[i]++;
			else				KeyFrame[i] = 0;
		}

		if (CheckMouseInput(MOUSE_INPUT_LEFT))	MouseLeftFrame++;
		else										MouseLeftFrame = 0;

		if (CheckMouseInput(MOUSE_INPUT_RIGHT))	MouseRightFrame++;
		else										MouseRightFrame = 0;

		NowMouseX = GetMouseX();
		NowMouseY = GetMouseY();

		GameUpdate();
		GameRender();

		BeforeMouseX = NowMouseX;
		BeforeMouseY = NowMouseY;

		//�I�v�V���������Ɉړ�
		//SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);

		ScreenFlip();
		while (GetNowCount() - Time < 17) {}
		if (ProcessMessage())	break;
		if (CheckHitKey(KEY_INPUT_F4))	break;

	}
	GameExit();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

//---------------------------------------------------------------------------------
//	�L�[�������ꂽ�u�Ԃ��擾����
//---------------------------------------------------------------------------------
bool PushHitKey(int key)
{
	if (KeyFrame[key] == 1) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	�}�E�X��������Ă��邩���擾����
//---------------------------------------------------------------------------------
bool CheckMouseInput(int button)
{
	if (GetMouseInput() & button) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	�}�E�X�������ꂽ�u�Ԃ��擾����
//---------------------------------------------------------------------------------
bool PushMouseInput(int button)
{
	if (button & MOUSE_INPUT_LEFT) {
		if (MouseLeftFrame == 1) {
			return true;
		}
	}
	if (button & MOUSE_INPUT_RIGHT) {
		if (MouseRightFrame == 1) {
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	�}�E�X�̍��W���擾����
//---------------------------------------------------------------------------------
int GetMouseX()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}
int GetMouseY()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}

int GetCenterMouseMoveX()
{
	int mov_x = GetMouseX() - (SCREEN_W / 2);
	return mov_x;
}
int GetCenterMouseMoveY()
{
	int mov_y = GetMouseY() - (SCREEN_H / 2);
	return mov_y;
}

int GetCenterMouseMoveY();
//---------------------------------------------------------------------------------
//	�}�E�X�̈ړ��ʂ��擾����
//---------------------------------------------------------------------------------
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}
//---------------------------------------------------------------------------------
//	�x�����W�A���ɕϊ�����֐�
//---------------------------------------------------------------------------------
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}
//---------------------------------------------------------------------------------
//	���W�A����x�ɕϊ�����֐�
//---------------------------------------------------------------------------------
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}

//----------------------------------------------------------------------------------
// ���[�h�摜�̑傫����ς���(�Q�{)
//----------------------------------------------------------------------------------
int MyLoadGraph(const char* FileName) {
	int handle = LoadGraph(FileName);//�摜�����[�h
	if (handle != -1) {//�摜�̃��[�h�ɐ��������ꍇ
		int SizeX, SizeY;//�摜�T�C�Y���i�[���邽�߂̕ϐ���p��
		GetGraphSize(handle, &SizeX, &SizeY);//���[�h�����摜�̃T�C�Y�擾
		//if ((SizeX >= 10000) && (SizeY >= 10000)) {//�摜�T�C�Y�ŏ�������
		int Nowscreen = GetDrawScreen();//���݂̕`��Ώۉ�ʂ��ꎞ�ۑ�
		SizeX *= 2;//��{�T�C�Y�ɂ������̂�
		SizeY *= 2;
		int minihandle = MakeScreen(SizeX, SizeY, TRUE);//�Q�{�T�C�Y�̃O���t�B�b�N�n���h�����쐬
		if (minihandle == -1) {//�n���h���쐬�Ɏ��s�����ꍇ
			DeleteGraph(handle);//���������[�N���Ȃ��悤�ɁA�{���̃T�C�Y�̃O���t�B�b�N�n���h�����폜��return handle;�ɂ���Ζ{���̃T�C�Y�̂܂ܕԂ����Ƃ��ł��܂�
			return -1;//�G���[����������-1��Ԃ�
		}
		SetDrawScreen(minihandle);//�`��Ώۉ�ʂ��قǍ������{�T�C�Y�̃O���t�B�b�N�n���h���ɕύX
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);//���[�h�����摜���{�T�C�Y�ŕ`��
		DeleteGraph(handle);//�s�v�ɂȂ����{���̃T�C�Y�̃O���t�B�b�N�n���h�����폜
		SetDrawScreen(Nowscreen);//���݂̕`��Ώۉ�ʂ����̉�ʂɖ߂�
		SetCameraNearFar(0.1f, 3000.0f);	// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ�
		SetupCamera_Perspective(TO_RADIAN(45.0f));	// ���ߖ@�J�������Z�b�g�A�b�v����
		SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 10.0f, -10.0f), VGet(0.0f, 0.0f, 0.0f));
		return minihandle;//�k�߂��摜�̃O���t�B�b�N�n���h����Ԃ�
		//}
		//else {
		//	return handle;//�w��T�C�Y���傫���Ȃ��̂ł��̂܂܂̃O���t�B�b�N�n���h����Ԃ�
		//}
	}
	else {//�摜�̃��[�h�Ɏ��s�����ꍇ
		return -1;//�G���[����������-1��Ԃ�
	}

}

void Natura_numbers(float *num)
{
	if (*num < 0) {
		*num *= -1;
	}
}