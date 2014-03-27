#!/bin/bash
# 解决wchat所需要的依赖
# libgtk-3-dev
# libsoup2.4-dev glib的http库
# libjson-glib-dev glib的json库，webqq的协议数据以json格式传送
# libsecret-1-dev glib的密码保存库
# libwebkitgtk-dev gtk+包装的webkit库，聊天界面用到
# libnotify-dev 系统的消息提示功能

sudo apt-get install libsoup2.4-dev libgtk-3-dev \
	libjson-glib-dev libsecret-1-dev libwebkitgtk-3.0-dev \
	libnotify-dev
