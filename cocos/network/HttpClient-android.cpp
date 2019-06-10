/****************************************************************************
 Copyright (c) 2012      greathqy
 Copyright (c) 2012      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "network/HttpClient.h"

#include <queue>
#include <sstream>
#include <stdio.h>
#include <errno.h>

#include "platform/CCApplication.h"
#include "platform/CCFileUtils.h"
#include "platform/android/jni/JniHelper.h"

#include "base/ccUTF8.h"
#include "base/CCThreadPool.h"

#ifndef JCLS_HTTPCLIENT
#define JCLS_HTTPCLIENT  "org/cocos2dx/lib/Cocos2dxHttpURLConnection"
#endif

NS_CC_BEGIN

namespace network {
    
typedef std::vector<std::string> HttpRequestHeaders;
typedef HttpRequestHeaders::iterator HttpRequestHeadersIter;
typedef std::vector<std::string> HttpCookies;
typedef HttpCookies::iterator HttpCookiesIter;

static HttpClient* _httpClient = nullptr; // pointer to singleton
    

struct CookiesInfo
{
    std::string domain;
    bool tailmatch;
    std::string path;
    bool secure;
    std::string key;
    std::string value;
    std::string expires;
};

//static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
static size_t writeData(void* buffer, size_t sizes, HttpResponse* response)
{
    std::vector<char> * recvBuffer = (std::vector<char>*)response->getResponseData();
    recvBuffer->clear();
    recvBuffer->insert(recvBuffer->end(), (char*)buffer, ((char*)buffer) + sizes);
    return sizes;
} 

//static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
size_t writeHeaderData(void* buffer, size_t sizes,HttpResponse* response)
{
    std::vector<char> * recvBuffer = (std::vector<char>*) response->getResponseHeader();
    recvBuffer->clear();
    recvBuffer->insert(recvBuffer->end(), (char*)buffer, (char*)buffer + sizes);
    return sizes;
}

class HttpURLConnection
{
public:
    HttpURLConnection(HttpClient* httpClient)
    :_client(httpClient)
    ,_httpURLConnection(nullptr)
    ,_requestmethod("")
    ,_responseCookies("")
    ,_cookieFileName("")
    ,_contentLength(0)
    {

    }

    ~HttpURLConnection()
    {
        if(_httpURLConnection != nullptr)
        {
            JniHelper::getEnv()->DeleteGlobalRef(_httpURLConnection);
        }
    }
    
    void setRequestMethod(const char* method)
    {
        _requestmethod = method;

        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "setRequestMethod",
                                           "(Ljava/net/HttpURLConnection;Ljava/lang/String;)V"))
        {
            jstring jstr = methodInfo.env->NewStringUTF(_requestmethod.c_str());
            methodInfo.env->CallStaticVoidMethod(
                                                 methodInfo.classID, methodInfo.methodID, _httpURLConnection, jstr);
            methodInfo.env->DeleteLocalRef(jstr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }
    
    bool init(HttpRequest* request)
    {
        createHttpURLConnection(request->getUrl());
        if(!configure(request))
        {
            return false;
        }
        /* get custom header data (if set) */
        HttpRequestHeaders headers=request->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (auto& header : headers)
            {
                int len = header.length();
                int pos = header.find(':');
                if (-1 == pos || pos >= len)
                {
                    continue;
                }
                std::string str1 = header.substr(0, pos);
                std::string str2 = header.substr(pos + 1, len - pos - 1);
                addRequestHeader(str1.c_str(), str2.c_str());
            }
        }
        
        addCookiesForRequestHeader();
        
        return true;
    }
    
    int connect()
    {
        int suc = 0;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "connect",
                                           "(Ljava/net/HttpURLConnection;)I"))
        {
            suc = methodInfo.env->CallStaticIntMethod(
                                                      methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return suc;
    }
    
    void disconnect()
    {
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "disconnect",
                                           "(Ljava/net/HttpURLConnection;)V"))
        {
            methodInfo.env->CallStaticVoidMethod(
                                                 methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }
    
    int getResponseCode()
    {
        int responseCode = 0;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseCode",
                                           "(Ljava/net/HttpURLConnection;)I"))
        {
            responseCode = methodInfo.env->CallStaticIntMethod(
                                                           methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return responseCode;
    }
    
    char* getResponseMessage()
    {
        char* message = nullptr;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseMessage",
                                           "(Ljava/net/HttpURLConnection;)Ljava/lang/String;"))
        {
            jobject jObj = methodInfo.env->CallStaticObjectMethod(
                                                                  methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            message = getBufferFromJString((jstring)jObj, methodInfo.env);
            if (nullptr != jObj)
            {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return message;
    }
    
    void sendRequest(HttpRequest* request)
    {
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "sendRequest",
                                           "(Ljava/net/HttpURLConnection;[B)V"))
        {
            
            jbyteArray bytearray;
            ssize_t dataSize = request->getRequestDataSize();
            bytearray = methodInfo.env->NewByteArray(dataSize);
            methodInfo.env->SetByteArrayRegion(bytearray, 0, dataSize, (const jbyte*)request->getRequestData());
            methodInfo.env->CallStaticVoidMethod(
                                                 methodInfo.classID, methodInfo.methodID, _httpURLConnection, bytearray);
            methodInfo.env->DeleteLocalRef(bytearray);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }
    
    size_t saveResponseCookies(const char* responseCookies, size_t count)
    {
        if (nullptr == responseCookies || strlen(responseCookies) == 0 || count == 0)
        {
            return 0;
        }
        
        if (_cookieFileName.empty())
        {
            _cookieFileName = FileUtils::getInstance()->getWritablePath() + "cookieFile.txt";
        }
        
        FILE* fp = fopen(_cookieFileName.c_str(), "w");
        if (nullptr == fp)
        {
            CCLOG("can't create or open response cookie files");
            return 0;
        }
        
        fwrite(responseCookies, sizeof(char), count, fp);
        
        fclose(fp);
        
        return count;
    }
    
    char* getResponseHeaders()
    {
        char* headers = nullptr;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseHeaders",
                                           "(Ljava/net/HttpURLConnection;)Ljava/lang/String;"))
        {
            jobject jObj = methodInfo.env->CallStaticObjectMethod(
                                                                  methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            headers = getBufferFromJString((jstring)jObj, methodInfo.env);
            if (nullptr != jObj) {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return headers;
        
    }
    
    char* getResponseContent(HttpResponse* response)
    {
        if (nullptr == response)
        {
            return nullptr;
        }
        
        char* content = nullptr;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseContent",
                                           "(Ljava/net/HttpURLConnection;)[B"))
        {
            jobject jObj = methodInfo.env->CallStaticObjectMethod(
                                                                  methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            
            _contentLength = getCStrFromJByteArray((jbyteArray)jObj, methodInfo.env, &content);
            if (nullptr != jObj) 
            {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return content;
    }
    
    char* getResponseHeaderByKey(const char* key)
    {
        char* value = nullptr;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseHeaderByKey",
                                           "(Ljava/net/HttpURLConnection;Ljava/lang/String;)Ljava/lang/String;"))
        {
            jstring jstrKey = methodInfo.env->NewStringUTF(key);
            jobject jObj = methodInfo.env->CallStaticObjectMethod(
                                                                  methodInfo.classID, methodInfo.methodID, _httpURLConnection, jstrKey);
            value = getBufferFromJString((jstring)jObj, methodInfo.env);
            methodInfo.env->DeleteLocalRef(jstrKey);
            if (nullptr != jObj) {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return value;
    }

    std::string getResponseURL()
    {
        char* value = nullptr;
        std::string responseURL;

        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseURL",
                                           "(Ljava/net/HttpURLConnection;)Ljava/lang/String;"))
        {
            jobject jObj = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, _httpURLConnection);
            value = getBufferFromJString((jstring)jObj, methodInfo.env);
            if (nullptr != jObj) {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        } else {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }

        responseURL = value;
        free(value);
        return responseURL;
    }
    
    int getResponseHeaderByKeyInt(const char* key)
    {
        int contentLength = 0;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseHeaderByKeyInt",
                                           "(Ljava/net/HttpURLConnection;Ljava/lang/String;)I"))
        {
            jstring jstrKey = methodInfo.env->NewStringUTF(key);
            contentLength = methodInfo.env->CallStaticIntMethod(
                                                                methodInfo.classID, methodInfo.methodID, _httpURLConnection, jstrKey);
            methodInfo.env->DeleteLocalRef(jstrKey);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return contentLength;
    }
    
    char* getResponseHeaderByIdx(int idx)
    {
        char* header = nullptr;
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
                                           JCLS_HTTPCLIENT,
                                           "getResponseHeaderByIdx",
                                           "(Ljava/net/HttpURLConnection;I)Ljava/lang/String;"))
        {
            jobject jObj = methodInfo.env->CallStaticObjectMethod(
                                                                  methodInfo.classID, methodInfo.methodID, _httpURLConnection, idx);
            header = getBufferFromJString((jstring)jObj, methodInfo.env);
            if (nullptr != jObj) {
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
        
        return header;
    }
    
    const std::string& getCookieFileName() const
    {
        return _cookieFileName;
    }
    
    void setCookieFileName(std::string& filename)
    {
        _cookieFileName = filename;
    }
    
    int getContentLength()
    {
        return _contentLength;
    }
    
private:
    void createHttpURLConnection(const std::string& url)
    {
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
            JCLS_HTTPCLIENT,
            "createHttpURLConnection",
            "(Ljava/lang/String;)Ljava/net/HttpURLConnection;"))
        {
            _url = url;
            jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
            jobject jObj = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, jurl);
            if (jObj != nullptr)
            {
                _httpURLConnection = methodInfo.env->NewGlobalRef(jObj);
                methodInfo.env->DeleteLocalRef(jObj);
            }
            methodInfo.env->DeleteLocalRef(jurl);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }

    void addRequestHeader(const char* key, const char* value) 
    {
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
            JCLS_HTTPCLIENT,
            "addRequestHeader",
            "(Ljava/net/HttpURLConnection;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring jstrKey = methodInfo.env->NewStringUTF(key);
            jstring jstrVal = methodInfo.env->NewStringUTF(value);
            methodInfo.env->CallStaticVoidMethod(
                methodInfo.classID, methodInfo.methodID, _httpURLConnection, jstrKey, jstrVal);
            methodInfo.env->DeleteLocalRef(jstrKey);
            methodInfo.env->DeleteLocalRef(jstrVal);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }
    
    void addCookiesForRequestHeader()
    {
        if(_client->getCookieFilename().empty())
        {
            return;
        }
        
        _cookieFileName = FileUtils::getInstance()->fullPathForFilename(_client->getCookieFilename());
        
        std::string cookiesInfo = FileUtils::getInstance()->getStringFromFile(_cookieFileName);
        
        if (cookiesInfo.empty())
            return;
        
        HttpCookies cookiesVec;
        cookiesVec.clear();
        
        std::stringstream  stream(cookiesInfo);
        std::string item;
        while (std::getline(stream, item, '\n'))
        {
            cookiesVec.push_back(item);
        }
        
        if (cookiesVec.empty())
            return;
        
        std::vector<CookiesInfo> cookiesInfoVec;
        cookiesInfoVec.clear();

        for (auto& cookies : cookiesVec)
        {
            if (cookies.find("#HttpOnly_") != std::string::npos)
            {
                cookies = cookies.substr(10);
            }
            
            if(cookies.at(0) == '#')
                continue;
            
            CookiesInfo co;
            std::stringstream streamInfo(cookies);
            std::string item;
            std::vector<std::string> elems;
            
            while (std::getline(streamInfo, item, '\t'))
            {
                elems.push_back(item);
            }
            
            co.domain = elems[0];
            if (co.domain.at(0) == '.')
            {
                co.domain = co.domain.substr(1);
            }
            co.tailmatch = strcmp("TRUE", elems.at(1).c_str())?true: false;
            co.path   = elems.at(2);
            co.secure = strcmp("TRUE", elems.at(3).c_str())?true: false;
            co.expires = elems.at(4);
            co.key = elems.at(5);
            co.value = elems.at(6);
            cookiesInfoVec.push_back(co);
        }

        std::string sendCookiesInfo = "";
        int cookiesCount = 0;
        for (auto& cookieInfo : cookiesInfoVec)
        {
            if (_url.find(cookieInfo.domain) != std::string::npos)
            {
                std::string keyValue = cookieInfo.key;
                keyValue.append("=");
                keyValue.append(cookieInfo.value);
                if (cookiesCount != 0)
                    sendCookiesInfo.append(";");
                
                sendCookiesInfo.append(keyValue);
            }
            cookiesCount++;
        }

        //set Cookie
        addRequestHeader("Cookie",sendCookiesInfo.c_str());
    }

    void setReadAndConnectTimeout(int readMiliseconds, int connectMiliseconds)
    {
        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
            JCLS_HTTPCLIENT,
            "setReadAndConnectTimeout",
            "(Ljava/net/HttpURLConnection;II)V"))
        {
            methodInfo.env->CallStaticVoidMethod(
                methodInfo.classID, methodInfo.methodID, _httpURLConnection, readMiliseconds, connectMiliseconds);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }

    void setVerifySSL()
    {
        if(_client->getSSLVerification().empty())
            return;

        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(_client->getSSLVerification());

        JniMethodInfo methodInfo;
        if (JniHelper::getStaticMethodInfo(methodInfo,
            JCLS_HTTPCLIENT,
            "setVerifySSL",
            "(Ljava/net/HttpURLConnection;Ljava/lang/String;)V"))
        {
            jstring jstrfullpath = methodInfo.env->NewStringUTF(fullpath.c_str());
            methodInfo.env->CallStaticVoidMethod(
                methodInfo.classID, methodInfo.methodID, _httpURLConnection, jstrfullpath);
            methodInfo.env->DeleteLocalRef(jstrfullpath);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
        else
        {
            CCLOGERROR("HttpClient::%s failed!", __FUNCTION__);
        }
    }

    bool configure(HttpRequest* request)
    {   
        if(nullptr == _httpURLConnection)
        {
            return false;
        }

        if(nullptr == _client)
        {
            return false;
        }

        setReadAndConnectTimeout(request->getTimeout() * 1000, request->getTimeout() * 1000);

        setVerifySSL();

        return true;
    }

    char* getBufferFromJString(jstring jstr, JNIEnv* env)
    {
        if (nullptr == jstr)
        {
            return nullptr;
        }
        std::string strValue = cocos2d::StringUtils::getStringUTFCharsJNI(env, jstr);
        return strdup(strValue.c_str());
    }

    int getCStrFromJByteArray(jbyteArray jba, JNIEnv* env, char** ppData)
    {
        if (nullptr == jba)
        {
            *ppData = nullptr;
            return 0;
        }

        int len = env->GetArrayLength(jba);
        char* str = (char*)malloc(sizeof(char)*len);
        env->GetByteArrayRegion(jba, 0, len, (jbyte*)str);

        *ppData = str;
        return len;
    }

    const std::string& getCookieString() const
    {
        return _responseCookies;
    }
private:
    HttpClient* _client;
    jobject _httpURLConnection;
    std::string _requestmethod;
    std::string _responseCookies;
    std::string _cookieFileName;
    std::string _url;
    int _contentLength;
};

class HttpContext {
public:
    HttpContext()
    : _httpURLConnection(nullptr)
    ,_response(nullptr){
    }

    ~HttpContext() {
        if(_httpURLConnection) {
            _httpURLConnection->disconnect();
        }
        if(_response) {
            _response->release();
        }
    }

    void setHttpUrlConnection(HttpURLConnection* httpURLConnection) {
        if(_httpURLConnection) {
            _httpURLConnection->disconnect();
        }
        _httpURLConnection = httpURLConnection;
    }

    void setHttpResponse(HttpResponse* httpResponse) {
        assert(_response == nullptr);
        _response = httpResponse;
    }

    HttpResponse* getHttpResponse() {
        return _response;
    }

private:
    HttpURLConnection* _httpURLConnection;
    HttpResponse* _response;
};

// HttpClient implementation
HttpClient* HttpClient::getInstance()
{
    if (_httpClient == nullptr) 
    {
        _httpClient = new (std::nothrow) HttpClient();
    }
    
    return _httpClient;
}

void HttpClient::destroyInstance()
{
    if (_httpClient == nullptr)
    {
        CCLOG("HttpClient singleton is nullptr");
        return;
    }

    CCLOG("HttpClient::destroyInstance ...");

    auto thiz = _httpClient;
    _httpClient = nullptr;

    {
        std::lock_guard<std::mutex> lock(thiz->_request2HttpContextMapMutex);
        for(auto& iter : thiz->_request2HttpContextMap) {
            iter.first->release();
        }
        thiz->_request2HttpContextMap.clear();
    }

    Scheduler* scheduler = Application::getInstance()->getScheduler();
    if (scheduler != nullptr) {
        scheduler->unscheduleAllForTarget(thiz);
    }

    CC_SAFE_DELETE(thiz);

    CCLOG("HttpClient::destroyInstance() finished!");
}

void HttpClient::enableCookies(const char* cookieFile) 
{
    std::lock_guard<std::mutex> lock(_cookieFileMutex);
    if (cookieFile)
    {
        _cookieFilename = std::string(cookieFile);
    }
    else
    {
        _cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
}
    
void HttpClient::setSSLVerification(const std::string& caFile)
{
    std::lock_guard<std::mutex> lock(_sslCaFileMutex);
    _sslCaFilename = caFile;
}

HttpClient::HttpClient()
: _isInited(false)
, _timeoutForConnect(30)
, _timeoutForRead(60)
, _cookie(nullptr)
{
    CCLOG("In the constructor of HttpClient!");
    _threadPool = ThreadPool::newCachedThreadPool(1, 10, 5, 2, 2);
}

HttpClient::~HttpClient()
{
    CCLOG("In the destructor of HttpClient!");
    CC_SAFE_DELETE(_threadPool);
}

void HttpClient::send(HttpRequest* request)
{
    if(nullptr == request) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        if(_request2HttpContextMap.find(request) != _request2HttpContextMap.end()) {
            return;
        }
        request->retain();
        HttpContext* context = new HttpContext();
        _request2HttpContextMap.emplace(std::make_pair(request, context));
    }

    _threadPool->pushTask([this, request](int tid) {
        networkThread(request);
    });
}

void HttpClient::abort(HttpRequest *request)
{
    if(nullptr == request) {
        return;
    }
    std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
    auto iter = _request2HttpContextMap.find(request);
    if(iter == _request2HttpContextMap.end()) {
        return;
    }
    iter->first->release();
    _request2HttpContextMap.erase(iter);
}

// Worker thread
void HttpClient::networkThread(HttpRequest* request)
{
    HttpResponse* response;
    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        if(_request2HttpContextMap.find(request) == _request2HttpContextMap.end()) {
            return;
        }
        response = new HttpResponse(request);
    }

    processResponse(response);

    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter == _request2HttpContextMap.end()) {
            response->release();
            return;
        }
        iter->second->setHttpResponse(response);
    }

    Scheduler* scheduler = Application::getInstance()->getScheduler();
    if (scheduler != nullptr) {
        scheduler->performFunctionInCocosThread(
               CC_CALLBACK_0(HttpClient::dispatchRequestCallback, this, request));
    }
}

void HttpClient::dispatchRequestCallback(HttpRequest* request)
{
    HttpContext* context;
    const ccHttpRequestCallback& callback = request->getResponseCallback();

    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter == _request2HttpContextMap.end()) {
            return;
        }
        context = iter->second.release();
        iter->first->release();
        _request2HttpContextMap.erase(iter);
    }

    HttpResponse* response = context->getHttpResponse();
    if(response && callback != nullptr) {
        callback(this, response);
    }

    CC_SAFE_DELETE(context);
}

// Process Response
void HttpClient::processResponse(HttpResponse* response)
{
    if(_httpClient == nullptr) {
        return;
    }

    auto request = response->getHttpRequest();
    HttpRequest::Type requestType = request->getRequestType();

    if (HttpRequest::Type::GET != requestType &&
        HttpRequest::Type::POST != requestType &&
        HttpRequest::Type::PUT != requestType &&
        HttpRequest::Type::DELETE != requestType)
    {
        CCASSERT(true, "CCHttpClient: unknown request type, only GET、POST、PUT、DELETE are supported");
        return;
    }

    long responseCode = -1;
    int  retValue = 0;
    std::string responseURL;

    HttpURLConnection urlConnection(this);
    if(!urlConnection.init(request)) {
        response->setSucceed(false);
        response->setErrorBuffer("HttpURLConnetcion init failed");
        return;
    }

    switch (requestType) {
        case HttpRequest::Type::GET:
            urlConnection.setRequestMethod("GET");
            break;

        case HttpRequest::Type::POST:
            urlConnection.setRequestMethod("POST");
            break;

        case HttpRequest::Type::PUT:
            urlConnection.setRequestMethod("PUT");
            break;

        case HttpRequest::Type::DELETE:
            urlConnection.setRequestMethod("DELETE");
            break;
        default:
            break;
    }

    int suc = urlConnection.connect();
    if (0 != suc) {
        response->setSucceed(false);
        response->setErrorBuffer("connect failed");
        response->setResponseCode(responseCode);
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter == _request2HttpContextMap.end()) {
            urlConnection.disconnect();
            return;
        }
        iter->second->setHttpUrlConnection(&urlConnection);
    }

    if (HttpRequest::Type::POST == requestType ||
        HttpRequest::Type::PUT == requestType) {
        urlConnection.sendRequest(request);
    }

    responseCode = urlConnection.getResponseCode();

    if (0 == responseCode) {
        response->setSucceed(false);
        response->setErrorBuffer("connect failed");
        response->setResponseCode(-1);
        return;
    }

    responseURL = urlConnection.getResponseURL();
    response->setResponseURL(responseURL);

    char* headers = urlConnection.getResponseHeaders();
    if (nullptr != headers) {
        writeHeaderData(headers, strlen(headers), response);
        free(headers);
    }

    //get and save cookies
    char* cookiesInfo = urlConnection.getResponseHeaderByKey("set-cookie");
    if (nullptr != cookiesInfo) {
        urlConnection.saveResponseCookies(cookiesInfo, strlen(cookiesInfo));
        free(cookiesInfo);
    }

    //content len
    int contentLength = urlConnection.getResponseHeaderByKeyInt("Content-Length");
    char* contentInfo = urlConnection.getResponseContent(response);
    if (nullptr != contentInfo) {
        std::vector<char> * recvBuffer = (std::vector<char>*)response->getResponseData();
        recvBuffer->clear();
        recvBuffer->insert(recvBuffer->begin(), (char*)contentInfo, ((char*)contentInfo) + urlConnection.getContentLength());
        free(contentInfo);
    }
    std::string responseMessage;
    char *messageInfo = urlConnection.getResponseMessage();
    if (messageInfo) {
        responseMessage = messageInfo;
        free(messageInfo);
    }

    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter != _request2HttpContextMap.end()) {
            iter->second->setHttpUrlConnection(nullptr);
        }
    }

    // write data to HttpResponse
    response->setResponseCode(responseCode);

    if (responseCode == -1) {
        response->setSucceed(false);
        if (!responseMessage.empty()) {
            response->setErrorBuffer(responseMessage.c_str());
        } else {
           response->setErrorBuffer("response code error!");
       }
    } else {
        response->setSucceed(true);
    }
}

void HttpClient::setTimeoutForConnect(int value)
{
    std::lock_guard<std::mutex> lock(_timeoutForConnectMutex);
    _timeoutForConnect = value;
}
    
int HttpClient::getTimeoutForConnect()
{
    std::lock_guard<std::mutex> lock(_timeoutForConnectMutex);
    return _timeoutForConnect;
}
    
void HttpClient::setTimeoutForRead(int value)
{
    std::lock_guard<std::mutex> lock(_timeoutForReadMutex);
    _timeoutForRead = value;
}
    
int HttpClient::getTimeoutForRead()
{
    std::lock_guard<std::mutex> lock(_timeoutForReadMutex);
    return _timeoutForRead;
}
    
const std::string& HttpClient::getCookieFilename()
{
    std::lock_guard<std::mutex> lock(_cookieFileMutex);
    return _cookieFilename;
}
    
const std::string& HttpClient::getSSLVerification()
{
    std::lock_guard<std::mutex> lock(_sslCaFileMutex);
    return _sslCaFilename;
}

}

NS_CC_END

#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
