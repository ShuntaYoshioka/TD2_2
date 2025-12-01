#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Enemy;

class EnemyAttack;

class MapChipField;

class Player {
	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

public:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};

	/// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void InputMove();
	void AnimateTurn();
	/// 更新
	void Update();
	/// 描画
	void Draw();

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	bool onGround_ = true;

	static inline const float kTimeTurn = 0.3f;

	static inline const float kAcceleration = 0.08f;

	static inline const float kAttenuation = 0.2f;

	static inline const float kLimitRunSpeed = 0.3f;
	
	static inline const float kGravityAcceleration = 0.1f;

	static inline const float kLimitFallSpeed = 0.5f;

	static inline const float kJumpAcceleration = 1.0f;

	static inline const float kWidth = 0.8f;

	static inline const float kHeight = 0.8f;

	KamataEngine::Vector3 velocity_ = {};

	KamataEngine::Camera* camera_ = nullptr;

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; };

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void CheckMapCollision(CollisionMapInfo& info);

	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapWall(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	//接地状態に切れ変え処理
	void CheckMapLanding(const CollisionMapInfo& info);
	// ワールド座標取得
	KamataEngine::Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(Enemy* enemy);
	void OnCollision(const EnemyAttack* enemyAttack); 



	// げった
	bool isDead() const { return isDead_; }

	bool isHit() const { return isHit_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

//隙間
	static inline const float kBlank = 0.1f;
	//着地時の速度減衰
	static inline const float kAttenuationLanding = 0.5f;
	//微小な数値
	static inline const float kGroundSearchHeight = 0.1f;
	//着地時の距離減衰率
	static inline const float kAttenuationWall = 0.5f;

	//被弾フラグ
	bool isDead_ = false;

	// 被ダメージ状態管理
	float blinkingTimer;
	bool isHit_ = false;  // ダメージ中か
	float hitTimer_ = 0.0f;          //無敵時間タイマー
	const float kHitDuration = 1.0f; //無敵時間
	KamataEngine::Vector3 knockback_;
};
