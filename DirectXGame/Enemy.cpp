#include "Enemy.h"
#include "MyMath.h"
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	
	model_ = model;
	camera_ = camera;
	walkTimer_ = 0.0f;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

}

void Enemy::Update() {
	// 横移動
	worldTransform_.translation_.x -= kWalkSpeed;

	// ジャンプ挙動用タイマー
	walkTimer_ += 1.0f / 60.0f;

	// 接地判定
	bool onGround = worldTransform_.translation_.y <= kGroundY;

	// 接地していたらジャンプ開始
	if (onGround && velocity_.y <= 0.0f) {
		velocity_.y = kJumpSpeed;
	}

	// 重力適用
	velocity_.y -= kGravity;

	// 位置更新
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	// 地面に到達したら接地
	if (worldTransform_.translation_.y < kGroundY) {
		worldTransform_.translation_.y = kGroundY;
		velocity_.y = 0.0f;
	}

	// 歩行に合わせて回転
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 2.0f * 3.14159265f);

	// 行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定義バッファに転送
	worldTransform_.TransferMatrix();
}


KamataEngine::Vector3 Enemy::GetWorldPosition() {
	KamataEngine::Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}


AABB Enemy::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {
	    worldPos.x - 1.0f / 2.0f,
	    worldPos.y - 1.0f / 2.0f,
	    worldPos.z - 1.0f / 2.0f,
	};

	aabb.max = {
	    worldPos.x + 1.0f / 2.0f,
	    worldPos.y + 1.0f / 2.0f,
	    worldPos.z + 1.0f / 2.0f,
	};
	return aabb;
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }