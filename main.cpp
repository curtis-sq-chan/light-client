#include "HttpClient.h"

#include <future>
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
  lightclient::HttpRequest request;
  request.url = "https://www.google.com/";
  auto promise = std::make_shared<std::promise<void>>();
  lightclient::HttpClient client;
  client.execute(request, [=](const lightclient::HttpResponse& response) {
    std::cout << response.content << std::endl;
    promise->set_value();
  });

  promise->get_future().get();

  return 0;
}