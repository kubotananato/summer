#pragma once
#include <memory>
#include "Geometry.h"

class FileLoader;

class Application
{
private:
	std::shared_ptr<FileLoader> loader_;
	bool requestExit_ = false;
	Size wsize_;
	Application();
	Application(const Application&) = delete; // 複製を防ぐ

public:
	static Application& GetInstance();
	~Application(){}
	// 初期化
	bool Init();
	// ゲームのメインループを走らせる
	void Run();
	// ゲーム終了処理
	void Terminate();

	/// <summary>
	/// ウィンドウサイズの取得
	/// </summary>
	/// <returns></returns>
	const Size& GetWindowSize() const;

	// アプリケーションの終了をリクエストする
	void RequestExit();

	std::shared_ptr<FileLoader> GetLoader();
};

