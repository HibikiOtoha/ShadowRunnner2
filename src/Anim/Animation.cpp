#include "WinMain.h"
#include "Anim/Animation.h"

//�A�^�b�`���Ċ�����������
void Animation::AnimInit(int model, int anim_model, int* anim_attach, float* anim_total, float* anim_frame, float* anim_rate)
{
	//�A�j���[�V�����̏�����
	// 1: ���f���ɃA�j���[�V�������A�^�b�`���܂�
	*anim_attach = MV1AttachAnim(model, 1, anim_model, FALSE);
	// 2: �A�^�b�`�ԍ����炻�̃A�j���[�V���������t���[�����擾
	*anim_total = MV1GetAttachAnimTotalTime(model, *anim_attach);
	//0�t���[���ڂ���J�n
	*anim_frame = 0.0f;
	//�������񊄍���������
	*anim_rate = 0.0f;
}


//�A�j���[�V�����̑����A
void Animation::AnimUpdate(int model, int* anim_attach, float* anim_frame, float* anim_rate)
{
	//�����A�j���[�V�����̊�����0.0f�ɂȂ�����f�^�b�`���A�^�b�`�ԍ����Ȃ���

		if (*anim_rate <= 0.0f && *anim_attach != -1) {
			//�f�^�b�`���A�A�^�b�`�ԍ����Ȃ���
			DetachAnim(&model, anim_attach);
		}

		//�A�^�b�`����Ă�����̂̂�
		if (*anim_attach >= 0) {
			//�A�j���[�V�����Ƀt���[�������w��
			MV1SetAttachAnimTime(model, *anim_attach, *anim_frame);
			//���̊����ɂ���
			MV1SetAttachAnimBlendRate(model, *anim_attach, *anim_rate);
		}
	
}

//�A�j���[�V�����̐i�s
void Animation::AnimProgress(float playspeed, float* anim_total, float* anim_frame, float* anim_rate, bool loop,bool* end)
{
	*end = false;

	//�A�j���[�V�����Đ�
	*anim_rate += ANIM_RATE_SPEED;

	if (*anim_rate >= 1.0f) {
		//������0.0f~1.0f�ɂ���
		*anim_rate = max(0.0f, min(*anim_rate, 1.0f));

		//�A�j���[�V������i�߂�
		*anim_frame += playspeed;
		//�A�j���[�V���������[�v������
		if (*anim_frame >= *anim_total) {
			if (loop) {
				*anim_frame = 0.0f;
			}
			else {
				*end = true;
			}
		}
		
	}
}

//�A�j���[�V�����̐؂�ւ��uAnimChanger(1,2) �c 1->2�ɕύX�v
void Animation::AnimChanger(int m_model, int anim_model, int* anim_attach, float* anim_rate, float* anim_frame,
	int *playing_anim_num, int next_playing_num)
{
	//�A�j���[�V�����؂�ւ�
	*playing_anim_num = next_playing_num;
	//�A�j���[�V�������A�^�b�`���ď�����
	SwitchAnimInit(m_model,anim_model,anim_attach,anim_rate,anim_frame);
}

//�A�j���[�V�������A�^�b�`���ď��������鏈��
void Animation::SwitchAnimInit(int m_model,int anim_model, int* anim_attach, float* anim_rate,float* anim_frame)
{
	//�A�j���[�V�������A�^�b�`
	*anim_attach = MV1AttachAnim(m_model, 1, anim_model, FALSE);
	//�A�j���[�V�����̃t���[���͂O����
	*anim_frame = 0.0f;
	//�A�j���[�V�����̊������O����
	*anim_rate = 1.0f;
}

//�g��Ȃ��A�j���[�V�������f�^�b�`���A�^�b�`�ԍ�����������
void Animation::DetachAnim(int* m_model,int* anim_attach)
{
	//�A�j���[�V�������f�^�b�`
	MV1DetachAnim(*m_model, *anim_attach);
	//�A�^�b�`�ԍ����Ȃ�
	*anim_attach = -1;
}
