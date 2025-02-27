#pragma once

class Animation {
public:
	//-----------------------------------------
	// アニメーション関係
	//-----------------------------------------
#define ANIM_RATE_SPEED 0.1f
	
	//アニメーションの初期化
	void AnimInit(int model, int anim_model, int* anim_attach, float* anim_total,
					float* anim_frame, float* anim_rate);

	//アニメーションの更新処理
	void AnimUpdate(int model, int* anim_attach, float* anim_frame, float* anim_rate);

	
	//アニメーションの再生速度を受け取り、アニメーションの時間進行
	void AnimProgress(float playspeed, float* anim_total, float* anim_frame, float* anim_rate, bool loop, bool* end);
	
	//<未実装>
	//アニメーションの切り替え「AnimChanger(1,2) … 1->2に変更」
	void AnimChanger(int m_model, int anim_model, int* anim_attach,
		float* anim_rate, float* anim_frame, int* playing_anim_num, int next_playing_num);


	//使わないアニメーションをデタッチしアタッチ番号を消す処理
	void DetachAnim(int* m_model, int* anim_attach);

	//アニメーションをアタッチして初期化する処理
	void SwitchAnimInit(int m_model, int anim_model, int* anim_attach,
						float* anim_rate, float* anim_frame);

protected:
	
};
