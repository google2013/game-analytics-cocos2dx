# TalkingData Game Analytics Cocos集成指南


### 目录
*   C++
	*   [为游戏申请APP ID](#Common)
	*   [iOS快速集成](#iOS)
	*   [Android快速集成](#Android)
	*   [初始化SDK](#initSDK)
	*   [SDK API](#SDKAPI)


*   Lua


#### <a name="Common"></a> 为游戏申请APP ID：
* 进入 [TalkingGame.com](https://www.talkingdata.com) 网站，使用您的注册账号登录后，请预先创建一款游戏，您将获得一串32位的16进制APP ID，用于唯一标识您的一款游戏。

##### 下载 [SDK Zip包](https://www.talkingdata.com/game/download/Game_Analytics_SDK_IOS_V3.2.3.zip)。
##### 导入依赖库：
* 解压缩SDK包当中的 `Game_Analytics_SDK_Cocos.zip` 文件。  

　　　　　　　　 <img src="images/gameCocos01.png" width="400px" />	

* 复制整个 `TalkingDataGameAnalytics` 文件夹到 cocos2d/plugin/plugins 文件夹下。

　　　　　　　　<img src="images/gameCocos06.png" width="400px" />	

### <a name="iOS"></a>iOS快速集成
##### 打开创建好的游戏工程，添加 `TalkingDataGameAnalytics` 里的 `include` 和 `ios` 到 `xcode` 项目的 `ios` 目录下
　　　　　　　　<img src="images/gameCocos03.png" width="200px" />	
##### <a name="addlib"></a>添加依赖的框架
　　`TalkingData Game Analytics` 需要使用 `Security.framework` 框架辅助存储设备标识，使用 `CoreTelephony.framework` 框架获取运营商标识，使用 `AdSupport.framework` 获取 `advertisingIdentifier` ，使用 `SystemConfiguration.framework` 检测网络状况，使用 `libz.dylib` (xcode7或以后为lib.tbd)进行数据压缩，使用 `CFNetwork.framework` 做网络传输。 `Xcode` 的添加方式如下所示：    
　　
　　　　　　<img src="images/gameCocos04.png" width="400px" />　　

### <a name="Android"></a>Android快速集成
##### 打开创建好的游戏工程，把 `TalkingDataGameAnalytics` 文件夹 copy 到您的工程所引用的 `cocos` 库的 `external` 目录下。
　　　　　　　　<img src="images/Image001.png" width="600px" />

##### 在 `Android Studio`集成
1. 在 `Android Studio` 里打开(或导入)你自己的游戏工程。
2. 把 `TalkingDataGameAnalytics/andorid/Game_Analytics_SDK_Android_xxx.jar`复制到 `libs` 
3. 修改 `jni/main.cpp` 文件.  
	* 添加头文件 `#include "TDGAJniHelper.h"`。
	* 在 `cocos2.x` 版本中在 `JNI_OnLoad()` 方法中添加
	* 
			TDGAJniHelper::setJavaVM(vm);
	* 在 `cocos3.x` 版本中在 `cocos_android_app_init()`方法中添加
	
			TDGAJniHelper::setJavaVM(JniHelper::getJavaVM());
4. 修改游戏项目中的 `jni/Android.mk`。
	* 在 `LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static` 后面添加

			LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx-talkingdata。
	* 在$(call import-module,.)后面添加

			$(call import-module,TalkingDataGameAnalytics/android)。
		<img src="images/Image002.png" width="600px" />
5. 运行 `TalkingData` 的 `Demo`。

	* 为了减小 `SDK` 包的大小,`TD` 的 `demo` 中已经把里面 `cocos2d` 文件夹删除,还需要从其它的任意 `cocos` 项目中把 `cocos2d` 整个文件夹拷贝过去

		<img src="images/Image003.png" width="600px" />

#### <a name="initSDK"></a>初始化SDK
##### IOS SDK 初始化
在 `AppDelegate.cpp 文件里导入 `TalkingData.h` 头文件。　


　　　　　　　　<img src="images/gameCocos07.png" width="400px" />  
　　　　

在 `applicationDidFinishLaunching` 方法初始化SDK,  `TalkingDataGA` 调用 `onStart` 方法。　
　　　　　　　　<img src="images/gameCocos08.png" width="400px" />
　　　　　　　　
　　　　　　　　

在Xcode上运行，在控制台看到下面结局标识SDK初始化成功，当红色箭头 `TalkingDataGA: Game data send success` Log出现时标识数据发送成功。　　　　
　　　　　　　
　　　　　　　　<img src="images/gameCocos09.png" width="900px" />

##### Android SDK 初始化
###### 配置AndroidManifest.xml
- `INTERNET`	允许程序联网和发送统计数据的权限。    
* `ACCESS_NETWORK_STATE`	允许应用检测网络连接状态，在网络异常状态下避免数据发送，节省流量和电量。
* `READ_PHONE_STATE`	允许游戏以只读的方式访问手机设备的信息，通过获取的信息来定位唯一的玩家。
* `ACCESS_WIFI_STATE`	获取设备的MAC地址。
* `WRITE_EXTERNAL_STORAGE`	用于保存设备信息，以及记录日志。
* `ACCESS_FINE_LOCATION（可选）`	用来获取该游戏被使用的精确位置信息
* `ACCESS_COARSE_LOCATION（可选）`	用来获取该应用被使用的粗略位置信息。

示例代码    

	<!--?xml version="1.0" encoding="utf-8"?-->
	<manifest ......>
    	<uses-permission android:name="android.permission.INTERNET" />
      	<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
      	<uses-permission android:name="android.permission.READ_PHONE_STATE"  />
      	<uses-permission android:name="android.permission.ACCESS_WIFI_STATE"  />
		<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"  />
		<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"  />
	  	<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"  />
 
      <application ......>
          <activity ......>
              ......
              </activity>
      </application>
	</manifest>

在应用程序的 `Application` 类的 `onCreate ` 方法中调用 `TalkingDataGA.init`    
_注：无 `Application ` 类请自行添加_  

	public class MainApplication extends Application{
	    @Override 
	    public void onCreate() {
	    	super.onCreate();
			TalkingDataGA.init(this, "您的 App ID", "渠道 ID");
   	 	}
	}  
成功调用此方法会输出调用日志，例如：

	03-07 14:11:52.643 8719-8719/org.cocos2dx.TalkingDataDemo I/TDGA: TalkingData Game Analytics Android_Native_SDK SDK Init: 
							SDK version is3.2.3
							App ID is :DEE6B9F5187FE0CB38009B6AC50A7DBC
							Channel is :TalkingData
	03-07 14:11:52.814 8719-8719/org.cocos2dx.TalkingDataDemo I/TDGA: TalkingDataGA.init()# TDGA Initialized Completed.     
`SDK` 初始化成功后调能看到数据上报日志，例如：  

	03-07 14:11:57.089 8719-8764/org.cocos2dx.TalkingDataDemo I/TDGA: Send data to server.
	03-07 14:11:57.189 8719-8764/org.cocos2dx.TalkingDataDemo I/TDGA: Send success : true  
附注：    
1. App ID: 在TalkingData Game Analytics创建应用后会得到App ID。 
		<img src="images/Image004.png" width="600px"/>  
2. 渠道 ID: 是渠道标识符，可通过不同渠道单独追踪数据。   
#### <a name="SDKAPI"></a>SDK API　　　　　　
###### 统计玩家帐户:  (TDGAAccount类)
``` bash
// 返回用户对象
static TDCCAccount* setAccount(const char* accountId);
// 设置帐户的显性名
void setAccountName(const char* accountName);
// 设置帐户类型
void setAccountType(TDCCAccountType accountType);
// 设置级别
void setLevel(int level);
// 设置性别
void setGender(TDCCGender gender);
// 设置年龄
void setAge(int age);
// 设置区服
void setGameServer(const char* gameServer);

```
##### 跟踪玩家充值:  (TDGAVirtualCurrency)
``` bash
// 充值请求
static void onChargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType);
// 充值成功
static void onChargeSuccess(const char* orderId);
// 虚拟币赠送
static void onReward(double currencyAmount, const char* reason);
```

##### 任务、关卡或副本: (TDGAMission)
``` bash
// 开始一项任务
static void onBegin(const char* missionId);
// 完成一项任务
static void onCompleted(const char* missionId);
// 一项任务失败
static void onFailed(const char* missionId, const char* failedCause);

```
##### 自定义事件: (TalkingDataGA)
``` bash
// 自定义事件
static void onEvent(const char* eventId, EventParamMap* map = NULL);
```
##### 注:具体SDK API 的使用请看Demo.