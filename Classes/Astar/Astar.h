#pragma once
#include "cocos2d.h"
#include "AstarItem.h"
#include <set>
#include "AstarMapManager.h"

enum class DimensionFlag
{
	D_2D = 0,
	D_2_5D,
	D_3D,
};

enum MoveDirection
{
	MD_NONE = 0,
	MD_N,
	MD_E,
	MD_S,
	MD_W,
	MD_UP,
	MD_DOWN,
	MD_END
};





class Astar
{
	
public:
	typedef std::set<int> TiledIdContainer;
	
	Astar();
	~Astar();

	

	inline cocos2d::Vec2 getCurrentPosi(void)const
	{
		return m_vCurPosi;
	}
	
	inline cocos2d::Vec2 getTargetPosi(void)const
	{
		return m_vTarPosi;
	}
	
	

	AstarItem * alterPathInfo(int row,int col,AstarItem * pParentItem);
	

	

	void startSearch(AstarItem * pParentItem);
	
	//排序，不需要完全排序，只需要每次冒泡选择最低耗散值的元素置于尾部
	void sortOne(void);


	bool findPath(cocos2d::Vec2 curPosi,cocos2d::Vec2 targetPosi,cocos2d::TMXTiledMap * pMap,cocos2d::Vector<AstarItem*>* pvPath,bool bAddpath = false);


protected:

	bool init();

	//生成初步搜索结果路径
	void generatePath(void);

	//
	void fromOpenToClose(AstarItem * pItem=nullptr);

	void fromClosedToOpen(AstarItem * pItem=nullptr);

	inline bool checkInClosed(AstarItem *pItem)const
	{
		//已被访问过的节点不会再被访问，因为本人采用收敛的H公式
		if (pItem->getVisitedState())
		{
			//todo 暂时不去重新包装set,时间不够
			return m_closedId.find(pItem->getRow()*100+pItem->getCol())!=m_closedId.end();
			
		}
		return false;
	}

	inline bool checkInOpen(AstarItem * pItem)const
	{
		////已被访问过的节点不会再被访问，因为本人采用收敛的H公式
		//if (pItem->getVisitedState())
		//{
		//	return false;
		//}

		//todo 暂时不去重新包装set,时间不够
		
			return m_openId.find(Calculate::calculateID(pItem))!=m_openId.end();
		
	}

	inline void createEmptyHead(cocos2d::Vector<AstarItem*> * container)
	{
		AstarItem * emptyHead = m_pAstarMapManager->getEmptyHeadItem();
		//todo 应该已清零
		container->pushBack(emptyHead);
	}
	


	inline void reset(bool cleanUp = false)
	{
		m_openId.clear();
		m_closedId.clear();

		if (m_vOpen!=nullptr)
		{
			m_vOpen->clear();
			m_vOpen->reserve(m_vOpen->capacity()/2);
		}
		if (m_vClosed!=nullptr)
		{
			m_vClosed->clear();
			m_vClosed->reserve(m_vClosed->capacity()/2);
		}

		if (m_pAstarMapManager!=nullptr)
		{
			m_pAstarMapManager->reset();
		}


	}

	//true 为Open表增加节点，false为closed表增加
	inline void addItemToList(AstarItem* pItem,bool flag = true)
	{
		//todo
		int tiledId = Calculate::calculateID(pItem);
		if(flag)
		{
			m_vOpen->pushBack(pItem);
			m_openId.insert(tiledId);
		}
		else
		{
			m_closedId.insert(tiledId);
			m_vClosed->pushBack(pItem);
		}
		
		
	}
	inline void removeItemFromList(AstarItem* pItem,bool flag = true)
	{
		
		int tiledId = Calculate::calculateID(pItem);
		if(flag)
		{
			CCAssert(pItem==m_vOpen->back(),"Only the back item can be removed");
			m_openId.erase(tiledId);
			//已栈的方式使用容器
			m_vOpen->erase(m_vOpen->size()-1);
				//popBack();
		}
		else
		{
			//CCAssert(pItem==m_vClosed.back(),"Only the back item can  be removed");
			m_closedId.erase(tiledId);
			//已栈的方式使用容器
			m_vClosed->erase(m_vClosed->size()-1);
		}
	}


	void searchColPath(int row,int col,AstarItem *pParentItem)
	{
			//不存在斜角穿越的情况
		if ( m_pAstarMapManager->isItemIn(row,col)&& m_pAstarMapManager->checkReachAble(row,col) )
		{
			//墙角问题
			//pParentItem->setG(pParentItem->getG()-1);
			auto pNewParentItem = alterPathInfo(row,col,pParentItem);
			if ( m_pAstarMapManager->isItemIn(row,col+1)&& m_pAstarMapManager->checkReachAble(row,col+1) )
			{
				alterPathInfo(row,col+1,pParentItem);
				pParentItem->setG(pParentItem->getG()-1);
			}
			if ( m_pAstarMapManager->isItemIn(row,col-1)&& m_pAstarMapManager->checkReachAble(row,col-1) )
			{
				alterPathInfo(row,col-1,pParentItem);
				pParentItem->setG(pParentItem->getG()-1);
			}
		}
	}

	void searchRowPath(int row,int col,AstarItem *pParentItem)
	{
			//不存在斜角穿越的情况
		if ( m_pAstarMapManager->isItemIn(row,col)&& m_pAstarMapManager->checkReachAble(row,col) )
		{
			//pParentItem->setG(pParentItem->getG()-1);
			auto pNewParentItem = alterPathInfo(row,col,pParentItem);
			if ( m_pAstarMapManager->isItemIn(row+1,col)&& m_pAstarMapManager->checkReachAble(row+1,col) )
			{
				alterPathInfo(row+1,col,pNewParentItem);
			}
			if ( m_pAstarMapManager->isItemIn(row-1,col)&& m_pAstarMapManager->checkReachAble(row-1,col) )
			{
				alterPathInfo(row-1,col,pNewParentItem);
			}
		}
	}

	void deletePtrs(void)
	{
		if (m_pAstarMapManager!=nullptr)
		{
			delete m_pAstarMapManager;
			m_pAstarMapManager = nullptr;
		}

		if (m_pAstarMapManager!=nullptr)
		{
			delete m_vOpen;
			m_pAstarMapManager = nullptr;
		}

		if (m_pAstarMapManager!=nullptr)
		{
			delete m_vClosed;
			m_pAstarMapManager = nullptr;
		}
	}
	

private:
	//todo 暂时不去重新包装set,时间不够
	TiledIdContainer m_openId;
	TiledIdContainer m_closedId;
	cocos2d::Vector<AstarItem*> * m_vOpen;
	cocos2d::Vector<AstarItem*> * m_vClosed;
	//todo因为结果路线需要圆滑修正，外界有直接修改的可能性,使用List比较好
	cocos2d::Vector<AstarItem*>* m_vPath;

	cocos2d::Vec2 m_vCurPosi;
	cocos2d::Vec2 m_vTarPosi;

	int m_nCount;

	DimensionFlag m_eDimensioFLag;
	

	AstarMapManager * m_pAstarMapManager;

	bool m_bIsInited;

	bool m_bHavePath;

	bool m_aFlag[4];
	AstarItem * m_apItem[4];
};


