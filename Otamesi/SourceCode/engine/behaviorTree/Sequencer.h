#pragma once
#include <d3dx12.h>
#include <vector>
#include <functional>

/// <summary>
/// シーケンサー
/// </summary>
class Sequencer
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>シーケンサー</returns>
	static Sequencer* Create();

	/// <summary>
	/// シーケンス
	/// </summary>
	bool Sequence();

	/// <summary>
	/// 子ノードを追加する
	/// </summary>
	/// <param name="func()">関数</param>
	void AddNode(std::function<bool()> func);

private:
	//子ノード
	std::vector<std::function<bool()>> nodes;
};