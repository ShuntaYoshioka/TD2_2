#include "Enemy.h"
#include "MyMath.h"
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	
	model_ = model;
	camera_ = camera;

	spawnPosition_ = position;
	alive_ = false;
	cleared_ = false;

	isAllEnemiesCleared = false;
	clearCount = 0;


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

}

void Enemy::Update() {

	if (cleared_) {
		return;
	}

	
	if (!alive_) {
		spawnTimer_ += 1.0f;


		if (spawnTimer_ >= spawnDelay_) {
			alive_ = true; 
			worldTransform_.translation_ = spawnPosition_;
		}
		return;
	}
	// 行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定義バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Kill() {
	if (alive_ && !cleared_) {
		alive_ = false;
		cleared_ = true;

		clearCount++;

		// 全部倒したらフラグ
		if (clearCount >= 4) {
			isAllEnemiesCleared = true;
		}

	}
}


void Enemy::disappear() {
	if (!cleared_) {
		alive_ = false;
		spawnTimer_ = 0.0f;
	}
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

	//たおしたら当たり判定を消す
	 if (!alive_ || cleared_) {
		return {
		    {0, 0, 0},
            {0, 0, 0}
        }; 
	}
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

void Enemy::Draw() {
	if (alive_) {
		model_->Draw(worldTransform_, *camera_);
	}
}