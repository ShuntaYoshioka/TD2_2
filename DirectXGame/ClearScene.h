#pragma once
#include <KamataEngine.h>

#include "Fade.h"
class ClearScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	void Initialize();
	void Update();
	void Draw();
	~ClearScene();

	KamataEngine::Model* model_;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransform_;

	bool isFinished() const { return finished_; }

private:
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
