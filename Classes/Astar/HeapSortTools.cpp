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
#include <algorithm>

namespace HeapSort
{
	
	void createNewHeap(cocos2d::Vector<AstarItem*> * vItemList, cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt, AstarGreaterThan greaterFunc, int nodesCount)
	{
		if(endIt<=startIt+1) return;

		int n = endIt-startIt;
		
		for (int j = 0; j<nodesCount; ++j)
		{
			placeElem(startIt, endIt, n - j, greaterFunc);
		}
		


	}//createNewHeap


	//在位将末位元素生成为maxHeap,剩余元素以开始下标为顶重新调整为堆
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList, cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt, AstarGreaterThan greaterFunc)
	{


		std::swap(*startIt,*(endIt-1));

		//只需要把顶点元素重新找出来，再把v放入空白位置即可
		placeElem(startIt, endIt - 1, 1, greaterFunc);

	}

	//排序的整体思想类似于红黑树map的插入，已在插入时有序，因为可能修改已有节点数据，不能使用stl的容器
	//传入当前节点所在的位置，以开始下标为堆顶调整该节点的位置
	void placeElem(cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt,
		int i, AstarGreaterThan greaterFunc)
	{
		std::make_heap(startIt, endIt, greaterFunc);
	}//placeElem























}//HeapSort
