#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	class Player;

	/// <summary>
	/// プレイヤーの攻撃スタンバイ中ステート
	/// </summary>
	class PlayerStandbyState : public IPlayerState
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="player">プレイヤーのインスタンス。</param>
		PlayerStandbyState(Player* player) : IPlayerState(player) {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~PlayerStandbyState() override;
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
