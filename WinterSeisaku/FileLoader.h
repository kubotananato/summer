#pragma once
#include<string>
#include<map>
#include<memory>

enum class AssetType
{
	continuous, // 常駐アセット(アプリ終了時まで解放しない)
	temporary,  // 一時的なもの
};

enum class FileType
{
	graphic // 画像
};

class FileLoader
{
	class Asset
	{
		friend FileLoader;
	private:
		AssetType type = AssetType::continuous;
		FileType filetype = FileType::graphic;
		int handle = -1;
	public:
		Asset(){}
		~Asset();
	};
	std::map < std::wstring, std::shared_ptr<Asset>> assets_; // 読み込み済みのアセットを管理する

public:
	/// <summary>
	/// 画像をロード
	/// </summary>
	/// <param name="path"></param>
	/// <param name="type">常駐か一時的か(一時的ならtemporaryを選択)</param>
	/// <returns></returns>
	int LoadGraph(const std::wstring& path, AssetType type = AssetType::continuous);

	// 一時的アセットのみを削除する
	void DeleteTemporalAssets();
};

