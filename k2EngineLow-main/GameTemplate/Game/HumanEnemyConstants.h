#pragma once

namespace 
{
	//体力の最大値
	const float MAX_HP = 100.0f;

	//四つん這いになる体力の境目
	const float KNEELING_HP = 50.0f;

	//死んでしまう体力
	const float DEAD_LINE = 0.0f;

	//追いかけてくるようになる距離
	const float CHASE_RANGE = 1000.0f;

	//追いかけてくるようになる距離(四つん這い)
	const float CHASE_RANGE_KNEELING = 1000.0f;

	//攻撃してくる距離
	const float ATTACK_RANGE = 150.0f;

	//移動速度
	const float MOVE_SPEED = 50.0f;
}