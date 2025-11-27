#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Player;

class EnemyAttack {
public:
	void Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& size, KamataEngine::Model* model);
	void Update();
	void Move();
	void Draw(KamataEngine::Camera* camera);


	// AABB 取得
	AABB GetAABB() const;

	// 衝突応答
	void OnCollision(const Player* player);

	KamataEngine::Vector3 GetWorldPosition()const;

	bool isFalling() const { return isFalling_; }

private:
	KamataEngine::Vector3 position_;
	KamataEngine::Vector3 size_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Vector3 initialPosition_; 

	bool isFalling_ = false;
	float timer_ = 0.0f;
	float waitTime_ = 0.0f; 

};
