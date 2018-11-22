// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#ifdef _WIN32
struct IUnknown;

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT

#endif // _WIN32

// #define CPPREST_FORCE_HTTP_LISTENER_ASIO

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/oauth2.h>
#include <memory>
#include "Strings.h"

namespace http = web::http;
namespace oauth = http::oauth2::experimental;
namespace json = web::json;
namespace client = http::client;
namespace listener = http::experimental::listener;

template <typename T>
using SharedPtrResponse_t = pplx::task<std::shared_ptr<T>>;
template <typename T>
using UniquePtrResponse_t = pplx::task<std::unique_ptr<T>>;

using JsonResponse_t = pplx::task<json::value>;
using FBID_t = unsigned long long;
