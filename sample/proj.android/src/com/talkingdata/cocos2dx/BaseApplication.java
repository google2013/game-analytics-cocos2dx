package com.talkingdata.cocos2dx;

import android.app.Application;

import com.mrocker.push.PushManager;
import com.tendcloud.tenddata.TalkingDataGA;

public class BaseApplication extends Application {
	@Override
	public void onCreate() {
		super.onCreate();
		PushManager.startPushService(getApplicationContext());
		TalkingDataGA.sPlatformType = TalkingDataGA.PLATFORM_TYPE_COCOS2DX;
		TalkingDataGA.init(this, "CB18160F9499FF79B30C73663A384A62", "360");
	}
	static {
        System.loadLibrary("talkingdata");
    }
}
