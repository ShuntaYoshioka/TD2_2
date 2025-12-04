#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Player;

class Enemy {
public:
	/// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

  bool IsAlive() const { return alive_; }
	bool IsCleared() const { return cleared_; }
  static float GetSpawnTimer() { return spawnTimer_; }
  static float GetSpawnDelay() { return spawnDelay_; }


	void Kill(); 
	void disappear();
	void MarkCleared() { cleared_ = true; }

	KamataEngine::Vector3 velocity_ = {};
	// AABBを取得
	AABB GetAABB();
	KamataEngine::Vector3 GetWorldPosition();

	// 衝突応答
	void OnCollision(const Player* player);

	inline static int clearCount = 0; // 衝突で増える
	inline static bool isAllEnemiesCleared = false; 

	inline static float spawnTimer_ = 0.0f;
	inline static float spawnDelay_ = 200.0f;

private:
	KamataEngine::WorldTransform worldTransform_; // ワールドトランスふぉーむ
	KamataEngine::Model* model_ = nullptr;        // モデル
	KamataEngine::Camera* camera_ = nullptr;      // カメラ

	bool alive_ = false; // 現在表示中か
	bool cleared_ = false; // 衝突済みか

	KamataEngine::Vector3 spawnPosition_;
};
