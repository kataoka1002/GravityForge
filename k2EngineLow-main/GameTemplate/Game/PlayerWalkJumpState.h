#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	//前方宣言
	class Player;

	class PlayerWalkJumpState : public IPlayerState
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="player">プレイヤーのインスタンス。</param>
		PlayerWalkJumpState(Player* player) : IPlayerState(player) {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~PlayerWalkJumpState() override;
		/// <summary>
		/// ステート開始時の処理。
		/// </summary>
		void Enter() override;
		/// <summary>
		/// ステートの遷移処理。
		/// </summary>
		/// <returns>遷移するステート</returns>
		IPlayerState* StateChange() override;
		/// <summary>
		/// ステートにおける更新処理。
		/// </summary>
		void Update() override;
	};
}

