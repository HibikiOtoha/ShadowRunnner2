#pragma once

//----------------------------------------------------------------
//! GimmickBase.h
//----------------------------------------------------------------

class GimmickBase {
private:

public:
	GimmickBase();

	void GetPlayerPosition();

	//ギミックによる移動量
	Vector3 mov;

	//ギミックのフラグ
	//bool flag;

	Vector3 m_pos_;
};