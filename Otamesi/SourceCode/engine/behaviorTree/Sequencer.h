#pragma once
#include <d3dx12.h>
#include <vector>
#include <functional>

/// <summary>
/// �V�[�P���T�[
/// </summary>
class Sequencer
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�V�[�P���T�[</returns>
	static Sequencer* Create();

	/// <summary>
	/// �V�[�P���X
	/// </summary>
	bool Sequence();

	/// <summary>
	/// �q�m�[�h��ǉ�����
	/// </summary>
	/// <param name="func()">�֐�</param>
	void AddNode(std::function<bool()> func);

private:
	//�q�m�[�h
	std::vector<std::function<bool()>> nodes;
};