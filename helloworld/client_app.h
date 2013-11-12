#ifndef CEFHW_CEFCLIENT_H_
#define CEFHW_CEFCLIENT_H_
#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"

class ClientApp : public CefApp {
 public:
  ClientApp();
  IMPLEMENT_REFCOUNTING(ClientApp);
};

#endif  // CEFHW_CEFCLIENT_H_
