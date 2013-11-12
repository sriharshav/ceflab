ceflab
======

Experimental projects with Chromium Embedded Framework on Windows

Download CEF and copy dependencies
==================================
1. Clone this repo.
2. Download [Windows 32bit 2013-11-08 CEF 3.1650.1508](http://cefbuilds.com/).
3. Extract and copy following folders to cloned folder
  - Debug
  - Release
  - libcef_dll
  - include

Build libcef_dll_wrapper
========================
Open libcef_dll_wrapper\libcef_dll_wrapper.sln from VS2012 and build in both Release and Debug configurations.
Static library libcef_dll_wrapper.lib will be copied to Debug|Release folder.

Run Hello World
========================
Open helloworld\\helloworld.sln from VS2012 and build. Binary will be copied to Debug|Release folder.

It is a simple Win32 project with `WinMain` as entry point function. Entry point __initialize cef__, __creates main window__ and __starts message loop__. 

Initialization needs 
 - CefMainArgs
 - CefSettings 
 - CefApp
CefMainArgs is passed just hInstance of WinMain. Only one setting [pack_loading_disabled](http://magpcss.org/ceforum/apidocs/projects/\(default\)/_cef_settings_t.html#pack_loading_disabled) is used. CefApp instance ClientApp is declared in `client_app.h`. It is very minimal derivation from CefApp with [reference counting](https://code.google.com/p/chromiumembedded/wiki/GeneralUsage#Reference_Counting). Creating main window is just like any windows app.  Message loop `WndProc` does main job of creating browser in `WM_CREATE` function. 

`ClienHandler` implements CefClient interface which provides access to browser-instance-specific callbacks. These callbacks provide opportinity to integrate application specific implementation. One of three(CefLifeSpanHandler,CefRequestHandler,CefContextMenuHandler) other interfaces implemented by ClientHandler is `CefRequestHandler`. This interface provides `GetResourceHandler` method using with a request can be intercepted and handled by applicaition.

