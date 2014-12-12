#include "Astar.h"
USING_NS_CC;
#include <cmath>

#define RecNum 1
Astar::Astar():m_eDimensioFLag(DimensionFlag::D_2D),m_vOpen(nullptr),m_vClosed(nullptr),m_vPath(nullptr),m_pAstarMapManager(nullptr),m_nCount(0),m_bIsInited(false)
{
}

Astar::~Astar()
{
	
	
	reset(true);
	
	deletePtrs();


}



	
AstarItem * Astar::alterPathInfo(int row,int col,AstarItem * pNewParentItem)
{
	
	CCAssert(m_pAstarMapManager!=nullptr,"m_pAstarMapManager should be inited before using!!");

	CCAssert(pNewParentItem!=nullptr,"pNewParentItem should be inited before using!!");
	
	AstarItem * tempItem = m_pAstarMapManager->getItem(row,col,pNewParentItem);
	tempItem->retain();

	//计算新耗散值
	int tempG = Calculate::calculateG(tempItem,pNewParentItem);
	float tempH = Calculate::calculateH(row,col,m_vTarPosi.x,m_vTarPosi.y);
	float tempF = tempG+tempH;
		
	if(checkInOpen(tempItem))
	{
		//已在open表中，则更新值和路径
		if (tempF<tempItem->getF())
		{
			tempItem->alterPathAndF(pNewParentItem,tempG,tempH,tempF);

		}
	}
	else if(checkInClosed(tempItem))//更新值和路径
	{
		//已在closed表中，则检测是否需要跟新路径
		if (tempF<tempItem->getF())
		{   
			//更新值和路径并加入Open表中，但是不修改已拓展标志                                                                                                                                                                   
			tempItem->alterPathAndF(pNewParentItem,tempG,tempH,tempF);

			fromClosedToOpen(tempItem);

		}
		
	}
	else
	{
		//未出现过的的节点
		tempItem->alterPathAndF(pNewParentItem,tempG,tempH,tempF);
		//未出现过的的节点加入open表中
		addItemToList(tempItem,true);
		//执行插入
	}
	tempItem->release();
	return tempItem;
		
		

}

//尾部为栈出口
void Astar::fromOpenToClose(AstarItem *pItem)
{
	if (pItem==nullptr)
	{
		pItem = m_vOpen->back();
	}

	addItemToList(pItem,false);

	removeItemFromList(pItem,true);

}

void Astar::fromClosedToOpen(AstarItem * pItem)
{
	if (pItem!=nullptr)
	{
		pItem = m_vClosed->back();
	}

	addItemToList(pItem,true);
				
	removeItemFromList(pItem,false);
}





//生成搜索路径
void Astar::generatePath(void)
{
	log("jingz----------------%s",__FUNCTION__);
	//todo 假如正确结束和错误结束，处理
	/*m_vCurPosi = m_vTarPosi;

	int row = m_vTarPosi.x;
	int col = m_vTarPosi.y;*/

	int row = m_vCurPosi.x;
	int col = m_vCurPosi.y;

	//无寻路路径，设置为当前位置,并结束寻路
	if (m_vClosed->empty())
	{
		//不知道有什么用途
		/*createEmptyHead(m_vClosed);
		auto temp = m_vClosed.begin();
		(*temp)->setPosition(row,col);*/

		CCAssert(m_pAstarMapManager!=nullptr,"m_pAstarMapManager should be inited before using!!");

		auto temp = m_pAstarMapManager->getItem(row,col,m_pAstarMapManager->getEmptyHeadItem());

		m_vPath->pushBack(temp);

		return ;
	}

	if (m_vPath->empty())
	{
		m_vPath->pushBack(*(m_vClosed->end()-1));
	}
	else//todo 为什么出现这种情况，可能用于跨地图寻路
	{
		m_vPath->insert(0,*(m_vClosed->end()-1));
	}

	//从尾部开始往前遍历
	auto pItem = *m_vPath->begin();
	//默认的空头节点耗散值为0
	while (pItem->getG()>abs(1E-5))
	{

		pItem = pItem->getParent();

		m_vPath->pushBack(pItem);

	}
	m_vPath->reverse();
	

}

void Astar::startSearch(AstarItem * pParentItem)
{
	
	int row = pParentItem->getRow();
	int col = pParentItem->getCol();
	log("jingz----------------%d,%d:%s",row,col,__FUNCTION__);

	//四个方位，或者八个方位，无斜角搜索，北东上为初始探索设定
	//在tiledMap中的地图是顺时针
	
		m_aFlag[0] = false;
		m_aFlag[1] = false;
		m_aFlag[2] = false;
		m_aFlag[3] = false;

		m_apItem[0] = nullptr;
		m_apItem[1] = nullptr;
		m_apItem[2] = nullptr;
		m_apItem[3] = nullptr;

	if (m_pAstarMapManager->isItemIn(row+1,col)&& m_pAstarMapManager->checkReachAble(row+1,col))
	{
		m_aFlag[0] = true;
		m_apItem[0] = alterPathInfo(row+1,col,pParentItem);

	}

	if (m_pAstarMapManager->isItemIn(row,col+1)&& m_pAstarMapManager->checkReachAble(row,col+1))
	{
		m_aFlag[1] = true;
		m_apItem[1]  = alterPathInfo(row,col+1,pParentItem);

	}

	if (m_pAstarMapManager->isItemIn(row-1,col)&& m_pAstarMapManager->checkReachAble(row-1,col))
	{
		m_aFlag[2] = true;
		m_apItem[2] = alterPathInfo(row-1,col,pParentItem);

	}

	if (m_pAstarMapManager->isItemIn(row,col-1)&& m_pAstarMapManager->checkReachAble(row,col-1))
	{
		m_aFlag[3] = true;
		m_apItem[3] = alterPathInfo(row,col-1,pParentItem);

	}
		
	

	//斜方位,暂时把墙角问题放在预测值中
	if (m_aFlag[0])
	{
		if (m_aFlag[1]&&m_pAstarMapManager->isItemIn(row+1,col+1)&& m_pAstarMapManager->checkReachAble(row+1,col+1))
		{
			alterPathInfo(row+1,col+1,pParentItem);
			//m_apItem[0]->setG(m_apItem[0]->getG()-RecNum);
			//m_apItem[1]->setG(m_apItem[1]->getG()-RecNum);
		}
		if (m_aFlag[3]&&m_pAstarMapManager->isItemIn(row+1,col-1)&& m_pAstarMapManager->checkReachAble(row+1,col-1))
		{
			alterPathInfo(row+1,col-1,pParentItem);
			//m_apItem[0]->setG(m_apItem[0]->getG()-RecNum);
			//m_apItem[3]->setG(m_apItem[3]->getG()-RecNum);
			
		}
		
		
	}
	if (m_aFlag[2])
	{
		if (m_aFlag[1]&&m_pAstarMapManager->isItemIn(row-1,col+1)&& m_pAstarMapManager->checkReachAble(row-1,col+1))
		{
			alterPathInfo(row-1,col+1,pParentItem);
			//m_apItem[2]->setG(m_apItem[2]->getG()-RecNum);
			//m_apItem[1]->setG(m_apItem[1]->getG()-RecNum);
		}
		if (m_aFlag[3]&&m_pAstarMapManager->isItemIn(row-1,col-1)&& m_pAstarMapManager->checkReachAble(row-1,col-1))
		{
			alterPathInfo(row-1,col-1,pParentItem);
			//m_apItem[2]->setG(m_apItem[2]->getG()-RecNum);
			//m_apItem[3]->setG(m_apItem[3]->getG()-RecNum);
		}

	}
	
	pParentItem->setVisited(true);
}
	
//排序，不需要完全排序，只需要每次冒泡选择最低耗散值的元素置于尾部
void Astar::sortOne(void)
{
	auto temp = std::min_element(m_vOpen->begin()+1,m_vOpen->end(),AstarCompare());

	std::swap(*temp,*(m_vOpen->end()-1));
}




//如果返回失败路径，则根据H值或F值出现增长轨迹切除掉无效路径。
bool Astar::findPath(Vec2 curPosi,cocos2d::Vec2 targetPosi,cocos2d::TMXTiledMap * pMap,cocos2d::Vector<AstarItem*>* pvPath,bool bAddpath)
{

	CCAssert(pvPath!=nullptr,"Path container should be created before");
	CCAssert(pMap!=nullptr,"Map should be created before");

	pMap->retain();


	////容器的动态增长性问题
	//bool cleanUp = false;
	//++m_nCount;
	//if (m_nCount>=20)
	//{
	//	cleanUp = true;
	//}
	//clear

	if (m_bIsInited)
	{
		reset();
	}
	else
	{
		init();
	}
	
	if (!bAddpath)
	{
		pvPath->clear();
	}

	m_vPath = pvPath;

	//todo 路径圆滑和惯性选择点。	
	m_vCurPosi = curPosi;
	m_vTarPosi = targetPosi;

	m_pAstarMapManager->init(pMap,targetPosi);

	pMap->release();

	

	int curRow = curPosi.x;
	int curCol = curPosi.y;
	int tarRow = targetPosi.x;
	int tarCol = targetPosi.y;

	//全图边界
	long nMaxDepth = abs(m_pAstarMapManager->getMapSize().width*m_pAstarMapManager->getMapSize().height);
	long cuuDepth = 0;

	//创建
	createEmptyHead(m_vOpen);

	//当前节点也是开始位置,parentId=0
	AstarItem * begin = m_pAstarMapManager->getItem(curRow,curCol,m_pAstarMapManager->getEmptyHeadItem());
	addItemToList(begin,true);
	begin->setG(0);
	float tempH = Calculate::calculateH(curRow,curCol,tarRow,tarCol);
	begin->setH(tempH);
	begin->setParent(*m_vOpen->begin());
	begin->setF(tempH);

	int rowDelta;
	int colDelta;

	bool bHavePath = false;
	//auto i = m_vOpen.size();
	while (m_vOpen->size()>1)
	{
		
		++cuuDepth;
		//排序
		sortOne();

		auto parentItem = m_vOpen->back();
		rowDelta = abs(tarRow-parentItem->getRow());
		colDelta = abs(tarCol-parentItem->getCol());

		log("jingz----------------while---%s,%d,%d",__FUNCTION__,parentItem->getRow(),parentItem->getCol());

		fromOpenToClose();

		//找到目标，退出循环
		if (rowDelta <1E-5 && colDelta<1E-5 )
		{
			//目标点也放在路径中
			bHavePath = true;		
			generatePath();
			break;
		}
		//超过最大深度，结束遍历
		else if (cuuDepth>nMaxDepth)
		{
			bHavePath = false;
			generatePath();

			break;
		}
		//已遍历
		else if (parentItem->getVisitedState())
		{
			//已拓展过的节点完成路径更新即可
			//（移入从close表放回open表，再放入close表就是正确的路径）

			continue;
		}
		else
		{
			startSearch(parentItem);
		}

		
	}

	//todo 圆滑处理
	log("jingz----------------end--%s",__FUNCTION__);
	reset();
	cuuDepth = 0;
	return bHavePath;


}


bool Astar::init()
{
	m_vOpen = new Vector<AstarItem*> ();

	m_vClosed = new Vector<AstarItem*> ();

	m_pAstarMapManager = new AstarMapManager();

	m_bIsInited = true;
	return true;
}

