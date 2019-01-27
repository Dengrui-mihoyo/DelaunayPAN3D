//*********************************************************
//Title		:Delaunay.h
//Author	:Tanabe Yuta
//Date		:2019/01/08
//Copyright	:(C)2019 TanabeYuta
//*********************************************************


#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <vector>

#include "Parameter.h"
#include "Node.h"
#include "Surface.h"
#include "Element.h"


namespace Delaunay3D {
	//**********���z�l�ʑ̂̐���**********
	void MakeSupertetrahedran(std::vector<Node*> &_nlist, std::vector<Element*> &_elist) {
		std::cout << "Make supertetraedron\n";

		//----------�ߓ_�̂����ŉ����_��T��----------
		double rmax = 0.0;
		for (auto pnode : _nlist) {
			if (pnode->Size() > rmax) {
				rmax = pnode->Size();
			}
		}
		rmax *= 1.5 * 3.0;

		//----------���z�l�ʑ̂̐���----------
		Node* nst0 = new Node(rmax * 2.0*sqrt(2.0) / 3.0 *cos(2.0*0.0*M_PI / 3.0), rmax * 2.0*sqrt(2.0) / 3.0 *sin(2.0*0.0*M_PI / 3.0), -rmax / 3.0, -1);
		Node* nst1 = new Node(rmax * 2.0*sqrt(2.0) / 3.0 *cos(2.0*1.0*M_PI / 3.0), rmax * 2.0*sqrt(2.0) / 3.0 *sin(2.0*1.0*M_PI / 3.0), -rmax / 3.0, -1);
		Node* nst2 = new Node(rmax * 2.0*sqrt(2.0) / 3.0 *cos(2.0*2.0*M_PI / 3.0), rmax * 2.0*sqrt(2.0) / 3.0 *sin(2.0*2.0*M_PI / 3.0), -rmax / 3.0, -1);
		Node* nst3 = new Node(0.0, 0.0, rmax, -1);

		_nlist.push_back(nst0);		_nlist.push_back(nst1);		_nlist.push_back(nst2);		_nlist.push_back(nst3);
		_elist.push_back(new Element(nst0, nst1, nst2, nst3));
	}


	//**********�Ǐ�Delaunay����**********
	//�Ǐ��I��Delaunay���������㖖���̗v�f��id��Ԃ�
	Element* MeshLocal(Node *_node, Element *_pethis, std::vector<Element*> &_elist) {
		std::vector<Element*> stack, substack;
		std::vector<Surface*> sstack;

		//----------�ǉ������_���O�ڋ����Ɏ��v�f���W�߂�----------
		substack.push_back(_pethis);
		while (substack.size()) {
			Element* pend = *(substack.end() - 1);				//�T�u�X�^�b�N�����̗v�f���w���|�C���^
			substack.pop_back();

			if (pend->IsActive) {
				stack.push_back(pend);
				pend->IsActive = false;

				for (auto& psurface : pend->psurfaces) {
					Element* pneighbor = psurface->pneighbor;
					if (pneighbor != nullptr && pneighbor->IsInSphere(_node)) {
						substack.push_back(pneighbor);
					}
					else {
						sstack.push_back(psurface);
					}
				}
			}
		}

		//----------���ʑ̂̉����𖄂߂�----------
		bool is_anysurface_invalid = true;
		while (is_anysurface_invalid) {
			is_anysurface_invalid = false;

			for (int i = 0; i < sstack.size(); i++) {
				if (sstack[i]->IsActive) {
					Node vn = (*sstack[i]->pnodes[1] - *sstack[i]->pnodes[0]) * (*sstack[i]->pnodes[2] - *sstack[i]->pnodes[0]);
					Node vp = *_node - *sstack[i]->pnodes[0];
					double D = vn ^ vp;

					//----------�s�ǂȖʂ�����ꍇ----------
					if (D < EPS) {
						Element* peadd = sstack[i]->pneighbor;			//�s�ǂȖʂɗאڂ���v�f���w���|�C���^

						//----------�V�����ǉ��ł���v�f������ꍇ----------
						if (peadd != nullptr) {
							if (peadd->IsActive) {
								is_anysurface_invalid = true;
								stack.push_back(peadd);
								peadd->IsActive = false;

								//----------�ǉ������v�f�̊e�ʂɂ��ėאږʂ�False��----------
								for (auto& psurface : peadd->psurfaces) {
									Element* pneighbor = psurface->pneighbor;
									if (pneighbor != nullptr && !pneighbor->IsActive) {
										pneighbor->GetAdjacentSurface(peadd)->IsActive = false;
									}
									else {
										sstack.push_back(psurface);
									}
								}
								break;
							}
						}

						//----------�V�����ǉ��ł���v�f�������ꍇ----------
						else if (fabs(D) < EPS) {
							sstack[i]->IsActive = false;
						}
					}
				}
			}
		}

		//----------�V�����v�f�𐶐�----------
		std::vector<Element*> penew;				//�V�������������v�f���w���|�C���^�̃X�^�b�N
		for (auto& psurface : sstack) {
			if (psurface->IsActive) {
				Element* tmp = new Element(psurface->pnodes[0], psurface->pnodes[1], psurface->pnodes[2], _node);
				tmp->psurfaces[3]->pneighbor = psurface->pneighbor;
				if (psurface->pneighbor != nullptr) {
					psurface->pneighbor->GetAdjacentSurface(psurface->pparent)->pneighbor = tmp;
				}
				penew.push_back(tmp);
			}
		}

		//----------�V�����������ꂽ�v�f���m�̗אڊ֌W���v�Z----------
		for (auto pelement : penew) {
			for (auto psurface : pelement->psurfaces) {
				if (psurface->pneighbor == nullptr) {
					for (auto pelement2 : penew) {
						if (pelement != pelement2) {
							for (auto psurface2 : pelement2->psurfaces) {
								if (*psurface == *psurface2) {
									psurface->pneighbor = pelement2;
									psurface2->pneighbor = pelement;
									break;
								}
							}
						}
					}
				}
			}
		}
		
		//----------�V�����������ꂽ�v�f��elist�ɒǉ�----------
		for (auto pelement : penew) {
			_elist.push_back(pelement);
		}

		//----------stack���̌Â��v�f���폜----------
		for (auto& pelement : _elist) {
			if (!pelement->IsActive) {
				auto tmp = pelement;
				pelement = *(_elist.end() - 1);
				_elist.pop_back();
				delete tmp;
			}
		}
		
		return *(_elist.end() - 1);
	}


	//**********�e��Delaunay����**********
	void MakeRoughMesh(std::vector<Node*> _nlist, std::vector<Element*> &_elist) {
		std::cout << "Make rough mash\n";

		Element* pethis = _elist[0];										//���ݒ��ׂĂ���v�f���w���|�C���^
		for (auto& pnode : _nlist) {
			if (pnode->type != -1) {
				while (1) {
					int count = 0;
					Element* penext = pethis->GetLocateId(pnode);				//���ɒ��ׂ�v�f���w���|�C���^
					//----------�v�f���ɓ_������Ƃ�----------
					if (penext == pethis) {
						std::cout << "at\t" << pethis << "\n";
						pethis = MeshLocal(pnode, pethis, _elist);
						break;
					}
					//----------�Ȃ��Ƃ�----------
					else {
						pethis = penext;
					}
				}
			}
		}
	}


	//**********���z�l�ʑ̂̍폜**********
	void DeleteSupertetrahedran(std::vector<Element*> &_elist) {
		for (auto& pelement : _elist) {
			if (pelement->pnodes[0]->type == -1 || pelement->pnodes[1]->type == -1 || pelement->pnodes[2]->type == -1 || pelement->pnodes[3]->type == -1) {
				for (auto& psurface : pelement->psurfaces) {
					if (psurface->pneighbor != nullptr) {
						psurface->pneighbor->GetAdjacentSurface(psurface->pparent)->pneighbor = nullptr;
					}
				}
				auto tmp = pelement;
				pelement = *(_elist.end() - 1);
				_elist.pop_back();
				delete tmp;
			}
		}
	}
}