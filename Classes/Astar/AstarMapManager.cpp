/****************************************************************************
 Copyright (c) 2014-2015 Jingz @ Recca2 Studio.
 
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "AstarMapManager.h"
USING_NS_CC;


#define Half_TiledHeight 16
#define Half_TiledWidth 32

AstarMapManager::AstarMapManager():m_pMap(nullptr),m_pLayer(nullptr),m_mItemPools(nullptr),m_EmptyHeadItem(nullptr)
{
	
	
}



AstarMapManager::~AstarMapManager()
{
	reset(true);
	deletePtrs();
}

void AstarMapManager::init(TMXTiledMap * pMap,Vec2 tarPosi,bool cleanup)
{

	CCAssert(pMap!=nullptr,"pMap is nullptr");
	if (m_EmptyHeadItem==nullptr)
	{
		m_EmptyHeadItem = new AstarItem();
		//不使用autorelease，默认为1
	}
	m_EmptyHeadItem->init();

	pMap->retain(); 

	if (cleanup||pMap!=m_pMap)
	{
		reset(true);
		m_pMap = pMap;
		m_pMap->retain();

		if (m_pLayer!=nullptr)
		{
			m_pLayer->release();
		}
		m_pLayer = m_pMap->getLayer("grass");
		m_pLayer->retain();
	}


	pMap->release();
	
	if (m_pLayer==nullptr)
	{
		m_pLayer = m_pMap->getLayer("grass");
		m_pLayer->retain();
	}

	m_vTarPosi = tarPosi;
	//todo 地图掩码层，暂时使用grass
	

	if (m_mItemPools==nullptr)
	{
		m_mItemPools = new std::map<Row,ColContainer*>();
	}

}


void AstarMapManager::reset(bool cleanup)
{

	if (m_pLayer!=nullptr)
	{
		m_pLayer->release();
		m_pLayer=nullptr;
	}
	
	if (m_pMap!=nullptr)
	{
		m_pMap->release();
		m_pMap=nullptr;
	}
	if (cleanup)
	{
		clearPools();
	}
	
}


//不存在则插入该item
AstarItem * AstarMapManager::getItem(int row,int col,AstarItem * pParentItem)
{
	auto it = m_mItemPools->find(row);
	if (it!=m_mItemPools->end())
	{
		auto itContainer = it->second;
		auto itItem = itContainer->find(col);
		if (itItem!=itContainer->end())
		{
			return itItem->second;
		}
		else
		{
			return CreateNewItem(itContainer,row,col,pParentItem);
		}

	}

	else
	{
		//插入rowContainer，并创建新的item
		//m_mItemPools
		ColContainer * p = new ColContainer();
		auto pItem = CreateNewItem(p,row,col,pParentItem);
		m_mItemPools->insert(std::make_pair(row,p));
		return pItem;
	}

	return nullptr;
}


bool AstarMapManager::isItemIn(int row,int col)const
{
	auto t_size = m_pMap->getMapSize();
	return (0 <= row && row< t_size.width) && (0<=col && col <t_size.height);
}

//理论上应该不需要这个接口，当不存在是出现内存错误了
bool AstarMapManager::isItemIn(AstarItem *pItem)const
{
	auto t_size = m_pMap->getMapSize();
	int row = pItem->getRow();
	int col = pItem->getCol();
	return (0 <= row && row< t_size.width) && (0<=col && col <t_size.height);
}

	
//todo 用途待定
void AstarMapManager::removeItem(AstarItem * p)
{

}

//todo 用途待定
void AstarMapManager::removeItem(int row,int col)
{

}

//todo
bool AstarMapManager::checkReachAble(int row,int col)const
{
	unsigned int tileGid = getTileGid(row,col);
	Value tiledic = m_pMap->getPropertiesForGID(tileGid);

	//todo 产生断点，原因未明
	


	if(tiledic.isNull())
	{
		return false;
	}

	int mv = tiledic.asValueMap().at("conflict").asInt();
	//todo 地图掩码尚未分级
	if (mv==1)
	{
		return false;
	}
	return true;
}


bool AstarMapManager::checkReachAble(TMXTiledMap * pMap,const std::string& layerName,Vec2 posi)
{
	auto tileGid = pMap->getLayer(layerName)->getTileGIDAt(posi);
	Value tiledic = pMap->getPropertiesForGID(tileGid);

	//todo 产生断点，原因未明


	if(tiledic.isNull())
	{
		return false;
	}

	int mv = tiledic.asValueMap().at("conflict").asInt();
	//todo 地图掩码尚未分级
	if (mv==1)
	{
		return false;
	}
	return true;
}

bool AstarMapManager::checkReachAble(TMXTiledMap * pMap,const std::string& layerName,int row,int col)
{
	return checkReachAble(pMap,layerName,Vec2(row,col));
}

//转化为地图距离
cocos2d::Vec2 AstarMapManager::convertToMapPosi(cocos2d::TMXTiledMap * pMap,float x,float y)
{
	/*

			E|  C|
			 |	 |   B
		D	 |	 |   
L			A|	 |            R
			 |	 |
			 |	 |
			 |	F|

	目标是求得DC和BC的长度

		1^2+2^2 = 5；
	   CF 斜45角的网格高度
	   LR 斜45角的网格宽度

	   正弦定理的推论：三边与正弦值比值相等

	   AC^2 = （A.x-LR/2）^2 + (CF-A.y)^2
	   ∠CAR = ∠ACE 
	*/

	float mapWidth = pMap->getMapSize().width * pMap->getTileSize().width;
	float mapHeight = pMap->getMapSize().height * pMap->getTileSize().height;
	float distanceAC;
	double sinCAR;
	double sin11;
	double sin22;
	double cos11;
	double cosCAR;
	int d2x;
	int d2y;
	double ratioHtoWidth = sqrt(5.00000);//斜45度角对角边比高的比率
	double distanceCL = Half_TiledHeight * ratioHtoWidth;//
	//char mch[256];
	if(x > mapWidth/2){
		
	   distanceAC = sqrt(std::pow(x - mapWidth/2,2)  + std::pow(mapHeight - y,2));
	   sinCAR = (mapHeight - y)/distanceAC;
	   cosCAR = (x - mapWidth/2)/distanceAC;
	   sin11 = (sinCAR * 2 - cosCAR) / ratioHtoWidth;



	   cos11 = (sinCAR + cosCAR * 2) / ratioHtoWidth;
	   d2y = distanceAC * 5 / 4 * sin11 / distanceCL;
	   sin22 = (2 * sinCAR + cosCAR) / ratioHtoWidth;
	   d2x = distanceAC * 5 / 4 * sin22 / distanceCL;
	   return Vec2(d2x,d2y);       

	}else{
		distanceAC = sqrt(std::pow(mapWidth/2 - x ,2)  + std::pow(mapHeight - y,2));
	   sinCAR = (mapHeight - y)/distanceAC;
	   cosCAR = (mapWidth/2 - x)/distanceAC;
	   sin11 = (sinCAR * 2 - cosCAR) / ratioHtoWidth;
	   cos11 = (sinCAR + cosCAR * 2) / ratioHtoWidth;
	   d2x = distanceAC * 5 / 4 * sin11 / distanceCL;
	   //sin22 = 4.0 * cos11 / 5 + 3.0 * sin11 / 5;
	   sin22 = (2 * sinCAR + cosCAR) / ratioHtoWidth;
	   d2y = distanceAC * 5 / 4 * sin22 / distanceCL;
	   return Vec2(d2x,d2y);
	}
}