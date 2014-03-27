## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DWQQ_DATA_DIR=\""$(srcdir)/../data"\" \
	-DPACKAGE_PREFIX=\""$(prefix)"\" \
	$(QCHAT_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = qchat

qchat_SOURCES =  \
	main.c \
	wqqm4.h \
	wqquicaptcha.c \
	wqquicaptcha.h \
	wqquilogin.c \
	wqquilogin.h \
	wqquiqqchat.c \
	wqquiqqchat.h \
	wqquiqqpanel.c \
	wqquiqqpanel.h \
	wqqutils.c \
	wqqutils.h \
	wqqui/wqqqqchat.c \
	wqqui/wqqqqchat.h \
	wqqui/wqqqqchatmessage.c \
	wqqui/wqqqqchatmessage.h \
	wqqui/wqquiqqcategory.c \
	wqqui/wqquiqqcategory.h \
	wqqui/wqquiqqcell.c \
	wqqui/wqquiqqcell.h \
	wqqui/wqquiqqgroup.c \
	wqqui/wqquiqqgroup.h \
	wqqui/wqquiqqgrouplist.c \
	wqqui/wqquiqqgrouplist.h \
	wqqui/wqquiqqheader.c \
	wqqui/wqquiqqheader.h \
	wqqui/wqquiqqlist.c \
	wqqui/wqquiqqlist.h \
	wqq/md5.c \
	wqq/md5.h \
	wqq/wqq.h \
	wqq/wqqcaptcha.c \
	wqq/wqqcaptcha.h \
	wqq/wqqcategory.c \
	wqq/wqqcategory.h \
	wqq/wqqcrypto.c \
	wqq/wqqcrypto.h \
	wqq/wqqfriend.c \
	wqq/wqqfriend.h \
	wqq/wqqgroup.c \
	wqq/wqqgroup.h \
	wqq/wqqgroupmember.c \
	wqq/wqqgroupmember.h \
	wqq/wqqmessagecontent.c \
	wqq/wqqmessagecontent.h \
	wqq/wqqpollmessage.c \
	wqq/wqqpollmessage.h \
	wqq/wqqpollmessagelist.c \
	wqq/wqqpollmessagelist.h \
	wqq/wqqsendmessage.c \
	wqq/wqqsendmessage.h \
	wqq/wqqsession.c \
	wqq/wqqsession.h \
	wqq/wqqtypes.h \
	wqq/wqquser.c \
	wqq/wqquser.h

qchat_LDFLAGS = \
	-Wl,--export-dynamic

qchat_LDADD = $(QCHAT_LIBS)


