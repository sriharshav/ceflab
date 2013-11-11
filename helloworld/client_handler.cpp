// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "client_app.h"
#include "util.h"
#include "client_handler.h"
#include "include/cef_runnable.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "tchar.h"

int ClientHandler::m_BrowserCount = 0;
const char kOrigin[] = "http://res/";

ClientHandler::ClientHandler()
  : m_MainHwnd(NULL),
  m_BrowserId(0),
  m_bIsClosing(false) {
	  if (m_StartupURL.empty())
		  m_StartupURL = "http://res/index.html";

}

ClientHandler::~ClientHandler() {
}

CefRefPtr<CefResourceHandler> ClientHandler::GetResourceHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) 
{
  std::string url = request->GetURL();
  if (url.find(kOrigin) == 0) 
  {
	  std::string data =  "<html><body bgcolor=\"#eee\"><h1>Hello CEF!</h1></body></html>";
	  CefRefPtr<CefStreamReader> stream = 
            CefStreamReader::CreateForData(
                static_cast<void*>(const_cast<char*>(data.c_str())),
                data.size());
	  if (stream.get())
		  return new CefStreamResourceHandler("text/html", stream);
  }
  return NULL;
}

void ClientHandler::SetMainHwnd(CefWindowHandle hwnd) {
	AutoLock lock_scope(this);
	m_MainHwnd = hwnd;
}

void ClientHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			NewCefRunnableMethod(this, &ClientHandler::CloseAllBrowsers,
			force_close));
		return;
	}

	if (m_Browser.get()) {
		// Request that the main browser close.
		m_Browser->GetHost()->CloseBrowser(force_close);
	}
}


void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	AutoLock lock_scope(this);
	if (!m_Browser.get())   {
		// We need to keep the main child window, but not popup windows
		m_Browser = browser;
		m_BrowserId = browser->GetIdentifier();
	}

	m_BrowserCount++;
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (m_BrowserId == browser->GetIdentifier()) {
		// Notify the browser that the parent window is about to close.
		browser->GetHost()->ParentWindowWillClose();

		// Set a flag to indicate that the window close should be allowed.
		m_bIsClosing = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier()) {
		// Free the browser pointer so that the browser can be destroyed
		m_Browser = NULL;
	}

	if (--m_BrowserCount == 0) {
		// All browser windows have closed. Quit the application message loop.
		CefQuitMessageLoop();
	}
}

void ClientHandler::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
		model->Clear();
}
