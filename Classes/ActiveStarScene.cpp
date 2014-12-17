#include "ActiveStarScene.h"
#include "Astar/Astar.h"

USING_NS_CC;

float ActiveStarScene:: m_fDeltaXCount = 0.0f;

float ActiveStarScene::m_fDeltaYCount = 0.0f;


ActiveStarScene::ActiveStarScene()
	:m_nItemIndex(-1),m_nStepIndex(-1)
	,m_pPath(nullptr),m_pTamara(nullptr),m_pMap(nullptr),m_pAstar(nullptr)
	
{
	m_bIsSearching = false;
}

ActiveStarScene::~ActiveStarScene()
{
	if (m_pPath!=nullptr)
	{
		m_pPath->clear();
		delete m_pPath;
		m_pPath = nullptr;
	}

	//if (m_pTamara!=nullptr)
	//{
	//	m_pTamara->release();
	//	m_pTamara = nullptr;
	//}

	//if (m_pMap!=nullptr)
	//{
	//	m_pMap->release();
	//	m_pMap = nullptr;
	//}

	if (m_pAstar!=nullptr)
	{
		delete m_pAstar;
		m_pAstar = nullptr;
	}

	//
	this->getEventDispatcher()->removeCustomEventListeners("AlterMapPosi");

}


Scene* ActiveStarScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ActiveStarScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ActiveStarScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	if (m_pPath==nullptr)
	{
		m_pPath = new Vector<AstarItem*>();
	}
	else
	{
		m_pPath->clear();
	}
	


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ActiveStarScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    

	m_pMap = TMXTiledMap::create("iso-test-zorder.tmx");

	this->addChild(m_pMap,0,kTagTileMap);

	m_pMap->setPosition(Vec2(-m_pMap->getContentSize().width/2+visibleSize.width/2,-m_pMap->getContentSize().height/2+visibleSize.height/2));




	m_pTamara = spine::SkeletonAnimation::createWithFile("spine/spineboy.json", "spine/spineboy.atlas");;

	m_pTamara->setMix("drawOrder","walk",0.5f);

	m_pTamara->setMix("walk","drawOrder",0.5f);

	m_pTamara->setAnimation(0,"drawOrder",true);

	m_pTamara->stopAllActions();
	

	m_pTamara->setScale(0.2,0.2);





	m_pTamara->retain();
	m_pTamara->setAnchorPoint(Vec2(0.5f,0));//以脚下为锚点，这样子可以直观地图是否可达
	m_pMap->addChild(m_pTamara,m_pMap->getChildrenCount());

	m_pTamara->release();

	float nMapWidth = m_pMap->getMapSize().width * m_pMap->getTileSize().width;
	float nMapheight = m_pMap->getMapSize().height * m_pMap->getTileSize().height;




	//todo 地址处理，不能处于障碍物身上
	m_pTamara->setPosition(nMapWidth/2+10,nMapheight/2+5);
	

	//todo
	m_nItemIndex = -1;
	//每次移动的步伐
	m_nStepIndex = -1;

	if (m_pAstar==nullptr)
	{
		m_pAstar = new Astar();
	}

	

	auto touchListner = EventListenerTouchOneByOne::create();

	//touchListner->setSwallowTouches(true);//吞噬点触摸事件

	touchListner->onTouchBegan = CC_CALLBACK_2(ActiveStarScene::touchBegan,this);

	this->getEventDispatcher()->addEventListenerWithFixedPriority(touchListner,-129);

	this->scheduleUpdate();

	this->getEventDispatcher()->addCustomEventListener("AlterMapPosi",CC_CALLBACK_0(ActiveStarScene::alterMapPosi,this));

    return true;
}


void ActiveStarScene::alterMapPosi()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pMap->setPosition(Vec2(-m_pMap->getContentSize().width/2+visibleSize.width/2-m_fDeltaXCount,-m_pMap->getContentSize().height/2+visibleSize.height/2-m_fDeltaYCount));

	m_fDeltaXCount = 0.0f;
	m_fDeltaYCount = 0.0f;
}

void ActiveStarScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//todo
//1和0.5的地图标尺未清楚
//斜45度地图的0点在地图上部的中间，往左下角方向为col正方向，往右下角方向为row正方向
//确认8方位移动方向，含路径处理.因为是斜45度角的地图，暂时不支持正视角，路径的更新采用斜0.5方式
void ActiveStarScene::update(float dt)
{
	Vec2 heroPosi = m_pTamara->getPosition();


	//todo 为什么是1
	
	if (!m_bIsSearching&&m_nItemIndex>=1&&m_pPath->size()>1&&m_nStepIndex>=0)
	{//todo 为什么是0
		
		

		CCAssert(m_pPath!=nullptr,"path is nullptr,empty");
		int nNextRow = m_pPath->at(m_nItemIndex)->getRow();
		int nNextCol = m_pPath->at(m_nItemIndex)->getCol();
		int nCurRow = m_pPath->at(m_nItemIndex-1)->getRow();
		int nCurCol = m_pPath->at(m_nItemIndex-1)->getCol();

		//x = 2y;x+2y = 2;
		if(nNextRow < nCurRow )
		{
			if (nNextCol < nCurCol)//0
			{
				heroPosi.y+=1;
			}
			else if(nNextCol > nCurCol)//6
			{
				heroPosi.x-=2;
					
			}
			else//7
			{
				heroPosi.x-=1;
				heroPosi.y+=0.5;
			}
		}
		else if(nNextRow > nCurRow)
		{
			if (nNextCol < nCurCol)//2
			{
				heroPosi.x+=2;
			}
			else if(nNextCol > nCurCol)//4
			{
				heroPosi.y-=1;
			}
			else//3
			{
				heroPosi.x+=1;
				heroPosi.y-=0.5;
			}
		}
		else if (nNextRow == nCurRow)//同一行
		{
			if (nNextCol < nCurCol)//1
			{
				heroPosi.x+=1;
				heroPosi.y+=0.5;
			}
			else if(nNextCol > nCurCol)
			{
				heroPosi.x-=1;
				heroPosi.y-=0.5;
			}
			else //当前位置
			{
				//不执行移动
			}
		}//8方位移动


		//每次刷新移动一个像素点，每32次移动一格

		++m_nStepIndex;
		
m_pTamara->setPosition(heroPosi);
	float nMapWidth = m_pMap->getMapSize().width * m_pMap->getTileSize().width;
	float nMapheight = m_pMap->getMapSize().height * m_pMap->getTileSize().height;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//人物和地图运动方向相反
	float deltaX = heroPosi.x - nMapWidth/2;
	float deltaY = heroPosi.y - nMapheight/2;


	m_fDeltaXCount+=deltaX;
	m_fDeltaYCount+=deltaY;

	auto tempPosi = m_pMap->convertToWorldSpace(heroPosi);

//保证人物始终在地图的中间位置

	m_pMap->setPosition(Vec2(-m_pMap->getContentSize().width/2+visibleSize.width/2-m_fDeltaXCount,-m_pMap->getContentSize().height/2+visibleSize.height/2-m_fDeltaYCount));

	m_fDeltaXCount = 0.0f;
	m_fDeltaYCount = 0.0f;


	}// if (m_nItemIndex>=1)


	

	//if (std::abs(tempPosi.x-visibleSize.width/2)>300 || std::abs(tempPosi.y-visibleSize.height/2)>200)
	//{
	//	//m_pMap->runAction(MoveTo::create(1.5f,Vec2(-m_pMap->getContentSize().width/2+visibleSize.width/2-m_fDeltaXCount,-m_pMap->getContentSize().height/2+visibleSize.height/2-m_fDeltaYCount)));

	//	this->getEventDispatcher()->dispatchEvent(new EventCustom("AlterMapPosi"));
	//}
	//
	
	if (m_nStepIndex>=32)
		{
			m_nStepIndex= 0;
			++m_nItemIndex;
			if (m_nItemIndex>=m_pPath->size())
			{
				//结束寻路刷新
				m_nItemIndex = -1;
				m_nStepIndex = -1;
				m_pPath->clear();
				log("jingz--------Hero--endPosition:%f,%f",heroPosi.x,heroPosi.y);
				m_pTamara->setAnimation(0,"drawOrder",true);
				return ;
			}
		}


	
}


bool ActiveStarScene::touchBegan(Touch  *touch, Event  *event)
{

	log("jingz----------------touching_began");
	if (m_bIsSearching)
	{
		return true;
	}
	
	log("jingz----------touch:%f,%f",touch->getLocation().x,touch->getLocation().y);
	//转化为节点坐标
	Vec2 t_vbeginPosi = touch->getLocation();

	Vec2 t_vMapPosi = m_pMap->getPosition();

	

	// 
	Vec2 t_vAimPosiInMap = AstarMapManager::convertToMapPosi(m_pMap,t_vbeginPosi.x- t_vMapPosi.x ,t_vbeginPosi.y-t_vMapPosi.y);

	if (t_vAimPosiInMap.x<0 || t_vAimPosiInMap.y<0 || t_vAimPosiInMap.x>=m_pMap->getMapSize().width ||t_vAimPosiInMap.y>=m_pMap->getMapSize().height)
	{
		//无效目标，直接结束
		return true;
	}
		
	Vec2 heroPosi = m_pTamara->getPosition();
	Vec2 heroPosiInMap = AstarMapManager::convertToMapPosi(m_pMap,heroPosi.x,heroPosi.y);
	

	if(!AstarMapManager::checkReachAble(m_pMap,"grass",t_vAimPosiInMap))
	{
		return true;
	}

	/*if(!AstarMapManager::checkReachAble(m_pMap,"grass",heroPosiInMap))
	{
		if(!AstarMapManager::checkReachAble(m_pMap,"grass",heroPosiInMap.x+1,heroPosiInMap.y))
		{
			m_pTamara->setPosition(heroPosi.x+32,heroPosi.y-16);
		
		}
		else if(!AstarMapManager::checkReachAble(m_pMap,"grass",heroPosiInMap.x-1,heroPosiInMap.y))
		{
			m_pTamara->setPosition(heroPosi.x-32,heroPosi.y+16);
		
		}
		else if(!AstarMapManager::checkReachAble(m_pMap,"grass",heroPosiInMap.x,heroPosiInMap.y-1))
		{
			m_pTamara->setPosition(heroPosi.x-32,heroPosi.y-16);
		
		}
		else if(!AstarMapManager::checkReachAble(m_pMap,"grass",heroPosiInMap.x,heroPosiInMap.y-1))
		{
			m_pTamara->setPosition(heroPosi.x+32,heroPosi.y+16);
		
		}

	}*/


	
	//todo 多次寻路问题


	

	m_bIsSearching = true;

	if (m_pPath!=nullptr)
	{
		m_pPath->clear();
	}

	CCAssert(m_pAstar!=nullptr,"jingz-------------------");



#ifdef Jingz_Debug

	bool flag = m_pAstar->findPath(Vec2(20,7),Vec2(9,13),m_pMap,m_pPath);

#else

bool flag = m_pAstar->findPath(heroPosiInMap,t_vAimPosiInMap,m_pMap,m_pPath);

#endif
	m_fDeltaXCount = 0.0f;
	m_fDeltaYCount = 0.0f;

	m_nItemIndex = 1;
	m_nStepIndex = 0;



	m_bIsSearching = false;
	log("jingz----------------%d,%s",flag,__FUNCTION__);



	m_pTamara->setAnimation(0,"walk",true);


	for (auto i = m_pPath->begin(); i != m_pPath->end(); ++i)
	{
		log("jingz----------------%d,%d,%f,%f,%f",(*i)->getRow(),(*i)->getCol(),(*i)->getG(),(*i)->getH(),(*i)->getF());
	}

	

	return true;
}