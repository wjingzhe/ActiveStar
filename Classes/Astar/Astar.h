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
	
	//���򣬲���Ҫ��ȫ����ֻ��Ҫÿ��ð��ѡ����ͺ�ɢֵ��Ԫ������β��
	void sortOne(void);


	bool findPath(cocos2d::Vec2 curPosi,cocos2d::Vec2 targetPosi,cocos2d::TMXTiledMap * pMap,cocos2d::Vector<AstarItem*>* pvPath,bool bAddpath = false);


protected:

	bool init();

	//���ɳ����������·��
	void generatePath(void);

	//
	void fromOpenToClose(AstarItem * pItem=nullptr);

	void fromClosedToOpen(AstarItem * pItem=nullptr);

	inline bool checkInClosed(AstarItem *pItem)const
	{
		//�ѱ����ʹ��Ľڵ㲻���ٱ����ʣ���Ϊ���˲���������H��ʽ
		if (pItem->getVisitedState())
		{
			//todo ��ʱ��ȥ���°�װset,ʱ�䲻��
			return m_closedId.find(pItem->getRow()*100+pItem->getCol())!=m_closedId.end();
			
		}
		return false;
	}

	inline bool checkInOpen(AstarItem * pItem)const
	{
		////�ѱ����ʹ��Ľڵ㲻���ٱ����ʣ���Ϊ���˲���������H��ʽ
		//if (pItem->getVisitedState())
		//{
		//	return false;
		//}

		//todo ��ʱ��ȥ���°�װset,ʱ�䲻��
		
			return m_openId.find(Calculate::calculateID(pItem))!=m_openId.end();
		
	}

	inline void createEmptyHead(cocos2d::Vector<AstarItem*> * container)
	{
		AstarItem * emptyHead = m_pAstarMapManager->getEmptyHeadItem();
		//todo Ӧ��������
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

	//true ΪOpen�����ӽڵ㣬falseΪclosed������
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
			//��ջ�ķ�ʽʹ������
			m_vOpen->erase(m_vOpen->size()-1);
				//popBack();
		}
		else
		{
			//CCAssert(pItem==m_vClosed.back(),"Only the back item can  be removed");
			m_closedId.erase(tiledId);
			//��ջ�ķ�ʽʹ������
			m_vClosed->erase(m_vClosed->size()-1);
		}
	}


	void searchColPath(int row,int col,AstarItem *pParentItem)
	{
			//������б�Ǵ�Խ�����
		if ( m_pAstarMapManager->isItemIn(row,col)&& m_pAstarMapManager->checkReachAble(row,col) )
		{
			//ǽ������
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
			//������б�Ǵ�Խ�����
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
	//todo ��ʱ��ȥ���°�װset,ʱ�䲻��
	TiledIdContainer m_openId;
	TiledIdContainer m_closedId;
	cocos2d::Vector<AstarItem*> * m_vOpen;
	cocos2d::Vector<AstarItem*> * m_vClosed;
	//todo��Ϊ���·����ҪԲ�������������ֱ���޸ĵĿ�����,ʹ��List�ȽϺ�
	cocos2d::Vector<AstarItem*>* m_vPath;

	cocos2d::Vec2 m_vCurPosi;
	cocos2d::Vec2 m_vTarPosi;

	int m_nCount;

	DimensionFlag m_eDimensioFLag;
	

	AstarMapManager * m_pAstarMapManager;

	bool m_bIsInited;


	bool m_aFlag[4];
	AstarItem * m_apItem[4];
};

