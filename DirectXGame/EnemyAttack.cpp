#include "EnemyAttack.h"
#include "MyMath.h"
#include "Player.h"
#include <cassert>

void EnemyAttack::Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& size, KamataEngine::Model* model) {
	assert(model);
	model_ = model;
	position_ = position;
	initialPosition_ = position;
	size_ = size;

	 waitTime_ = (rand() % 1000) / 100.0f + 2.0f;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position_;


}

void EnemyAttack::Update() {

	  if (!isFalling_) {
		timer_ += 1.0f / 60.0f;

		if (timer_ >= waitTime_) {
			isFalling_ = true; // 自分で落下開始
		}
	}

	worldTransform_.rotation_.z += 0.03f; // 回転させてみる
	if (worldTransform_.rotation_.z > 6.28f) {
		worldTransform_.rotation_.z -= 6.28f;
	}

	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void EnemyAttack::Move() {
	if (isFalling_) {
		position_.y -= 0.1f;
	}

	   // もし y が 18 （地面）についたらリセット
	if (position_.y <= 18.0f) {
		// 元の位置に戻す
		position_ = initialPosition_;

		// 再びランダム時間待ち
		timer_ = 0.0f;
		waitTime_ = (rand() % 1000) / 100.0f + 2.0f; // 次の落下までの時間
		isFalling_ = false;
	}
}

KamataEngine::Vector3 EnemyAttack::GetWorldPosition()const {
	KamataEngine::Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyAttack::Draw(KamataEngine::Camera* camera) { model_->Draw(worldTransform_, *camera); }

AABB EnemyAttack::GetAABB() const {
	return AABB{
	    {position_.x - size_.x * 0.5f, position_.y - size_.y * 0.5f, position_.z - size_.z * 0.5f},

	    {position_.x + size_.x * 0.5f, position_.y + size_.y * 0.5f, position_.z + size_.z * 0.5f}
    };
}

void EnemyAttack::OnCollision(const Player* player) { (void)player; }

// 衝突判定はプレイヤー側で実施
