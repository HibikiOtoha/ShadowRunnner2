#pragma once

//----------------------------------------------------------------
//! JumpBoard.h
//----------------------------------------------------------------

class JumpBoard:public GimmickBase {
private:

	//ギミックの効果範囲
#define GIMMICK_RANGE 3.0f
#define GIMMICK_JUMP_POWER 2.0f
#define ZERO_JUMP_POWER 1.0f



public:
	JumpBoard();

	//プレイヤーがギミックの適応内にいるかどうか
	bool CheckOnHitPlayer(Vector3 player_pos);


	void Update(Vector3 player_pos, Vector3 *Gimmick_mov);
	void Render(Vector3 player_pos);
};