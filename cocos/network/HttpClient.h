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

#ifndef __CCHTTPCLIENT_H__
#define __CCHTTPCLIENT_H__

#include <thread>
#include <condition_variable>
#include <map>
#include "base/CCVector.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpCookie.h"
#include "base/CCScheduler.h"

/**
 * @addtogroup network
 * @{
 */

NS_CC_BEGIN

class ThreadPool;
namespace network {

 /** Singleton that handles asynchronous http requests.
 *
 * Once the request completed, a callback will issued in main thread when it provided during make request.
 *
 * @lua NA
 */

class HttpContext;
class CC_DLL HttpClient
{
public:
    /**
    * The buffer size of _responseMessage
    */
    static const int RESPONSE_BUFFER_SIZE = 256;

    /**
     * Get instance of HttpClient.
     *
     * @return the instance of HttpClient.
     */
    static HttpClient *getInstance();

    /**
     * Release the instance of HttpClient.
     */
    static void destroyInstance();

    /**
     * Enable cookie support.
     *
     * @param cookieFile the filepath of cookie file.
     */
    void enableCookies(const char* cookieFile);

    /**
     * Get the cookie filename
     *
     * @return the cookie filename
     */
    const std::string& getCookieFilename();

    /**
     * Set root certificate path for SSL verification.
     *
     * @param caFile a full path of root certificate.if it is empty, SSL verification is disabled.
     */
    void setSSLVerification(const std::string& caFile);

    /**
     * Get the ssl CA filename
     *
     * @return the ssl CA filename
     */
    const std::string& getSSLVerification();

    /**
     * Add a get request to task queue
     *
     * @param request a HttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     */
    void send(HttpRequest* request);
    /**
    *  abort a request
    *
    * @param request a HttpRequest object, which includes url, response callback etc.
    */
    void abort(HttpRequest* request);

    /**
     * Set the timeout value for connecting.
     *
     * @param value the timeout value for connecting.
     * @deprecated Please use `HttpRequest.setTimeout` instead.
     */
    CC_DEPRECATED_ATTRIBUTE void setTimeoutForConnect(int value);

    /**
     * Get the timeout value for connecting.
     *
     * @return int the timeout value for connecting.
     * @deprecated Please use `HttpRequest.getTimeout` instead.
     */
    CC_DEPRECATED_ATTRIBUTE int getTimeoutForConnect();

    /**
     * Set the timeout value for reading.
     *
     * @param value the timeout value for reading.
     * @deprecated Please use `HttpRequest.setTimeout` instead.
     */
    CC_DEPRECATED_ATTRIBUTE void setTimeoutForRead(int value);

    /**
     * Get the timeout value for reading.
     *
     * @return int the timeout value for reading.
     * @deprecated Please use `HttpRequest.setTimeout` instead.
     */
    CC_DEPRECATED_ATTRIBUTE int getTimeoutForRead();

    HttpCookie* getCookie() const {return _cookie; }

    std::mutex& getCookieFileMutex() {return _cookieFileMutex;}

    std::mutex& getSSLCaFileMutex() {return _sslCaFileMutex;}
private:
    HttpClient();
    virtual ~HttpClient();
    bool init();

    void networkThread(HttpRequest* request);
    void processResponse(HttpResponse* response);
    void dispatchRequestCallback(HttpRequest* request);
    void dispatchHttpHeaderReceivedCallback(HttpRequest* request);

private:
    bool _isInited;

    int _timeoutForConnect;
    std::mutex _timeoutForConnectMutex;

    int _timeoutForRead;
    std::mutex _timeoutForReadMutex;

    std::string _cookieFilename;
    std::mutex _cookieFileMutex;

    std::string _sslCaFilename;
    std::mutex _sslCaFileMutex;

    HttpCookie* _cookie;

    ThreadPool* _threadPool;

    std::map<HttpRequest*, std::unique_ptr<HttpContext>> _request2HttpContextMap;
    std::mutex _request2HttpContextMapMutex;
};

} // namespace network

NS_CC_END

// end group
/// @}

#endif //__CCHTTPCLIENT_H__

