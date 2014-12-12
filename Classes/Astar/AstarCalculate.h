
#pragma once
extern class AstarItem;

namespace Calculate
{
	//整个方程的单调性有待证明（公式来自于满硕泉的书中代码，但是该书没有提及公式，使用了一堆魔法数，不知道是水平有限还是故意为之）
	//计算当前节点的耗散值，因为父节点的G为已知量，直接增加每次探索的固定耗散值
	float calculateG(AstarItem * pCurItem,AstarItem * pNewParentItem);

	//寻路的估计值，目前采用线性距离估计。
	float calculateH(int row,int col,int targetX,int targetY);

	int calculateID(int row,int col);

	int calculateID(AstarItem * p);
}