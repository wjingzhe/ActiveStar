/****************************************************************************
Copyright (c) 2014-2015 Jingz @ ReccaStudio.


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
			placeElem(startIt,endIt-startIt,n/2-j,lessFunc);
		}
		


	}//createNewHeap


	//在位将末位元素生成为maxHeap,剩余元素以开始下标为顶重新调整为堆
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList,cocos2d::Vector<AstarItem*>::iterator startIt,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc)
	{


		std::swap(*startIt,*(endIt-1));

		//只需要把顶点元素重新找出来，再把v放入空白位置即可
		placeElem(startIt,endIt-1-startIt,1,lessFunc);

	}

	//末位增加元素，以开始下标为顶重新调整为堆
	void placeElem(cocos2d::Vector<AstarItem*>::iterator startIt,int n,int i,AstarLessThan lessFunc)
	{
		if (n<=1 || i<=0)
		{
			return ;
		}

		//向下
		auto v = *(startIt-1+i);

		int k = i,j = 0;
			
		bool heap = false;

		while(!heap && 2*k <= n )//实现父母优势
		{
			j = 2*k;
			if(j<n)//存在两个儿子
			{

				if ( !lessFunc( *(startIt-1+j),*(startIt-1+j+1) ) )
				{
					j = j+1;
					//std::swap(*(startIt-1+j),*(startIt-1+j+1));
				}

			}

			if(  lessFunc(v,*(startIt-1+j)) )
			{
				heap = true;
			}
			else 
			{
				*(startIt-1+k) = *(startIt-1+j);
				k = j;
			}
		}//while
		*(startIt-1+k) = v;


		//向上
		v = *(startIt-1+i);

		heap = false;

		for ( k = i; !heap; )	
		{
			j = k/2;
			if (j<1)
			{
				break;
			}

			if ( !lessFunc( *(startIt-1+k),*(startIt-1+j) ) )
			{
				*(startIt-1+k) = *(startIt-1+j);
					k = j;
			}
			else
			{
				heap = true;
			}
		}//for
		
		*(startIt-1+k) = v;
	}























}//HeapSort
