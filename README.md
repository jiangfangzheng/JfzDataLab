# JfzDataLab
异想家数据处理实验室

#### 2017-05-24：

* 新的主题界面，规范代码结构。

#### 2017-05-18：

* 界面微调。

#### 2017-05-17：

* 新增波长修复算法：
　　将与标定的波长范围不符，解调仪错误截图出来的通道删除。
　　数据拟合，0值的用前一时刻(后一时刻)的值赋值，前后轮一遍，保证数据空余处补全。
　　异常点消除，温度在30s内变化1.5度视为突变，用前值覆盖。

#### 2017-05-17：

* 新增波长修复算法，屏蔽老的处理接口。

#### 2017-05-16：

* 重新整合了工程，VS编译器真是好啊，换成GCC各种细节要修改。实现MinGW版静态编译，生成的软件体积小很多，压缩后就更小了。

#### 2017-05-14：

* 做了两个版本：
　　一个是QT 5.6.2 VS2013 x64版本的，一个是QT 5.6.1 MinGW版本的。VS版基本功能实现了，用到了编译好的Armadillo的blas_win64_MT.dll、lapack_win64_MT.dll做计算，性能更好。
　　后来发现VS版的依赖太复杂了，拷到别人的电脑上不友好，所以重写了一个MinGW版本的，懒得自己编译blas_win64_MT.dll、lapack_win64_MT.dll，在Armadillo把#define ARMA_USE_LAPACK和#define ARMA_USE_BLAS关了，性能上稍弱一些。以后主要更新MinGW版，跨平台也方便。