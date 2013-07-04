#include "TDGAJniHelper.h"
#include "TDGALog.h"
#include <string.h>

#define JAVAVM    TDGAJniHelper::getJavaVM()

using namespace std;

extern "C"
{

    //////////////////////////////////////////////////////////////////////////
    // java vm helper function
    //////////////////////////////////////////////////////////////////////////

    static bool getEnv(JNIEnv **env)
    {

        bool bRet = false;

        do 
        {
        	if (JAVAVM == NULL) {
        		LOGE("!!!!!! Not set javavm. Please Call TDGAJniHelper::setJavaVM() in JNI_OnLoad.");
        		break;
        	}
            if (JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
            {
                LOGD("Failed to get the environment using GetEnv()");
                break;
            }

            if (JAVAVM->AttachCurrentThread(env, 0) < 0)
            {
                LOGD("Failed to get the environment using AttachCurrentThread()");
                break;
            }
            bRet = true;
        } while (0);        

        return bRet;
    }

    static jclass getClassID_(const char *className, JNIEnv *env)
    {
        JNIEnv *pEnv = env;
        jclass ret = 0;

        do 
        {
            if (! pEnv)
            {
                if (! getEnv(&pEnv))
                {
                    break;
                }
            }
            
            ret = pEnv->FindClass(className);
            if (! ret)
            {
                 LOGD("Failed to find class of %s", className);
                break;
            }
        } while (0);

        return ret;
    }

    static bool getStaticMethodInfo_(TDGAJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;
        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);
            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find static method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;
            bRet = true;
        } while (0);

        return bRet;
    }

    static bool getMethodInfo_(TDGAJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);

            methodID = pEnv->GetMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static string jstring2string_(jstring jstr)
    {
        if (jstr == NULL)
        {
            return "";
        }
        
        JNIEnv *env = 0;

        if (! getEnv(&env))
        {
            return 0;
        }

        const char* chars = env->GetStringUTFChars(jstr, NULL);
        string ret(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        return ret;
    }
}


JavaVM* TDGAJniHelper::m_psJavaVM = NULL;

bool TDGAJniHelper::getJNIEnv(JNIEnv **env) {
	return getEnv(env);
}

JavaVM* TDGAJniHelper::getJavaVM()
{
    return m_psJavaVM;
}

void TDGAJniHelper::setJavaVM(JavaVM *javaVM)
{
    m_psJavaVM = javaVM;
}

string TDGAJniHelper::m_externalAssetPath;

const char* TDGAJniHelper::getExternalAssetPath() {
    return m_externalAssetPath.c_str();
}

void TDGAJniHelper::setExternalAssetPath(const char * externalAssetPath) {
    m_externalAssetPath = externalAssetPath;
}

jclass TDGAJniHelper::getClassID(const char *className, JNIEnv *env)
{
    return getClassID_(className, env);
}

bool TDGAJniHelper::getStaticMethodInfo(TDGAJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getStaticMethodInfo_(methodinfo, className, methodName, paramCode);
}

bool TDGAJniHelper::getMethodInfo(TDGAJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getMethodInfo_(methodinfo, className, methodName, paramCode);
}

string TDGAJniHelper::jstring2string(jstring str)
{
    return jstring2string_(str);
}

