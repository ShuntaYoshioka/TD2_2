#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

struct AABB;

class EnemyAttack {
public:
	void Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& size, KamataEngine::Model* model);
	void Update();
	void Draw(KamataEngine::Camera* camera);


	// AABB 取得
	AABB GetAABB() const;

	//自機に当たったか
	bool IsReached() const { return reached_; }

	KamataEngine::Vector3 GetPosition() const { return position_; }

	bool isFalling() const { return isFalling_; }

private:
	KamataEngine::Vector3 position_;
	KamataEngine::Vector3 size_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	bool reached_ = false;
	bool isFalling_ = false; // 落下開始フラグ

};
