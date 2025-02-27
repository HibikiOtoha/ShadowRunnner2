#pragma once

class Animation {
public:
	//-----------------------------------------
	// �A�j���[�V�����֌W
	//-----------------------------------------
#define ANIM_RATE_SPEED 0.1f
	
	//�A�j���[�V�����̏�����
	void AnimInit(int model, int anim_model, int* anim_attach, float* anim_total,
					float* anim_frame, float* anim_rate);

	//�A�j���[�V�����̍X�V����
	void AnimUpdate(int model, int* anim_attach, float* anim_frame, float* anim_rate);

	
	//�A�j���[�V�����̍Đ����x���󂯎��A�A�j���[�V�����̎��Ԑi�s
	void AnimProgress(float playspeed, float* anim_total, float* anim_frame, float* anim_rate, bool loop, bool* end);
	
	//<������>
	//�A�j���[�V�����̐؂�ւ��uAnimChanger(1,2) �c 1->2�ɕύX�v
	void AnimChanger(int m_model, int anim_model, int* anim_attach,
		float* anim_rate, float* anim_frame, int* playing_anim_num, int next_playing_num);


	//�g��Ȃ��A�j���[�V�������f�^�b�`���A�^�b�`�ԍ�����������
	void DetachAnim(int* m_model, int* anim_attach);

	//�A�j���[�V�������A�^�b�`���ď��������鏈��
	void SwitchAnimInit(int m_model, int anim_model, int* anim_attach,
						float* anim_rate, float* anim_frame);

protected:
	
};
