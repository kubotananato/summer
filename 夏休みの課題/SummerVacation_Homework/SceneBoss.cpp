#include "SceneBoss.h"
#include "DxLib.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "BGM.h"

SceneBoss::SceneBoss() :
	m_pPlayer(nullptr),
	m_isGoToBossBattle(false),
	m_isTalked(false),
	m_bgmHandle(-1),
	m_animTimer(0)
{
	for (int i = 0; i < kBossAnimFrames; i++)
	{
		m_bossImgHandle[i] = -1;
	}
}

SceneBoss::~SceneBoss()
{
}

void SceneBoss::Init(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_isGoToBossBattle = false;
	m_isTalked = false;
	m_animTimer = 0;

	BgmManager::Play("data/BGM/BossScene.mp3", 100);

	// マップの初期化
	m_map.Init("data/Bossmap.csv");

	// プレイヤーの位置調整
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetPos(Vec2(Game::kScreenWidth / 2.0f - 16.0f, Game::kScreenHeight - 120.0f));
	}

	// ボス画像の読み込み
	if (m_bossImgHandle[0] == -1)
	{
		LoadDivGraph("data/Enemy/BIdle.png", 4, 4, 1, 100, 100, m_bossImgHandle);
	}

	m_bossPos = Vec2(Game::kScreenWidth / 2.0f, 150.0f);
}

void SceneBoss::End()
{
	BgmManager::Stop();
	m_map.End();

	// 分割画像の削除
	for (int i = 0; i < kBossAnimFrames; i++)
	{
		if (m_bossImgHandle[i] != -1)
		{
			DeleteGraph(m_bossImgHandle[i]);
			m_bossImgHandle[i] = -1;
		}
	}

	if (m_bgmHandle != -1)
	{
		StopSoundMem(m_bgmHandle);
		DeleteSoundMem(m_bgmHandle);
		m_bgmHandle = -1;
	}
}

void SceneBoss::Update()
{
	if (m_pPlayer == nullptr) return;

	m_animTimer++;

	// 会話中ではない時だけプレイヤーを移動させる
	if (!m_isTalked)
	{
		m_pPlayer->Update(m_map);

		// ボスとの距離判定
		Vec2 diff = m_pPlayer->GetColCenter() - m_bossPos;
		if (diff.SqLength() < (CHIP_SIZE * 2) * (CHIP_SIZE * 2))
		{
			m_isTalked = true;
		}
	}
	else
	{
		// 会話中に SPACE か ボタンを押すとボス戦へ
		if (CheckHitKey(KEY_INPUT_SPACE) || (GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_1))
		{
			m_isGoToBossBattle = true;
		}
	}
}

void SceneBoss::Draw()
{
	m_map.Draw();

	// 玉座の前にボスを描画
	if (m_bossImgHandle[0] != -1)
	{
		int currentFrame = (m_animTimer / 10) % kBossAnimFrames;
		DrawRotaGraph(static_cast<int>(m_bossPos.x), static_cast<int>(m_bossPos.y), 2.5, 0.0, m_bossImgHandle[currentFrame], TRUE);
	}

	m_pPlayer->Draw();

	// 会話ウィンドウ表示
	if (m_isTalked)
	{
		DrawBox(50, 360, Game::kScreenWidth - 50, 460, GetColor(0, 0, 0), TRUE);
		DrawBox(50, 360, Game::kScreenWidth - 50, 460, GetColor(255, 255, 255), FALSE);

		DrawString(80, 380, "魔王「よくぞ ここまで たどりついたな…」", GetColor(255, 255, 255));
		DrawString(80, 410, "[ SPACE / BUTTON ] 戦闘開始", GetColor(255, 255, 0));
	}
}