//*********************************************************
//Title		:Element.h
//Author	:Tanabe Yuta
//Date		:2019/01/26
//Copyright	:(C)2019 TanabeYuta
//*********************************************************


#pragma once
#include <array>

#include "Node.h"

class Surface;

class Element
{
public:
	Element();
	~Element();
	Element(Node*, Node*, Node*, Node*);


	bool IsActive;
	std::array<Surface*, 4> psurfaces;		//�l�ʑ̂̕\��
	std::array<Node*, 4> pnodes;			//�l�ʑ̂��\�����钸�_
	Node scenter;							//�O�ڋ��̒��S���W
	double sround;							//�O�ڋ��̔��a
	Node gcenter;							//�l�ʑ̂̏d�S���W
	double volume;							//�l�ʑ̂̑̐�
	double aspect;							//�l�ʑ̂̃A�X�y�N�g��


	Element* GetLocateId(Node*);			//�v�f���ɓ_������Ύ��g���w���|�C���^��Ԃ��@�����łȂ���Ηאڗv�f���w���|�C���^��Ԃ�
	bool IsInSphere(Node*);					//true:�_���O�ڋ���
	Surface* GetAdjacentSurface(Element*);	//�n���ꂽ�v�f�Ɨאڂ���ʂ��w���|�C���^��Ԃ�
};

