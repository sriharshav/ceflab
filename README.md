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

Run Hello World
========================
Open \helloworld\\helloworld.sln from VS2012 and build in both Release and Debug configurations.

