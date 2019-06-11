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
#include "network/HttpClient.h"

#include <queue>
#include <errno.h>

#import "network/HttpAsynConnection-apple.h"
#include "network/HttpCookie.h"
#include "platform/CCFileUtils.h"
#include "platform/CCApplication.h"
#include "base/CCThreadPool.h"

NS_CC_BEGIN

namespace network {

static HttpClient *_httpClient = nullptr; // pointer to singleton

class HttpContext {
public:
    HttpContext()
    : _httpURLConnection(nullptr),
    _response(nullptr){
    }

    ~HttpContext() {
        if(_httpURLConnection) {
            [_httpURLConnection cancelRequest];
        }
        if(_response) {
            _response->release();
        }
    }

    void setHttpUrlConnection(HttpAsynConnection* httpURLConnection) {
        if(_httpURLConnection) {
            [_httpURLConnection cancelRequest];
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
    HttpAsynConnection* _httpURLConnection;
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
    if (nullptr == _httpClient)
    {
        CCLOG("HttpClient singleton is nullptr");
        return;
    }

    CCLOG("HttpClient::destroyInstance begin");

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
    _cookieFileMutex.lock();
    if (cookieFile)
    {
        _cookieFilename = std::string(cookieFile);
        _cookieFilename = FileUtils::getInstance()->fullPathForFilename(_cookieFilename);
    }
    else
    {
        _cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
    _cookieFileMutex.unlock();

    if (nullptr == _cookie)
    {
        _cookie = new(std::nothrow)HttpCookie;
    }
    _cookie->setCookieFileName(_cookieFilename);
    _cookie->readFile();
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
    if (!_cookieFilename.empty() && nullptr != _cookie)
    {
        _cookie->writeFile();
        CC_SAFE_DELETE(_cookie);
    }
    CC_SAFE_DELETE(_threadPool);
    CCLOG("HttpClient destructor");
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

// Process Response
void HttpClient::processResponse(HttpResponse* response)
{
    if (_httpClient == nullptr)
    {
        return;
    }
    
    auto request = response->getHttpRequest();
    long responseCode = -1;
    std::string responseURL;
    NSString* requestType = nil;

    // Process the request -> get response packet
    switch (request->getRequestType())
    {
        case HttpRequest::Type::GET: // HTTP GET
            requestType = @"GET";
            break;

        case HttpRequest::Type::POST: // HTTP POST
            requestType = @"POST";
            break;

        case HttpRequest::Type::PUT:
            requestType = @"PUT";
            break;

        case HttpRequest::Type::DELETE:
            requestType = @"DELETE";
            break;

        default:
            CCASSERT(false, "CCHttpClient: unknown request type, only GET,POST,PUT or DELETE is supported");
            break;
    }

    std::string responseMessage;
    
    //create request with url
    NSString* urlstring = [NSString stringWithUTF8String:request->getUrl()];
    NSURL *url = [NSURL URLWithString:urlstring];
    
    NSMutableURLRequest *nsrequest = [NSMutableURLRequest requestWithURL:url
                                                             cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData
                                                         timeoutInterval:request->getTimeout()];
    
    //set request type
    [nsrequest setHTTPMethod:requestType];
    
    /* get custom header data (if set) */
    std::vector<std::string> headers=request->getHeaders();
    if(!headers.empty())
    {
        /* append custom headers one by one */
        for (auto& header : headers)
        {
            unsigned long i = header.find(':', 0);
            unsigned long length = header.size();
            std::string field = header.substr(0, i);
            std::string value = header.substr(i+1, length-i);
            NSString *headerField = [NSString stringWithUTF8String:field.c_str()];
            NSString *headerValue = [NSString stringWithUTF8String:value.c_str()];
            [nsrequest setValue:headerValue forHTTPHeaderField:headerField];
        }
    }
    
    //if request type is post or put,set header and data
    if([requestType  isEqual: @"POST"] || [requestType isEqual: @"PUT"])
    {
        char* requestDataBuffer = request->getRequestData();
        if (nullptr !=  requestDataBuffer && 0 != request->getRequestDataSize())
        {
            NSData *postData = [NSData dataWithBytes:requestDataBuffer length:request->getRequestDataSize()];
            [nsrequest setHTTPBody:postData];
        }
    }
    
    //read cookie properties from file and set cookie
    std::string cookieFilename = getCookieFilename();
    if(!cookieFilename.empty() && nullptr != getCookie())
    {
        const CookiesInfo* cookieInfo = getCookie()->getMatchCookie(request->getUrl());
        if(cookieInfo != nullptr)
        {
            NSString *domain = [NSString stringWithCString:cookieInfo->domain.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *path = [NSString stringWithCString:cookieInfo->path.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *value = [NSString stringWithCString:cookieInfo->value.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *name = [NSString stringWithCString:cookieInfo->name.c_str() encoding:[NSString defaultCStringEncoding]];
            
            // create the properties for a cookie
            NSDictionary *properties = [NSDictionary dictionaryWithObjectsAndKeys: name,NSHTTPCookieName,
                                        value, NSHTTPCookieValue, path, NSHTTPCookiePath,
                                        domain, NSHTTPCookieDomain,
                                        nil];
            
            // create the cookie from the properties
            NSHTTPCookie *cookie = [NSHTTPCookie cookieWithProperties:properties];
            
            // add the cookie to the cookie storage
            [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookie:cookie];
        }
    }
    
    HttpAsynConnection *httpAsynConn = [[HttpAsynConnection new] autorelease];
    httpAsynConn.srcURL = urlstring;
    httpAsynConn.sslFile = nil;
    
    std::string sslCaFileName = getSSLVerification();
    if(!sslCaFileName.empty())
    {
        long len = sslCaFileName.length();
        long pos = sslCaFileName.rfind('.', len-1);
        
        httpAsynConn.sslFile = [NSString stringWithUTF8String:sslCaFileName.substr(0, pos).c_str()];
    }
    [httpAsynConn startRequest:nsrequest];
    
    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter == _request2HttpContextMap.end()) {
            [httpAsynConn cancelRequest];
            return;
        }
        iter->second->setHttpUrlConnection(httpAsynConn);
    }
    
    while( httpAsynConn.finish != true)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    
    //if http connection return error
    if (httpAsynConn.connError != nil)
    {
        NSString* errorString = [httpAsynConn.connError localizedDescription];
        responseMessage = [errorString UTF8String];
        response->setResponseCode(responseCode);
        response->setSucceed(false);
        response->setErrorBuffer(responseMessage.c_str());
        
        {
            std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
            auto iter = _request2HttpContextMap.find(request);
            if(iter != _request2HttpContextMap.end()) {
                iter->second->setHttpUrlConnection(nullptr);
            }
        }
        
        return;
    }
    
    //if http response got error, just log the error
    if (httpAsynConn.responseError != nil)
    {
        NSString* errorString = [httpAsynConn.responseError localizedDescription];
        responseMessage = [errorString UTF8String];
    }
    
    responseCode = httpAsynConn.responseCode;
    responseURL = [httpAsynConn.responseURL UTF8String];
    //add cookie to cookies vector
    if(!cookieFilename.empty())
    {
        NSArray *cookies = [NSHTTPCookie cookiesWithResponseHeaderFields:httpAsynConn.responseHeader forURL:url];
        for (NSHTTPCookie *cookie in cookies)
        {
            //NSLog(@"Cookie: %@", cookie);
            NSString *domain = cookie.domain;
            //BOOL session = cookie.sessionOnly;
            NSString *path = cookie.path;
            BOOL secure = cookie.isSecure;
            NSDate *date = cookie.expiresDate;
            NSString *name = cookie.name;
            NSString *value = cookie.value;
            
            CookiesInfo cookieInfo;
            cookieInfo.domain = [domain cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.path = [path cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.secure = (secure == YES) ? true : false;
            cookieInfo.expires = [[NSString stringWithFormat:@"%ld", (long)[date timeIntervalSince1970]] cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.name = [name cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.value = [value cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.tailmatch = true;
            
            getCookie()->updateOrAddCookie(&cookieInfo);
        }
    }
    
    //handle response header
    NSMutableString *header = [NSMutableString string];
    [header appendFormat:@"HTTP/1.1 %ld %@\n", (long)httpAsynConn.responseCode, httpAsynConn.statusString];
    for (id key in httpAsynConn.responseHeader)
    {
        [header appendFormat:@"%@: %@\n", key, [httpAsynConn.responseHeader objectForKey:key]];
    }
    if (header.length > 0)
    {
        NSRange range = NSMakeRange(header.length-1, 1);
        [header deleteCharactersInRange:range];
    }
    NSData *headerData = [header dataUsingEncoding:NSUTF8StringEncoding];
    std::vector<char> *headerBuffer = (std::vector<char>*)response->getResponseHeader();
    const void* headerptr = [headerData bytes];
    long headerlen = [headerData length];
    headerBuffer->insert(headerBuffer->end(), (char*)headerptr, (char*)headerptr+headerlen);
    
    //handle response data
    std::vector<char> *recvBuffer = (std::vector<char>*)response->getResponseData();
    const void* ptr = [httpAsynConn.responseData bytes];
    long len = [httpAsynConn.responseData length];
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+len);
    
    {
        std::lock_guard<std::mutex> lock(_request2HttpContextMapMutex);
        auto iter = _request2HttpContextMap.find(request);
        if(iter != _request2HttpContextMap.end()) {
            iter->second->setHttpUrlConnection(nullptr);
        }
    }
    
    // write data to HttpResponse
    response->setResponseCode(responseCode);
    response->setResponseURL(responseURL);
    response->setSucceed(true);
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
