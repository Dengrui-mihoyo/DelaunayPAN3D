//*********************************************************
//Title		:Node.h
//Author	:Tanabe Yuta
//Date		:2019/01/26
//Copyright	:(C)2019 TanabeYuta
//*********************************************************

#pragma once
class Node
{
public:
	Node();
	~Node();
	Node(double, double, double, int);


	double x, y, z;						//���W
	int type;							//�ߓ_�̎��


	Node operator+(const Node &);		//�x�N�g���̘a���v�Z
	Node operator-(const Node &);		//�x�N�g���̍����v�Z
	Node operator*(const Node &);		//�x�N�g���̊O�ς��v�Z
	double operator^(const Node &);		//�x�N�g���̓��ς��v�Z
	Node operator*(double);				//�x�N�g���̃X�J���[�{���v�Z
	Node operator/(double);				//�x�N�g���̃X�J���\�����v�Z


	double Size();						//�x�N�g���̑傫�����v�Z
};

