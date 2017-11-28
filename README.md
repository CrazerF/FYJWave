# FYJWave

### 直接使用：
1. 将头文件FYJWave.h导入

         #import "FYJWave.h"
  
2. 创建FYJWave浪对象

         FYJWave *fyjWave = [[FYJWave alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, 200)];
         fyjWave.backgroundColor = [UIColor redColor];
         [self.view addSubview:fyjWave];
    
3. 开始动画

        [fyjWave startWaveAnimation];
    
### 其他方法：
传回的centerY值,修改其它需要跟着一起动的图层的Y坐标。+ centerY实现的动作和浪一致，- center实现的动作和浪的相反，如

       [fyjWave setWaveFloatYCallBack:^(CGFloat centerY) {
        CGRect frame = imageView.frame;
        frame.origin.y = 100 + centerY;
        imageView.frame = frame;
      }];

 结束动画
 
    [fyjWave endWaveAnimation];



### 实现思路
1. 定义一个View
2. 运用CGMutablePathRef和CAShapeLayer画出静态正弦函数
3. 运用帧刷新类CADisplayLink，不断的改变路径上的各个点，让波浪动起来
4. 回调每一帧的中点值，可以增加动态的圆形等自定义视图

### 复习一下三角函数
假设 y = Asin（ωx+φ）+ C
A 表示振幅，也就是使用这个变量来调整波浪的最大的高度
ω 与周期相关，周期 T = 2 x pi / ω ，这个变量用来调整同宽度内显示的波浪的数量
φ 表示波浪横向的偏移，也就是使用这个变量来调整波浪的流动
C 表示波浪纵向偏移的位置。

[正弦函数伸缩变换](https://wenku.baidu.com/view/beda69870029bd64783e2c28.html)

[三角函数平移变换和周期变换](https://wenku.baidu.com/view/3ecdb3f0b0717fd5360cdc80.html)

### CADisplayLink
一种精确到帧的定时器,其根本利用刷帧和屏幕频率一样来重绘渲染页面。
 注意1：初始CADisplayLink对象，指定target的时候，其属于单例，会保留其目标对象，而CADisplayLink的目标对象如果恰好保留了计时器本身，就会导致循环引用。
注意2：frameInterval的值默认为1，真机刷新频率是每秒60次，所以每一次调用的“target”函数的运行的时间大于1/60秒，就会出现严重的丢帧现象。frameInterval的值默认为2时，刷新频率就变为了每秒30次。

其创建方式:

        CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(wave)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

### NSRunLoop
什么是run loop ，简单说下：让线程在有任务的时候忙于工作，而没任务的时候处于休眠状态。
run loop线程开启的时候便被创建，所以只需要在当前线程获取即可。
1. NSDefaultRunLoopMode
App的默认 Mode，通常主线程是在这个 Mode 下运行的。
2. UITrackingRunLoopMode
界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响。
3. NSRunLoopCommonModes
公共的mode，即是这个Mode下的事件，可以在其他的Mode下被运行。

CADisplayLink、NSTimer执行的小动画，在拖动scrollView的时候动画会被暂停，此时就需要设置所加在的run loop 的mode了。

### 主要代码：
    
    //创建浪的路径
    CGMutablePathRef wavePath = CGPathCreateMutable();
    
    //创建一个起点
    CGPathMoveToPoint(wavePath, NULL, 0, height);
    
    //创建中间点
    for (int x = 0; x < width; x++) {
        //主要的实现算法就是y = Asin（ωx+φ） 
        CGFloat y = height  * sinf(self.waveCurve * x * M_PI / 180 + offset * M_PI / 180);
        CGPathAddLineToPoint(wavePath, NULL, x, y);
    }
    
    //调整填充路径
    CGPathAddLineToPoint(wavePath, NULL, width, height);
    CGPathAddLineToPoint(wavePath, NULL, 0, height);
    
    //结束路径
    CGPathCloseSubpath(wavePath);
    
    //用路径创建浪
    waveShapeLayer.path = wavePath;
    waveShapeLayer.fillColor = waveWolor.CGColor;
    
    //释放浪路径
    CGPathRelease(wavePath);

当开始动画的时候将CADisplayLink对象加到runLoop中
      
     [self.displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

结束动画时注销掉CADisplayLink对象
     
     [self.displayLink invalidate];
    self.displayLink = nil;
    
    
 ### 如果觉得对你有帮助，不要吝啬你的Star噢~
