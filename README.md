# JfzDataLab
异想家数据处理实验室

截图：
![](http://file.jfz.me/1.png)

#### 2017-08-26

* 环境温度Xls文件读取终于修复了。
* 精简代码。

#### 2017-08-15

* 电类环境温度标准化。
* 修复抽样算法问题。
* 修复相关性分析算法。
* 数据诊断完善。

#### 2017-08-14

* 数据概览以新的形式展示。
* 关于页面美化、代码优化。
 
#### 2017-08-09

* 处理Csv、Mat功能分开。

#### 2017-08-05

* 界面调整。
* 几乎所有算法多线程化、异步化。

#### 2017-08-03

* 精简没必要的功能、代码重构、界面重组。

#### 2017-07-20

* 支持数据概览界面显示未分类数据。

#### 2017-07-08

* 增加数据合并功能。
* 增加一键转换为虚拟映射文件功能。
* 增加txt温度文件处理功能。
* 新的电类数据ID排序算法，防止数组越界。

#### 2017-07-04

* 增加数据虚拟映射功能，整理为新的规范文件。
* 增加数据按天拆分为文件的小处理功能。
* 坑爹的VS2015不支持C99变长数组，改了一大堆代码。

#### 2017-06-30

* 增加数据小处理功能-归零、增量化、数据压缩拉伸、增量化等。

#### 2017-06-22

* 处理进度显示。
* 处理异步化、多线程化。

#### 2017-06-20

* 多线程功能优化，学到了更好的写法。
* 新的数据标定算法，适合新数据。

#### 2017-06-19

* 增加更新检测功能。
* 多元线性回归优化算法，增加C语言数组导出。

#### 2017-06-18

* 完善数据库更新功能。
* 优化数据导出功能。
* 各种细节优化，修正某些数据不支持中文的问题。

#### 2017-06-17

* 增加Log保存功能。
* 增加模型预测功能。
* 增加SQL数据导出功能。

#### 2017-06-11

* 小改信号和槽，优化代码。
* 增加32位lapack和blas，MinGW版本可用。

#### 2017-06-07

* 新增导入数据库功能(功能雏形，很不完善)。

#### 2017-06-05

* 修复绘图第一张图片显示Bug。
* 增加托盘图标。

#### 2017-06-03

* 绘图功能继续优化，增加JIO类方便读取文件。

#### 2017-06-01

* 高DPI优化。

#### 2017-05-30

* 增加多元线性回归处理功能。
* 增加绘图功能。
* 增加PS黑皮肤(有BUG)。
* 封装Mat、csv读取类。

#### 2017-05-27

* 重构大量代码。
* 增加换肤系统。

#### 2017-05-25

* 增加版权信息！

#### 2017-05-24

* 新的主题界面，规范代码结构。

#### 2017-05-18

* 界面微调。

#### 2017-05-17

* 新增波长修复算法：

　　将与标定的波长范围不符，解调仪错误截图出来的通道删除。

　　数据拟合，0值的用前一时刻(后一时刻)的值赋值，前后轮一遍，保证数据空余处补全。

　　异常点消除，温度在30s内变化1.5度视为突变，用前值覆盖。

#### 2017-05-17

* 新增波长修复算法，屏蔽老的处理接口。

#### 2017-05-16

* 重新整合了工程，VS编译器真是好啊，换成GCC各种细节要修改。实现MinGW版静态编译，生成的软件体积小很多，压缩后就更小了。

#### 2017-05-14

* 做了两个版本：

　　一个是QT 5.6.2 VS2013 x64版本的，一个是QT 5.6.1 MinGW版本的。VS版基本功能实现了，用到了编译好的Armadillo的blas_win64_MT.dll、lapack_win64_MT.dll做计算，性能更好。

　　后来发现VS版的依赖太复杂了，拷到别人的电脑上不友好，所以重写了一个MinGW版本的，懒得自己编译blas_win64_MT.dll、lapack_win64_MT.dll，在Armadillo把#define ARMA_USE_LAPACK和#define ARMA_USE_BLAS关了，性能上稍弱一些。以后主要更新MinGW版，跨平台也方便。