#include "WinMain.h"
#include "GameMain.h"
#include "Vector3.h"

#include "Base/GimmickBase.h"
#include "JumpBoard.h"

//ジャンプ力が倍になるエリア(仮)

JumpBoard::JumpBoard()
{
	m_pos_ = { -180,5,0 };

	//m_pos_ = {-543,30,0};

	mov = { 0.0f,1.0f,0.0f };
}

bool JumpBoard::CheckOnHitPlayer(Vector3 player_pos)
{
	bool flag = false;
	if (player_pos.x >= m_pos_.x - GIMMICK_RANGE && player_pos.x <= m_pos_.x + GIMMICK_RANGE &&
		player_pos.z >= m_pos_.z - GIMMICK_RANGE && player_pos.z <= m_pos_.z + GIMMICK_RANGE 
		&& player_pos.y >= m_pos_.y - GIMMICK_RANGE && player_pos.y <= m_pos_.y + GIMMICK_RANGE)
	{
		flag = true;
	}
	
	return flag;
}

void JumpBoard::Update(Vector3 player_pos,Vector3 *Gimmick_mov)
{
	//効果範囲内でジャンプしたら
	if (CheckOnHitPlayer(player_pos))
	{
		mov = { 0.0f,1.0f,0.0f };
		mov.SetLength(GIMMICK_JUMP_POWER);
	}
	else
	{
		mov = { 0.0f,0.0f,0.0f };
		mov.SetLength(ZERO_JUMP_POWER);
	}

	*Gimmick_mov = mov;
}

void JumpBoard::Render(Vector3 player_pos)
{
	DrawCube3D(m_pos_ - Vector3{ GIMMICK_RANGE,0,GIMMICK_RANGE }, m_pos_ + Vector3{ GIMMICK_RANGE,GIMMICK_RANGE * 2,GIMMICK_RANGE }, GetColor(255, 0, 0), GetColor(255, 255, 255), CheckOnHitPlayer(player_pos));
}