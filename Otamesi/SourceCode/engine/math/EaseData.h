#pragma once

class EaseData
{
public:
	EaseData(int count = 0);
	~EaseData() = default;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

public:
	/// <summary>
	/// �I��������
	/// </summary>
	/// <returns></returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// �s�����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// �i�s�x�̎擾
	/// </summary>
	/// <returns>0.0 ~ 1.0</returns>
	float GetTimeRate() { return timeRate_; }
	/// <summary>
	/// �t���[���̐ݒ�
	/// </summary>
	/// <param name="count"></param>
	void SetCount(int count) { countNum_ = count; }
	/// <summary>
	/// �I���t���O�̐ݒ�
	/// </summary>
	/// <param name="Flag"></param>
	void SetEndFlag(bool flag) { endFlag_ = flag; }
	/// <summary>
	/// �s���t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetActFlag(bool flag) { actFlag_ = flag; }
private:
	// �C�[�W���O�̐i�s�x�p
	float timer_ = 0.0f;
	float timeRate_ = 0.0f;
	int countNum_ = 0;
	bool endFlag_ = false;
	bool actFlag_ = true;
};

