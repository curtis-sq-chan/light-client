#pragma once

#include <functional>
#include <map>
#include <string>

namespace lightclient {

struct HttpResponse {
  long status_code;
  std::string content;
};

struct HttpRequest {
  std::string url;
  std::map<std::string, std::string> header_params;
  std::map<std::string, std::string> query_params;
};

using HttpResponseCallback = std::function<void(const HttpResponse&)>;

class HttpClient {
 public:
  HttpClient() = default;
  virtual ~HttpClient() = default;

  // todo : cancellation
  virtual void execute(const HttpRequest& request,
                       const HttpResponseCallback& callback);
};

}  // namespace lightclient