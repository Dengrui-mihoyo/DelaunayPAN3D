//*********************************************************
//Title		:Surface.h
//Author	:Tanabe Yuta
//Date		:2019/01/26
//Copyright	:(C)2019 TanabeYuta
//*********************************************************


#pragma once
#include <array>

#include "Node.h"

class Element;

class Surface
{
public:
	Surface();
	~Surface();
	Surface(Node*, Node*, Node*, Element*, Element*);


	Element* pneighbor;					//�אڗv�f���w���|�C���^
	bool IsActive;						//true:���ʑ̂̋��E��
	std::array<Node*, 3> pnodes;		//�ߓ_
	Element* pparent;					//���̖ʂ����v�f���w���|�C���^
	
	
	bool operator==(const Surface &);	//�ڂ��Ă���ʂ�����


	bool IsRayCross(Node, Node);		//true:�x�N�g�����ʂƌ���
};

