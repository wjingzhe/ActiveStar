#pragma once

#include "cocos2d.h"
#include <functional>

//#define Jingz_Debug

extern struct AstarCompare;
extern class AstarMapManager; 



class AstarItem :public cocos2d::Ref
{
	friend AstarCompare;
	friend AstarMapManager;

protected:
	AstarItem()
	{

	};

public:
	virtual ~AstarItem()
	{

	};

	bool init(const int &row = -1,const int & col = -1,AstarItem * const &pParentItem = nullptr,float tempG = 0.0,float tempH = 0.0,bool state = false)
	{
		m_iRow = row;
		m_iCol = col;
		
		m_bIsVisited = false;
		m_pParentItem = pParentItem;
		//默认的空头节点耗散值为0
		//没有计算公式，全部置为0
		m_fG = tempG;
		m_fH = tempH;
		m_fF = m_fG+m_fH;
		return true;
	}

	inline void setPosition(const int & row,const int & col)	
	{
		m_iRow = row;
		m_iCol = col;
		
	}

	inline cocos2d::Vec2 getPosition(void)const
	{
		return cocos2d::Vec2(m_iCol,m_iRow);
	}


	inline int getRow(void)const
	{
		return m_iRow;
	}

	inline int getCol(void)const
	{
		return m_iCol;
	}


	inline void setG(const float & g)
	{
		m_fG = g;
	}
	inline float getG(void)const
	{
		return m_fG;
	}


	inline void setH(const float & h)
	{
		m_fH = h;
	}

	inline float getH(void)const
	{
		return m_fH;
	}

	inline void setF(const float &f)
	{
		m_fF = f;
	}

	inline void setF(float &&f)
	{
		m_fF = f;
	}

	//用于排序提高效率
	inline float getF(void)const
	{
		return m_fF;
	}

	inline void setGHF(const float &g,const float &h,const float &f)
	{
		m_fG = g;
		m_fH = h;
		m_fF = f;
	}

	//右值引用调用方式，虽然编译器自己会优化，但是我还是写出来，培养习惯
	inline void setGHF(float &&g,float &&h,float &&f)
	{
		m_fG = g;
		m_fH = h;
		m_fF = f;
	}


	inline AstarItem * getParent(void)const
	{
		return m_pParentItem;
	}

	inline void setParent(AstarItem * const &pParentItem)
	{
		m_pParentItem = pParentItem;
	}

	inline void setVisited(bool)
	{
		m_bIsVisited = true;
	}
	inline bool getVisitedState(void)
	{
		return m_bIsVisited;
	}


	inline void alterPathAndF(AstarItem * const &pParentItem,const float & tempG,const float & tempH,const float & tempF)
	{
		m_pParentItem = pParentItem;
		m_fG = tempG;
		m_fH = tempH;
		m_fF = tempF;
	}

private:
	
	int m_iRow;
	int m_iCol;
	
	bool m_bIsVisited;

	float m_fG;
	float m_fH;
	float m_fF;

	AstarItem * m_pParentItem;//TMXmap中地图数据是id，所以不需要再使用链表指针

};

struct AstarCompare:public std::binary_function<AstarItem*,AstarItem*,bool>
{
public:
	bool operator()(const AstarItem* left,const AstarItem * right)const
	{
		if (left->getF()-right->getF()<1E-5)
		{
			//同耗散，优先考虑预测值
			return left->getH()<right->getH();
		}
		else
		{
			return (left->getF()<right->getF());
		}

		
	}
};


