//
//  FYJWave.h
//  FYJWave
//
//  Created by fuyujiao on 2017/11/23.
//  Copyright © 2017年 fyj. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^WaveFloatCenterYCallBack)(CGFloat centerY);

@interface FYJWave : UIView
{
    WaveFloatCenterYCallBack _waveFloatCenterYCallBack;
}
//设置参数
//浪高 默认为5
@property (nonatomic, assign) CGFloat waveHeight;
//浪曲度 默认为1
@property (nonatomic, assign) CGFloat waveCurve;
//浪速 默认为1
@property (nonatomic, assign) CGFloat waveSpeed;
//实浪颜色 默认为白色
@property (nonatomic, copy) UIColor *realWaveColor;
//遮罩浪颜色 默认为白色+0.5的透明度
@property (nonatomic, copy) UIColor *maskWaveColor;

//返回浮动的中间Y值的回调
- (void)setWaveFloatYCallBack:(WaveFloatCenterYCallBack)callBack;

//开始浪
- (void)startWaveAnimation;
//停止浪
- (void)endWaveAnimation;




@end
