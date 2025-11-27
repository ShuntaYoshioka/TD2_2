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

	// 位置更新
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;

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