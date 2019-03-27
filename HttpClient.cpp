#include "HttpClient.h"

#include <curl/curl.h>
#include <exception>
#include <thread>

namespace {

size_t writeDataCallback(void* contents, size_t size, size_t nmemb,
                         std::string* s) {
  try {
    s->append((char*)contents, nmemb);
  } catch (std::bad_alloc& e) {
    // handle memory problem
    return 0;
  }
  return nmemb;
}

int progressCallback(void* cancellationToken, curl_off_t dltotal,
                     curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
  return 0;
}

}  // namespace

namespace lightclient {

void HttpClient::execute(const HttpRequest& request,
                         const HttpResponseCallback& callback) {
  // todo : retries, should reuse that handle, and just need to call the ease
  // perform
  std::thread([=]() {
    HttpResponse response;
    response.status_code = -1;
    CURL* handle = curl_easy_init();
    if (handle) {
      curl_easy_setopt(handle, CURLOPT_URL, request.url.c_str());
      curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeDataCallback);
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response.content);
      curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
      curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, progressCallback);
      // curl_easy_setopt(handle, CURLOPT_XFERINFODATA, &cancellationToken);
      CURLcode result = curl_easy_perform(handle);
      if (result != CURLE_OK) {
        response.content = curl_easy_strerror(result);
      } else {
        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE,
                          &response.status_code);
      }

      curl_easy_cleanup(handle);
      callback(response);
    }
  })
      .detach();
}

}  // namespace lightclient