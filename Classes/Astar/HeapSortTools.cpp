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

#include "HeapSortTools.h"

namespace HeapSort
{
	
	void createNewHeap(cocos2d::Vector<AstarItem*> * vItemList,cocos2d::Vector<AstarItem*>::iterator startIt,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc,int nodesCount)
	{
		if(endIt<=startIt+1) return;

		int n = endIt-startIt;
		
		for (int j = 0; j<nodesCount; ++j)
		{
			placeElem(startIt, endIt, n - j, lessFunc);
		}
		


	}//createNewHeap


	//在位将末位元素生成为maxHeap,剩余元素以开始下标为顶重新调整为堆
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList,cocos2d::Vector<AstarItem*>::iterator startIt,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc)
	{


		std::swap(*startIt,*(endIt-1));

		//只需要把顶点元素重新找出来，再把v放入空白位置即可
		placeElem(startIt,endIt,1,lessFunc);

	}

	//排序的整体思想类似于红黑树map的插入，已在插入时有序，因为可能修改已有节点数据
	//传入当前节点所在的位置，以开始下标为堆顶调整该节点的位置
	void placeElem(cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt, int offsetIndedx, AstarLessThan lessFunc)
	{
		int endIndex = endIt - startIt;
		if (endIndex <= 1 || offsetIndedx <= 0)
		{
			return ;
		}

		//向下
		auto v = *(startIt+offsetIndedx);//保持子节点副本

		int k = offsetIndedx,parentIndex = 0;
			
		bool heap = false;
		//插入子节点
		while(!heap && 2*k+1 < endIndex )//实现父母优势
		{
			parentIndex = 2*k+1;
			if(parentIndex+1<endIndex)//存在两个儿子
			{

				if ( !lessFunc( *(startIt+parentIndex),*(startIt+parentIndex+1) ) )
				{
					parentIndex = parentIndex+1;
				}

			}

			if(  lessFunc(v,*(startIt+parentIndex)) )
			{
				heap = true;
			}
			else 
			{
				*(startIt+k) = *(startIt+parentIndex);//将父亲节点赋值子节点位置
				k = parentIndex;//重新指向旧父亲节点
			}
		}//while
		*(startIt+k) = v;


		//向上遍历，将当前子堆的堆顶元素放在合理位置
		v = *(startIt+offsetIndedx);

		heap = false;

		for (k = offsetIndedx; !heap&& k>0;)
		{
			parentIndex = k/2;
			
			if ( !lessFunc( *(startIt+parentIndex),*(startIt+k) ) )
			{
				*(startIt+k) = *(startIt+parentIndex);//将父亲节点赋值子节点位置
					k = parentIndex;//重新指向旧父亲节点
			}
			//否则已是有序
			else
			{
				heap = true;
			}
		}//for
		
		*(startIt+k) = v;
	}//placeElem

}//HeapSort
