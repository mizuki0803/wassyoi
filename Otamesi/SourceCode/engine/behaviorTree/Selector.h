#pragma once
#include <d3dx12.h>
#include <vector>
#include <functional>

/// <summary>
/// �Z���N�^�[
/// </summary>
class Selector
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�Z���N�^�[</returns>
	static Selector* Create();

	/// <summary>
	/// �Z���N�g
	/// </summary>
	bool Select();

	/// <summary>
	/// �q�m�[�h��ǉ�����
	/// </summary>
	/// <param name="func()">�֐�</param>
	void AddNode(std::function<bool()> func);

private:
	//�q�m�[�h
	std::vector<std::function<bool()>> nodes;
};