#pragma once
//----------------------------------------------------------------
//! Stage.h
//----------------------------------------------------------------

class Stage {
private:

public:
	Stage();

	//モデルを入れる変数
	int m_model_;	//モデル
	int sky_model;		//スカイボックスのモデル

	Vector3 m_pos_;   //ステージモデルの位置
	Vector3 sky_rot;	// 空モデルの向き

	Vector3 goal_pos;	//ゴールの位置
	const Vector3 goal_size = {-80,20,110};
	bool goalflag;

#define GIMMICK_MAX 5
	Vector3 Gimmick_pos[GIMMICK_MAX];

	Vector3 player_pos;

	void ReSet();

	void Init(int model,int skymodel);
	void Update(Vector3 player_pos, float* player_rot_y, Vector3* JumpBoard_mov, Vector3* WireAction_mov, Vector3 cam_rot, bool GimmickMov_Off);
	void Render();
	void Exit();
};