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


	//��λ��ĩλԪ������ΪmaxHeap,ʣ��Ԫ���Կ�ʼ�±�Ϊ�����µ���Ϊ��
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList,cocos2d::Vector<AstarItem*>::iterator startIt,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc)
	{


		std::swap(*startIt,*(endIt-1));

		//ֻ��Ҫ�Ѷ���Ԫ�������ҳ������ٰ�v����հ�λ�ü���
		placeElem(startIt,endIt,1,lessFunc);

	}

	//���������˼�������ں����map�Ĳ��룬���ڲ���ʱ������Ϊ�����޸����нڵ�����
	//���뵱ǰ�ڵ����ڵ�λ�ã��Կ�ʼ�±�Ϊ�Ѷ������ýڵ��λ��
	void placeElem(cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt, int offsetIndedx, AstarLessThan lessFunc)
	{
		int endIndex = endIt - startIt;
		if (endIndex <= 1 || offsetIndedx <= 0)
		{
			return ;
		}

		//����
		auto v = *(startIt+offsetIndedx);//�����ӽڵ㸱��

		int k = offsetIndedx,parentIndex = 0;
			
		bool heap = false;
		//�����ӽڵ�
		while(!heap && 2*k+1 < endIndex )//ʵ�ָ�ĸ����
		{
			parentIndex = 2*k+1;
			if(parentIndex+1<endIndex)//������������
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
				*(startIt+k) = *(startIt+parentIndex);//�����׽ڵ㸳ֵ�ӽڵ�λ��
				k = parentIndex;//����ָ��ɸ��׽ڵ�
			}
		}//while
		*(startIt+k) = v;


		//���ϱ���������ǰ�ӶѵĶѶ�Ԫ�ط��ں���λ��
		v = *(startIt+offsetIndedx);

		heap = false;

		for (k = offsetIndedx; !heap&& k>0;)
		{
			parentIndex = k/2;
			
			if ( !lessFunc( *(startIt+parentIndex),*(startIt+k) ) )
			{
				*(startIt+k) = *(startIt+parentIndex);//�����׽ڵ㸳ֵ�ӽڵ�λ��
					k = parentIndex;//����ָ��ɸ��׽ڵ�
			}
			//������������
			else
			{
				heap = true;
			}
		}//for
		
		*(startIt+k) = v;
	}//placeElem

}//HeapSort
