
package com.talkingdata.cocos2dx;

import com.tendcloud.tenddata.TalkingDataGA;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.util.Log;

public class TalkingDataCocos2dx extends Cocos2dxActivity {

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        com.igexin.sdk.PushManager.getInstance().initialize(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d("TalkingDataCocos2dx", "onResume");
        TalkingDataGA.onResume(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d("TalkingDataCocos2dx", "onPause");
        TalkingDataGA.onPause(this);
    }

}
