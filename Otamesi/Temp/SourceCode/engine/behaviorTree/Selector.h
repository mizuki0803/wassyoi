#pragma once
#include <d3dx12.h>
#include <vector>
#include <functional>

/// <summary>
/// セレクター
/// </summary>
class Selector
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>セレクター</returns>
	static Selector* Create();

	/// <summary>
	/// セレクト
	/// </summary>
	bool Select();

	/// <summary>
	/// 子ノードを追加する
	/// </summary>
	/// <param name="func()">関数</param>
	void AddNode(std::function<bool()> func);

private:
	//子ノード
	std::vector<std::function<bool()>> nodes;
};