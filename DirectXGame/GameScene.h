#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "EnemyAttack.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "TitleScene.h"
#include <vector>

class GameScene {

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	bool finished_ = false;

	EnemyAttack enemyAttack_;

public:
	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kClear,
		kFadeOut,
	};

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デストラクタ
	~GameScene();

	// 3Dモデル
	KamataEngine::Model* modelBlock_;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Model* modelDeathParticle_ = nullptr;

	KamataEngine::Model* modelEnemyAttack_ = nullptr;

	KamataEngine::Model* modelBossEnemy_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// 自キャラ
	Player* player_ = nullptr;

	// sky
	Skydome* skydome_ = nullptr;

	// ボス
	KamataEngine::WorldTransform* objBossEnemy_ = nullptr;

	// 敵攻撃
	std::vector<Enemy*> enemies_;



	//果物
	std::vector<EnemyAttack*> enemyAttacks_;

	// パーティクル
	DeathParticles* deathParticles_ = nullptr;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	// ゲームプレイから開始
	Phase phase_;

	bool isFinished() const { return finished_; }

	bool isclear_ = false;

	Fade* fade_ = nullptr;

	std::vector<bool> enemyAttackStartFlags_; // 落下を開始したかどうか

};
