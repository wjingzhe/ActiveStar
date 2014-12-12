#include "AstarCalculate.h"
#include "AstarItem.h"
#include <cmath>

namespace Calculate
{
	//整个方程的单调性有待证明（公式来自于满硕泉的书中代码，但是该书没有提及公式，使用了一堆魔法数，不知道是水平有限还是故意为之）
	//计算当前节点的耗散值，因为父节点的G为已知量，直接增加每次探索的固定耗散值
	float calculateG(AstarItem * pCurItem,AstarItem * pNewParentItem)
	{
		int row = pCurItem->getRow();
		int col = pCurItem->getCol();

		int parentRow = pNewParentItem->getRow();
		int parentCol = pNewParentItem->getCol();
	
		int g = pNewParentItem->getG();
		//todo 耗散值使用的魔法数，下个版本更换为有效的耗散相对值
		//斜向的路程开销是正向的1.4倍
		if (row != parentRow && col != parentCol)
		{
			g+=14;
		}
		else
		{
			g+=10;
		}
		//todo 增加墙角耗散计算
		if (true)
		{

		}
		return g;
	}

	//寻路的估计值，目前采用线性距离估计。
	float calculateH(int row,int col,int targetX,int targetY)
	{
		return std::sqrt(std::pow(targetX-row,2)+std::pow(targetY-col,2))*10;
	}

	int calculateID(int row,int col)
	{
		return row*100+col;
	}
	
	int calculateID(AstarItem * pItem)
	{
		return pItem->getRow()*100+pItem->getCol();
	}
}


