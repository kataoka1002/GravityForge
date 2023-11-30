#include "stdafx.h"
#include "Wall.h"

namespace
{
	const Vector3 WALL_SIZE = { 900.0f,200.0f,10.0f };
}

Wall::~Wall()
{

}

bool Wall::Start()
{
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	//ボックスのコリジョンを作成する。
	m_collisionObject->CreateBox(
		m_position,
		m_rotation,
		WALL_SIZE
	);

	//コリジョンに名前を付ける
	m_collisionObject->SetName("BossFloorWall");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Wall::Update()
{
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position);
}