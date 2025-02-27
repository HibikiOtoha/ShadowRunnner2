#include "WinMain.h"
#include "GameMain.h"
#include "Vector3.h"
#include <cmath>

#include "Base/GimmickBase.h"
#include "WireActions.h"

//ワイヤーアクション

//#define DEBUG

WireActions::WireActions()
{
	m_pos_ = { 0,0,0 };

	Y_mov = 0;

	mov = { 0,0,0 };

	actionflag = false;

	cooltime = COOLTIME_MAX;
}

bool WireActions::CheckOnHitPlayer(Vector3 player_pos)
{
	bool flag = false;

	// プレイヤーとこのギミックの距離
	LinkLine = GetVector3Distance(m_pos_ , player_pos);

	//プレイヤーからギミックまでの直線距離
	if (LinkLine <= GIMMICK_RANGE)
	{
		flag = true;
	}
	return flag;	
}

bool WireActions::CheckOnHitCenter(Vector3 player_pos)
{
	bool flag = false;

	// プレイヤーとこのギミック中心までの距離
	LinkLine = GetVector3Distance(m_pos_, player_pos);

	//プレイヤーからギミック中心までの直線距離
	if (LinkLine <= GIMMICK_CENTER_RANGE)
	{
		flag = true;
	}
	return flag;
}


void WireActions::Update(Vector3 player_pos, Vector3* Gimmick_mov,float* player_rot_y,Vector3 cam_rot,bool Move_Off)
{
	//クールタイム計算
	SetCoolTime();

	//ギミックが使用可能かどうか
	bool Usable = (!(CheckCameraViewClip(m_pos_)) && cooltime == 0);

	// プレイヤーからギミック座標に向かうベクトルを算出
	Vector3 SubVector = VSub(m_pos_, player_pos);

	//カメラの向きを保存
	cam_rot = this->cam_rot;

	

	if (CheckOnHitPlayer(player_pos) && CheckHitKey(KEY_INPUT_E) && Usable)
	{
		//アクションスタート
		actionflag = true;

		//プレイヤーをギミックに向かせるための方向ベクトル
		Vector3 RotSubVector = VSub(player_pos, m_pos_);

		//プレイヤーをこのギミック側に向ける
		// atan2 を使用して角度を取得
		float Angle = atan2(RotSubVector.x, RotSubVector.z) + DX_PI_F;

		//プレイヤーの向きを変更する(ギミック側に向ける)
		*player_rot_y = TO_DEGREE(Angle);

		//総移動量計算
		mov_length = sqrt((SubVector.x * SubVector.x) + (SubVector.y * SubVector.y) + (SubVector.z * SubVector.z));

		beforeUse = true;
	}
	else
	{
		if (beforeUse == true) {
			cooltime = COOLTIME_MAX;
			
		}
		beforeUse = false;
		actionflag = false;
	}
	

	if (actionflag) {
		//プレイヤーからこのギミックまでの距離算出
		float Length = GetVector3Distance(m_pos_, player_pos);
		Natura_numbers(&Length);

		//プレイヤーからギミックまでの距離が規定スピード以上なら
		if (!CheckOnHitCenter(player_pos)) {
			//規定スピード分ギミック方向に移動し
			mov.x = (SubVector.x / mov_length) * WIREMOVE_SPEED_RATE;
			mov.z = (SubVector.z / mov_length) * WIREMOVE_SPEED_RATE;
			mov.y = (SubVector.y / mov_length) * WIREMOVE_SPEED_RATE;			
		}
		//プレイヤーからギミックまでの距離をそのまま移動し終わった場合
		else {
			//mov = VSub(player_pos, m_pos_);
			//クールタイムをつける
			cooltime = COOLTIME_MAX;
			actionflag = false;
		}
	}
	else {
		mov.y = 0.0f;
		if (Move_Off) {
			mov.clear();
		}
	}

	*Gimmick_mov += mov;
}

void WireActions::Render(Vector3 player_pos)
{
#ifdef DEBUG 
		DrawCapsule3D(m_pos_, m_pos_, GIMMICK_RANGE, 1, GetColor(255, 0, 0), GetColor(255, 255, 255), false);
#endif
		//身長分
		Vector3 length = { 0,5,0 };
		bool active = CheckCameraViewClip(m_pos_);
	DrawCapsule3D(m_pos_ + length, m_pos_ + length, GIMMICK_CENTER_RANGE, 1, GetColor(255, 255, 0), GetColor(200, 200, 200), !active);


	if (actionflag) {
		DrawLine3D(m_pos_, player_pos + Vector3(0.0f, 7.0f, 0.0f), GetColor(100, 100, 255));
	}
}

void WireActions::SetCoolTime()
{
	frame_time++;
	if (frame_time >= 60)
	{
		cooltime--;
		frame_time = 0;
	}

	if (cooltime <= 0) {
		cooltime = 0;
	}
}

void WireActions::MoveClear()
{
	mov.clear();
}