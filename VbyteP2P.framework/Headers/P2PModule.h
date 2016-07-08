//
//  ios.h
//  ios
//
//  Created by 张鹏 on 15/7/30.
//  Copyright (c) 2015年 Vbyte Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, EventCode) {
    LOAD = 100,
    START,
    STOP,
    EXIT,
    DESTROY,
    STUN_CONNECTED,
    JOIN,
    HTBT,
    BYE,
    NEW_PARTNER,
    STREAM_READY,
    P2P_STABLE,
    BLOCK,
    REPORT
};

typedef NS_ENUM(NSInteger, ErrorCode) {
    CONF_UNAVAILABLE = 200,
    AUTH_FAILED,                        //验证失败
    CONF_INVALID,                       //CONF失败
    CHANNEL_EMPTY,                      //频道为空,直播有效
    NO_SUCH_CHANNEL,                    //没有该频道
    RESOLUTION_INVALID,                 //无效分辨率
    BAD_NETWORK,                        //网络异常
    STUN_FAILED,                        //穿透失败
    CDN_UNSTABLE,                       //cdn不稳定
    JOIN_FAILED,                        //peer加入失败
    HTBT_FAILED,                        //已掉队
    BYE_FAILED,                         //离开p2p失败
    REPORT_FAILED,                      //上报数据失败
    UNKNOWN_PACKET,                     //未知的数据包
    INVALID_PACKET,                     //无效的数据包
    INTERNAL                            //内部错误
};

// 事件代理,上层应用需集成来监听事件状态和错误信息
@protocol P2PStreamDelegate <NSObject>

@required
// 事件信息接口
- (void) onEvent: (int)code msg:(NSString *)msg;
// 错误信息接口
- (void) onError: (int)code msg:(NSString *)msg;

@end
//----------------- p2pModule ---------------------------------------
@interface P2PModule : NSObject
// P2PModul 依赖libstd++.tdb, videotool.framework
// 初始化P2P模块，在didFinishLaunchingWithOptions中调用
+ (int) init:(NSString *)appId appKey:(NSString *)appKey appSecretKey:(NSString *)appSecretKey;
// P2P模块释放函数，在applicationWillTerminate中调用
+ (void) release;
// 设置事件代理
+ (void) setDelegate: (id<P2PStreamDelegate>) delegate;
+ (id<P2PStreamDelegate>) getDelegate;
// 打印版本信息
+ (NSString *) version;
// debug 开关
+ (void) enableDebug;
+ (void) disableDebug;

@end

//----------------- p2p 直播接口 ---------------------------------------
@interface LiveController : NSObject
/*
     *  加载p2p直播频道，通过输入channel等参数，返回可供播放器播放的p2p uri.
     *  若加载失败，P2PStreamDelegate 会返回相应错误码.
     *  param[in] channel      直播频道ID
     *  param[in] resoltion    分辨率
     *  param[in] startTime    播放起始时间（unix标准时间，针对时移功能）
     *  ret   可供播放器播放的p2p uri.
 */
+ (NSString *) load: (NSString *)channel resolution:(NSString *)resoltion startTime:(double)startTime;
// 退出播放
+ (void) unload;

@end

//----------------- p2p 点播接口 -------------------------------------------
@interface VodController : NSObject
/*
    *  加载p2p点播频道，通过输入uri等参数，返回可供播放器播放的p2p uri.
    *  若加载失败，P2PStreamDelegate 会返回相应错误码.
    *  param[in] uri          视频原始uri
    *  param[in] resoltion    分辨率
    *  param[in] startTime    播放偏移时间（s）
    *  ret   可供播放器播放的p2p uri.
*/
+ (NSString *) load: (NSString *)uri resolution:(NSString *)resoltion    startTime:(double)startTime;

// 退出播放，当切换频道或关闭时调用该接口
+ (void) unload;
// 暂停播放
+ (void) pause;
// 恢复播放
+ (void) resume;

@end
