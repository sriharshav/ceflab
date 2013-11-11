// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEFHW_CLIENT_HANDLER_H_
#define CEFHW_CLIENT_HANDLER_H_
#pragma once

#include "include/cef_client.h"

// ClientHandler implementation.
class ClientHandler : public CefClient,
  public CefLifeSpanHandler,
  public CefRequestHandler,
  public CefContextMenuHandler {
    public:
      ClientHandler();
      virtual ~ClientHandler();

      virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
        return this;
      }
      virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
        return this;
      }
      virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
        return this;
      }

      // CefLifeSpanHandler methods
      virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
      virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
      virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

      // CefRequestHandler methods
      virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
          CefRefPtr<CefBrowser> browser,
          CefRefPtr<CefFrame> frame,
          CefRefPtr<CefRequest> request) OVERRIDE;

      // CefContextMenuHandler methods
      virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
          CefRefPtr<CefFrame> frame,
          CefRefPtr<CefContextMenuParams> params,
          CefRefPtr<CefMenuModel> model) OVERRIDE;

      CefWindowHandle GetMainHwnd() { return m_MainHwnd; }
      void SetMainHwnd(CefWindowHandle hwnd);
      CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }

      // Request that all existing browser windows close.
      void CloseAllBrowsers(bool force_close);

      // Returns true if the main browser window is currently closing. Used in
      // combination with DoClose() and the OS close notification to properly handle
      // 'onbeforeunload' JavaScript events during window close.
      bool IsClosing() { return m_bIsClosing; }

      // Returns the startup URL.
      std::string GetStartupURL() { return m_StartupURL; }

    protected:
      // The child browser window
      CefRefPtr<CefBrowser> m_Browser;

      // The main frame window handle
      CefWindowHandle m_MainHwnd;

      // The child browser id
      int m_BrowserId;

      // True if the main browser window is currently closing.
      bool m_bIsClosing;

      // The startup URL.
      std::string m_StartupURL;

      // Number of currently existing browser windows. The application will exit
      // when the number of windows reaches 0.
      static int m_BrowserCount;

      // Include the default reference counting implementation.
      IMPLEMENT_REFCOUNTING(ClientHandler);
      // Include the default locking implementation.
      IMPLEMENT_LOCKING(ClientHandler);
  };

#endif  // CEFHW_CLIENT_HANDLER_H_
