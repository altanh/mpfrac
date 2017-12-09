#!/bin/bash
INCLUDE="-I/usr/local/Cellar/wxmac/3.0.3.1_1/lib/wx/include/osx_cocoa-unicode-3.0 -I/usr/local/Cellar/wxmac/3.0.3.1_1/include/wx-3.0  -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__"
LIB="-L/usr/local/lib -L/usr/local/Cellar/wxmac/3.0.3.1_1/lib   -framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework System -framework OpenGL -lwx_osx_cocoau_xrc-3.0 -lwx_osx_cocoau_webview-3.0 -lwx_osx_cocoau_html-3.0 -lwx_osx_cocoau_qa-3.0 -lwx_osx_cocoau_adv-3.0 -lwx_osx_cocoau_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0"
OUT="mpfrac-gui"
g++ -std=c++11 -g $INCLUDE $LIB src/gui/*.cpp -o $OUT