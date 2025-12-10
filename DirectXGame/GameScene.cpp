#include "GameScene.h"
#include "CameraController.h"
#include "MyMath.h"

#include <cstdlib>
#include <ctime>

using namespace KamataEngine;

void GameScene::Initialize() {

	phase_ = Phase::kFadeIn;
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources./uvChecker.png");

	// 3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	modelEnemyAttack_ = Model::CreateFromOBJ("fruit", true);
	modelBossEnemy_ = Model::CreateFromOBJ("Tree", true);
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	// マップチップフィールドの初期化
	mapChipField_->LoadMapchipCsv("Resources/blocks.csv");

	// 自キャラ生成
	player_ = new Player();

	// 自キャラ座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(18, 10);

	//ボスの描画
	objBossEnemy_ = new WorldTransform();
	objBossEnemy_->Initialize();

	KamataEngine::Vector3 basePos = playerPosition;
	objBossEnemy_->translation_ = {
	    basePos.x, 
		basePos.y - 6.0f,
	    basePos.z + 30.0f
	};

	// 弱点をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> enemyPositions = {
	    {6,  5 }, // 1つ目
	    {29, 5 },
	    {6,  12},
	    {29, 12},
	};

	// 敵の初期化
	for (const auto& tilePos : enemyPositions) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(static_cast<uint32_t>(tilePos.x), static_cast<uint32_t>(tilePos.y));
		Vector3 enemySize = {1.0f, 1.0f, 1.0f};
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	skydome_ = new Skydome();

	player_->SetMapChipField(mapChipField_);

	// カメラの初期化
	camera_.Initialize();

	cameraController_ = new CameraController();

	cameraController_->Initialize();
	cameraController_->SetTarget(player_);

	CameraController::Rect cameraAera = {10.0f, 24.0f, 0.0f, 32.0f};
	cameraController_->SetMovableArea(cameraAera);

	// 仮生成パーティクル
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticle_, &camera_, playerPosition);

	// 敵の攻撃をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> enemyAttackPositions = {
	    {3,  -1}, // 1つ目
	    {5,  -1},
        {7,  -1},
        {9,  -1},
        {11, -1},
        {13, -1},
        {15, -1},
        {17, -1},
        {19, -1},
        {21, -1},
        {23, -1},
        {25, -1},
        {27, -1},
        {29, -1},
        {31, -1},
	};

	// 果物の初期化
	for (const auto& tilePos : enemyAttackPositions) {
		EnemyAttack* newenemyAttack = new EnemyAttack();
		Vector3 enemyAttackPosition = mapChipField_->GetMapChipPositionByIndex(static_cast<uint32_t>(tilePos.x), static_cast<uint32_t>(tilePos.y));
		Vector3 enemyAttackSize = {1.0f, 1.0f, 1.0f};
		newenemyAttack->Initialize(enemyAttackPosition, enemyAttackSize, modelEnemyAttack_);
		enemyAttacks_.push_back(newenemyAttack);
	}

	enemyAttackStartFlags_.resize(enemyAttacks_.size(), false);

	// 他の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	GenerateBlocks();

	cameraController_->Reset();

	// カメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定
	worldTransformBlocks_.resize(20);
	for (uint32_t i = 0; i < 20; ++i) {
		// 1列の要素数を設定
		worldTransformBlocks_[i].resize(100);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {

		aabb2 = enemy->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);

			 for (Enemy* e2 : enemies_) {
				if (e2 != enemy) {
					e2->disappear();
				}
			}
			 break;
		}
	}

	for (EnemyAttack* enemyAttack : enemyAttacks_) {

		aabb2 = enemyAttack->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemyAttack);

			enemyAttack->OnCollision(player_);
		}
	}

#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズ処理
		if (player_->isDead() == true) {
			phase_ = Phase::kDeath;

			//const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			//deathParticles_ = new DeathParticles;
			//deathParticles_->Initialize(modelDeathParticle_, &camera_, deathParticlesPosition);
		}

		if (Enemy::isAllEnemiesCleared) {
			phase_ = Phase::kClear;
		}
		break;
	case Phase::kDeath:

		if (deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;

	case Phase::kClear:
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);

	case Phase::kFadeIn:
		if (fade_->isFinished()) {
			phase_ = Phase::kPlay;
		}
		break;

	case Phase::kFadeOut:
		if (fade_->isFinished()) {
			finished_ = true;
		}
		break;
	};
}

void GameScene::Update() {

	fade_->Update();

	switch (phase_) {
	case Phase::kPlay:

		CheckAllCollisions();

		break;
	case Phase::kDeath:
		//deathParticles_->Update();
		break;
	case Phase::kFadeIn:

		fade_->Update();

		break;
	case Phase::kFadeOut:

		fade_->Update();
		CheckAllCollisions();
		if (phase_ == Phase::kFadeOut) {
			deathParticles_->Update();
		}
		break;
	}


	// 共通の処理
	if (phase_ != Phase::kFadeOut) {
		player_->Update();
	}

	// Skyblock
	skydome_->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	for (EnemyAttack* enemyAttack : enemyAttacks_) {
		enemyAttack->Update();
		enemyAttack->Move();
	}

	// ===== ボスのワールド行列更新 =====
	if (objBossEnemy_) {
		objBossEnemy_->matWorld_ = MakeAffineMatrix(objBossEnemy_->scale_, objBossEnemy_->rotation_, objBossEnemy_->translation_);
		objBossEnemy_->TransferMatrix();
	}

	// カメラコントロール
	if (!player_->isHit()) {

			cameraController_->Update();
	} else {
		if (player_->GetHitTimer() > 0.3f) {

			cameraController_->Update();
		}
	}
	

	// デバッグカメラの更新
	debugCamera_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	ChangePhase();

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		camera_.TransferMatrix();
		// camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	if (skydome_) {
		skydome_->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 自キャラの描画

	if (!player_->isDead()) {
		if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn)
			player_->Draw();
	}

	if (objBossEnemy_ && modelBossEnemy_) {
		modelBossEnemy_->Draw(*objBossEnemy_, camera_);
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	for (EnemyAttack* enemyAttack : enemyAttacks_) {
		enemyAttack->Draw(&camera_);
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	fade_->Draw();
}

GameScene::~GameScene() {
	delete modelBlock_;
	delete debugCamera_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete deathParticles_;
	delete fade_;

	delete objBossEnemy_;
	delete modelBossEnemy_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (EnemyAttack* enemyAttack : enemyAttacks_) {
		delete enemyAttack;
	}
	delete modelSkydome_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}
