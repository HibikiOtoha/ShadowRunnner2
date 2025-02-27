#pragma once

//----------------------------------------------------------------
//! WireActions.h
//----------------------------------------------------------------

class WireActions :public GimmickBase {
private:

	//ギミックの効果範囲(判定範囲)
#define GIMMICK_RANGE 85.0f
#define GIMMICK_CENTER_RANGE 5.0f

#define WIREMOVE_SPEED_RATE 3.0f

#define RESISTANCE_POWER 0.0001f;

#define COOLTIME_MAX 2

	bool actionflag = false;

	// プレイヤーとこのギミックの距離
	float LinkLine;

	//Y軸の移動量
	float Y_mov;

	//クールタイム
	float cooltime;
	//フレーム数を数える
	float frame_time;

	//(クールタイム)時間計測用
	void SetCoolTime();

	//判定時のプレイヤーからギミックまでの距離
	float mov_length = 0;

	//描画用
	Vector3 cam_rot;

	//ひとつ前のフレームでキー押しているかどうか
	bool beforeUse = false;

public:
	WireActions();

	
	//プレイヤーがギミックの適応内にいるかどうか
	bool CheckOnHitPlayer(Vector3 player_pos);

	//プレイヤーがギミックの中心側にいるかどうか
	bool CheckOnHitCenter(Vector3 player_pos);

	//移動量をすべて削除
	void MoveClear();


	void Update(Vector3 player_pos, Vector3* Gimmick_mov, float* player_rot, Vector3 cam_rot, bool Move_Off);
	void Render(Vector3 player_pos);
};