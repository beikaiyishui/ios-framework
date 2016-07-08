P2P点播直播iOS-SDK
===

## 依赖安装

- 首先将**VByteP2P.framework**放入到您的工程目录下
- 然后，将VByteP2P.framework包含到您的工程中，或在工程配置项 -> link Binary With libraries中添加该framework
- 导入系统库**libstdc++.tbd**
- 最后在需要引用的地方 **import"VByteP2P/P2PModule.h"**

## 文档

### P2PModule接口API
> +(int) init:(NSString *)appId appKey:(NSString *)appKey appSecretKey:(NSString *)appSecretKey
 
	描述:    初始化p2p模块，整个应用中只需初始化一次.
	参数:    appId	 		    授权id
	参数:    appkey			    授权key
	参数:    appSecretKey	    授权secretkey
	返回值:  EXIT_SUCCESS 表示成功，否则请查看代理返回错误码.


**example:**

	- (BOOL)application:(UIApplication *)application 		didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
	{
		....
    	NSString *appID = @"575e4062803663d22e9846ae";
    	NSString *key = @"bd8CtLIuhOn0fxCm";
    	NSString *secret = @"7zFeC9iMlTHOyLpIbzAT8oEUnfSMrLut";
	    [P2PModule init:appID appKey:key appSecretKey:secret];
		..... //省略代码
	}  
> +release()
	
	描述:   销毁p2p模块，此操作之后需要重新init才能重新启动p2p.
	返回值:  无。
**example:**
	
	- (void)applicationWillTerminate:(UIApplication *)application {
		[P2PModule release];
	}
> +(void) setDelegate: (id<P2PStreamDelegate>) delegate
	
	描述:    设置事件代理，通过代理能够探测到p2p模块的事件状态，主要包含两类事件
			 普通事件和错误事件.
	参数:    delegate      代理对象
	返回值:  无.

> +(NSString *) version

	描述:    打印库版本信息。
	返回值:  NSString*  当前版本的字符串信息.
> +(void) enableDebug

	描述:    日志开关，允许输出debug 信息.
	返回值:   无。
> +(void) disableDebug

	描述:    日志开关，关闭输出debug 信息.
	返回值:   无.
### LiveController接口API
> +(NSString *) load: (NSString *)channel resolution:(NSString *)resoltion 					   startTime:(double)startTime

	描述:    加载p2p直播频道，通过输入channel等参数，返回可供播放器播放的p2p uri.
     		 若加载失败，P2PStreamDelegate 会返回相应错误码.
    参数:    channel     直播频道ID
    参数:    resoltion   分辨率
    参数:    startTime   播放起始时间（unix标准时间，针对时移功能）
    返回值:   可供播放器播放的P2P uri.

**example:**
	
	-(void) loadChannel:(NSString*)channel {
	    ....
		//原始加载channel
	   [player load channel];
	   //新加载channel
	   NSString* newChannel=[LiveController load:channel resolution:"UHD" startTime:0];
	   [player load:newChannel];
	   .... //省略
	}

> +(void) unload

	描述:     退出当前播放。
	返回值:    无。
### VodController接口API
> +(NSString *) load: (NSString *)uri resolution:(NSString *)resoltion    						startTime:(double)startTime

	描述:     加载p2p点播频道，通过输入uri等参数，返回可供播放器播放的p2p uri,
			  若加载失败，P2PStreamDelegate 会返回相应错误码.
	参数:     uri 请求播放uri
	参数:     resoltion  分辨率
	参数:     startTime  播放偏移时间
	返回值:   可供播放器播放的p2p uri.

**example:**

	-(void) loaduri:(NSString*)uri {
	    ....
		//原始加载uri
	   [player load uri];
	   //新加载uri
	   NSString* newUri=[VodController load:uri resolution:"UHD" startTime:0];
	   [player load:newUri];
	   .... //省略
	}
> +(void) unload
    
    描述:     卸载uri,当播放器停止播放时调用.
    返回值:    无.
> +(void) pause
	
	描述:    暂停播放.
	返回值:  无.
> +(void) resume

	描述:    恢复播放.
	返回值:  无.
### 事件

#### 正常事件

* **CREATE** : 标志着P2P模块的创建成功
* **START**: 标志着P2P成功加载频道
* **STOP**: 表明P2P成功停止了上一个频道（上一个频道可能早被停止过了）
* **EXIT**: 表明P2P模块收到了退出信号，即将退出
* **DESTROY**: 标志着P2P模块成功销毁了自己
* **STUN_SUCCESS**: 表明P2P模块成功获取到了自己的公网地址
* **JOIN_SUCCESS**: 表明P2P模块在载入一个频道的过程中成功加入了P2P的大军
* **HTBT_SUCCESS**: 表明此时当前程序实例没有掉队
* **BYE_SUCCESS**: 表明当前程序实例要退出P2P了，这在播放器停止播放，程序调用unload之后会发生
* **NEW_PARTNER**: 表明当前应用程序又获取了一个伙伴
* **STREAM_READY**: 表明即将载入频道的数据流已经就绪，将会给播放器数据，在播放器有足够的缓冲后（这取决于播放器自己的设定），就会有画面呈现
* **P2P_STABLE**: 表明当前程序实例的P2P效果很稳定
* **BLOCK**: 表明在写数据时遇到了阻塞，这可能会造成播放器的卡顿
* **REPORT**: 表明P2P模块将上传数据，要上传的数据在message里面，是一段json数据

** 注意 **: 请务必处理这些事件时不要执行耗时的操作，因为它跟Android ui主线程一样，如果耗时太久，将会阻止数据流的连续载入；如需要耗时的操作，请使用异步处理。

#### 异常和错误

* **CONF_UNAVAILABLE**: 配置服务器不可用，将停止载入，不会播放！
* **AUTH_FAILED**: 认证失败，此时您应确保您填入的app id，app key， app secret key都正确
* **CONF_INVALID**: 配置不对，此时，应联系运营人员或者我们，及时修改
* **CHANNEL_EMPTY**: 您在载入一个频道时没有传频道或者频道为空
* **RESOLUTION_INVALID**: 该频道不存在这个分辨率，您填写的分辨率不合法或者超出的源本有的清晰度
* **NO_SUCH_CHANNEL**: 不存在你想要播放的频道，请检查和确认你填写的频道是否正确，是否被下线等
* **BAD_NETWORK**: 网络差，或者程序没有连接上网络，这个错误将会在P2P模块联网超时N次超时后抛出
* **STUN_FAILED**: 获取自己的公网地址失败，此时应用程序将退化为和普通CDN一样拉去数据流，将没有P2P效果
* **CDN_UNSTABLE**: 表明CDN不稳定，可能因网络造成，可能因源本身就不太稳定，P2P模块在连续N次获取数据失败后会抛出此错误，并停止加载，您的程序收到此错误后，可让用户刷新重试。
* **JOIN_FAILED**: 加入P2P大军失败，后续会继续尝试
* **HTBT_FAILED**: 表明应用程序已掉队，对P2P效果会减弱，并且可能会带来片刻的卡顿
* **BYE_FAILED**: 退出P2P大军时失败，然而这不会影响当前应用程序从P2P大军中剔除
* **REPORT_FAILED**: 应用程序上报统计数据失败
* **UNKNOWN_PACKET**: 收到一个未知类型的包，将忽略
* **INVALID_PACKET**: 收到一个数据不一致的包，将忽略
* **INTERNAL**: 内部错误
