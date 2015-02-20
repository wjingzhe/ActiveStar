
1、项目采用cocos2d-x 3.2开发，C++分支。资源懒人路径，我把整个文件夹共享了，有下面的全部文件：链接：http://pan.baidu.com/s/1ntjzAdV 密码：p3zf

下面的有些步骤可以并发，除了5对前面有依赖之外。

2、下载cocos2d-x 3.2 版本并解压，不需要安装：链接：http://pan.baidu.com/s/1dD8Iv2x 密码：d4zl 解压路径比如：D:/coco2d-x-3.2
3、下载python 2.7.x，不要安装3.x，不兼容。如果使用mac或者linux，你比我熟，这里主要按照win环境。链接：http://pan.baidu.com/s/1pJNrhnt 密码：3t4x。在win环境变量中增加python工具的安装路径：如D:/python2.7.6
4、tiledMap，地图编辑器，可能debug状态对应屏幕或者编辑地图用，或理解2.5D地图布局。链接：http://pan.baidu.com/s/1bnwBl8r 密码：24ji 布局原理如下： 可选安装

                    
		---------------------------------------------------------------------
		|              Col:0   (0,0)   Row:0          |
		|                  (0,1)   (1,0)              |
		|                     (1,1)                   |
		|                                             |
		|                                             |
Col:max |                                     		  |row:Max
		|                                             |
		|                                             |
		|                                             |
		---------------------------------------------------------------------
                   Row:max,Col:max
5、进入安装目录D:/coco2d-x-3.2/，运行setup.py。关于安卓方面，如果不熟悉cocos的话继续阅读：运行命令行可能有三种结果，1、只找到COCOS_CONSOLE_ROOT 2、还找到ANDROID_SDK_ROOT提示输入或enter 3、还找到ANT_ROOT和NDK_ROOT（一般会一闪而过退出命令行）
如果不需要安卓打包，可以全部敲击enter略过，不影响代码在mac和win32上面运行调试；如果需要则浏览此配置博客：http://www.itnose.net/detail/6090401.html
此时已经完成cocos的环境配置。由于项目共享在git上，下载回来的。Lib/a/so等都是不在版本管理中的，所以需要一个其他工程的库借用一下。在命令窗口中输入：cocos new projectName -l cpp -p com.whatevet.xxx [-d projPath] 后面中括号意思是可选。
6、Git项目路径：https://github.com/wjingzhe/ActiveStar 如果不参与开发话，可以直接点右下角的download下载即可，之后解压。大家兴趣参与开发的话，我们进入组织提交申请即可：https://github.com/ReccaStudio
7、把5中项目的cocos2d文件夹复制到6的相同文件夹中，目前0.0.1版可以直接替换。以后可能需要跳过冲突文件。打开vs2012以上的IDE均可运行工程。已注意Mac、Android的路径问题，截止当前已完成路径管理问题；Linux工程待续