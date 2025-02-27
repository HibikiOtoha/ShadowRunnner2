#include "WinMain.h"
#include "Anim/Animation.h"

//アタッチして割合を初期化
void Animation::AnimInit(int model, int anim_model, int* anim_attach, float* anim_total, float* anim_frame, float* anim_rate)
{
	//アニメーションの初期化
	// 1: モデルにアニメーションをアタッチします
	*anim_attach = MV1AttachAnim(model, 1, anim_model, FALSE);
	// 2: アタッチ番号からそのアニメーションが何フレームか取得
	*anim_total = MV1GetAttachAnimTotalTime(model, *anim_attach);
	//0フレーム目から開始
	*anim_frame = 0.0f;
	//いったん割合を初期化
	*anim_rate = 0.0f;
}


//アニメーションの総数、
void Animation::AnimUpdate(int model, int* anim_attach, float* anim_frame, float* anim_rate)
{
	//もしアニメーションの割合が0.0fになったらデタッチしアタッチ番号をなくす

		if (*anim_rate <= 0.0f && *anim_attach != -1) {
			//デタッチし、アタッチ番号をなくす
			DetachAnim(&model, anim_attach);
		}

		//アタッチされているもののみ
		if (*anim_attach >= 0) {
			//アニメーションにフレーム数を指定
			MV1SetAttachAnimTime(model, *anim_attach, *anim_frame);
			//その割合にする
			MV1SetAttachAnimBlendRate(model, *anim_attach, *anim_rate);
		}
	
}

//アニメーションの進行
void Animation::AnimProgress(float playspeed, float* anim_total, float* anim_frame, float* anim_rate, bool loop,bool* end)
{
	*end = false;

	//アニメーション再生
	*anim_rate += ANIM_RATE_SPEED;

	if (*anim_rate >= 1.0f) {
		//割合を0.0f~1.0fにする
		*anim_rate = max(0.0f, min(*anim_rate, 1.0f));

		//アニメーションを進める
		*anim_frame += playspeed;
		//アニメーションをループさせる
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

//アニメーションの切り替え「AnimChanger(1,2) … 1->2に変更」
void Animation::AnimChanger(int m_model, int anim_model, int* anim_attach, float* anim_rate, float* anim_frame,
	int *playing_anim_num, int next_playing_num)
{
	//アニメーション切り替え
	*playing_anim_num = next_playing_num;
	//アニメーションをアタッチして初期化
	SwitchAnimInit(m_model,anim_model,anim_attach,anim_rate,anim_frame);
}

//アニメーションをアタッチして初期化する処理
void Animation::SwitchAnimInit(int m_model,int anim_model, int* anim_attach, float* anim_rate,float* anim_frame)
{
	//アニメーションをアタッチ
	*anim_attach = MV1AttachAnim(m_model, 1, anim_model, FALSE);
	//アニメーションのフレームは０から
	*anim_frame = 0.0f;
	//アニメーションの割合も０から
	*anim_rate = 1.0f;
}

//使わないアニメーションをデタッチしアタッチ番号を消す処理
void Animation::DetachAnim(int* m_model,int* anim_attach)
{
	//アニメーションをデタッチ
	MV1DetachAnim(*m_model, *anim_attach);
	//アタッチ番号もなし
	*anim_attach = -1;
}
