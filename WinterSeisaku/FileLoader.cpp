#include "FileLoader.h"
#include <DxLib.h>
#include<cassert>
#include<algorithm>

FileLoader::Asset::~Asset()
{
	switch (filetype)
	{
	case FileType::graphic:
		DeleteGraph(handle);
		break;
	default:
		assert(false);
		break;
	}
}


int FileLoader::LoadGraph(const std::wstring& path, AssetType type)
{
	// すでにロード済みのアセットか
	if (assets_.contains(path))
	{
		return assets_[path]->handle;
	}
	// まだロード済みでないアセット
	int h = DxLib::LoadGraph(path.c_str());
	assert(h >= 0);
	auto asset = std::make_shared<Asset>();
	asset->filetype = FileType::graphic;
	asset->type = type;
	asset->handle = h;
	assets_[path] = asset;
	return h;
}

void FileLoader::DeleteTemporalAssets()
{
	// 一時的なアセットだけをまとめて削除する
	std::erase_if(assets_, [](const auto& asset) {
		return asset.second->type == AssetType::temporary;
		});
}

