#include <string.h>
#include "wqqqqchat.h"


G_DEFINE_TYPE(WqqQQChat, wqq_qqchat, GTK_TYPE_SCROLLED_WINDOW);

static void wqq_qqchat_getter(GObject * object, guint property_id,
							  GValue * value, GParamSpec * pspec);
static void wqq_qqchat_setter(GObject * object, guint property_id,
							  const GValue * value, GParamSpec * pspec);

static void _on_auto_scroll(GtkWidget * view, GdkRectangle * alloc,
							gpointer data);
static gboolean _block_context_menu(GtkWidget * widget, gpointer data);
static void on_webview_load_finished(WebKitWebView * webview,
									 WebKitWebFrame * frame,
									 gpointer user_data);

static void wqq_qqchat_init(WqqQQChat * chat)
{
	gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(chat), NULL);
	gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(chat), NULL);
	/*GtkWidget *viewport=gtk_viewport_new(NULL,NULL); */
	/*gtk_scrollable_set_vadjustment(GTK_SCROLLABLE(viewport),NULL); */
	chat->webview = webkit_web_view_new();
	/*gtk_container_add(GTK_CONTAINER(chat),viewport); */
	gtk_container_add(GTK_CONTAINER(chat), chat->webview);
}

static void wqq_qqchat_class_init(WqqQQChatClass * klass)
{
	G_OBJECT_CLASS(klass)->get_property = wqq_qqchat_getter;
	G_OBJECT_CLASS(klass)->set_property = wqq_qqchat_setter;
}

static void wqq_qqchat_getter(GObject * object, guint property_id,
							  GValue * value, GParamSpec * pspec)
{
	WqqQQChat *chat = WQQ_QQCHAT(object);
}

static void wqq_qqchat_setter(GObject * object, guint property_id,
							  const GValue * value, GParamSpec * pspec)
{
	WqqQQChat *chat = WQQ_QQCHAT(object);
}

static gboolean on_timeout_scroll(gpointer data)
{
	GtkAdjustment *adj =
		gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(data));
	gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
	return FALSE;
}


static void _on_auto_scroll(GtkWidget * view, GdkRectangle * alloc,
							gpointer data)
{
	g_timeout_add(50, on_timeout_scroll, data);
}

static gboolean _block_context_menu(GtkWidget * widget, gpointer data)
{
	return TRUE;
}

static void on_webview_load_finished(WebKitWebView * webview,
									 WebKitWebFrame * frame,
									 gpointer user_data)
{
	WqqQQChat *chat = (WqqQQChat *) user_data;
	chat->loading = FALSE;
}


/***********************************************
 * PUBLIC
 **********************************************/
WqqQQChat *wqq_qqchat_new(const gchar * base, const gchar * css_file)
{
	WqqQQChat *chat = (WqqQQChat *) g_object_new(WQQ_TYPE_QQCHAT, NULL);
	GtkWidget *webview = chat->webview;

	GMappedFile *file = g_mapped_file_new(css_file, FALSE, NULL);
	if (file != NULL) {
		gchar *CSS = g_mapped_file_get_contents(file);
		gchar *tmp =
			g_base64_encode((guchar *) CSS, strlen((gchar *) CSS));
		gchar *css = g_strconcat("data:text/css;charset=utf-8;base64,",
								 tmp, NULL);
		g_object_set(webkit_web_view_get_settings
					 (WEBKIT_WEB_VIEW(webview)), "user-stylesheet-uri",
					 css, NULL);
		g_free(css);
		g_free(tmp);
		g_mapped_file_unref(file);
	}
	webkit_web_view_load_string(WEBKIT_WEB_VIEW(webview),
				"<html><body></body></html>",
				"text/html", "UTF-8", base);
	chat->loading = TRUE;

	g_signal_connect(G_OBJECT(chat->webview), "size-allocate",
				G_CALLBACK(_on_auto_scroll), chat);
	g_signal_connect(G_OBJECT(chat->webview), "context-menu",
					 G_CALLBACK(_block_context_menu), NULL);
	g_signal_connect(G_OBJECT(chat->webview), "load-finished",
					 G_CALLBACK(on_webview_load_finished), chat);


	return chat;
}

void wqq_qqchat_append_time(WqqQQChat * chat, GDateTime * dt)
{
	g_return_if_fail(WQQ_IS_QQCHAT(chat));

	while (chat->loading) {
		gtk_main_iteration_do(TRUE);
	}

	gchar *now = g_date_time_format(dt, "%m-%d %k:%M");

	WebKitDOMDocument *doc =
		webkit_web_view_get_dom_document(WEBKIT_WEB_VIEW(chat->webview));
	WebKitDOMElement *body = webkit_dom_document_query_selector(doc,
																"body",
																NULL);
	WebKitDOMElement *div = webkit_dom_document_create_element(doc,
															   "div",
															   NULL);
	WebKitDOMElement *span = webkit_dom_document_create_element(doc,
																"span",
																NULL);
	webkit_dom_element_set_attribute(div, "class", "chat_time", NULL);
	webkit_dom_node_set_text_content(WEBKIT_DOM_NODE(span), now, NULL);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(body),
								 WEBKIT_DOM_NODE(div), NULL);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(div),
								 WEBKIT_DOM_NODE(span), NULL);
	g_free(now);
}

void wqq_qqchat_append_time_now(WqqQQChat * chat)
{
	g_return_if_fail(WQQ_IS_QQCHAT(chat));

	GDateTime *dt = g_date_time_new_now_local();
	wqq_qqchat_append_time(chat, dt);
	g_date_time_unref(dt);
}

static WebKitDOMElement *webkit_element_avatar_img(WebKitDOMDocument * doc,
												   const gchar * avatar)
{
	WebKitDOMElement *img =
		webkit_dom_document_create_element(doc, "img", NULL);
	webkit_dom_element_set_attribute(img, "src", avatar, NULL);
	webkit_dom_element_set_attribute(img, "width", "40px", NULL);
	webkit_dom_element_set_attribute(img, "height", "40px", NULL);
	webkit_dom_element_set_attribute(img, "class", "chat_content_avatar",
									 NULL);
	return img;
}

static WebKitDOMElement *webkit_element_chat_content(WebKitDOMDocument *
													 doc,
													 WqqQQChatMessage *
													 msg)
{
	WebKitDOMElement *ct =
		webkit_dom_document_create_element(doc, "p", NULL);
	webkit_dom_element_set_attribute(ct, "class", "chat_content", NULL);
	GList *lp = msg->list;
	WqqQQChatMessageContent *mct;
	WebKitDOMElement *span, *img, *br, *inspan;
	while (lp != NULL) {
		mct = (WqqQQChatMessageContent *) lp->data;
		switch (mct->type) {
		case WQQ_QQCHAT_MESSAGE_TYPE_TEXT:
			span = webkit_dom_document_create_element(doc, "span", NULL);
			gchar **strv = g_strsplit(mct->text, "\n", -1);
			if (strv) {
				guint i = 0;
				guint len = g_strv_length(strv);
				for (i = 0; i < len - 1; i++) {
					br = webkit_dom_document_create_element(doc, "br",
															NULL);
					inspan =
						webkit_dom_document_create_element(doc, "span",
														   NULL);
					webkit_dom_node_set_text_content(WEBKIT_DOM_NODE
													 (inspan), strv[i],
													 NULL);
					webkit_dom_node_append_child(WEBKIT_DOM_NODE(inspan),
												 WEBKIT_DOM_NODE(br),
												 NULL);
					webkit_dom_node_append_child(WEBKIT_DOM_NODE(span),
												 WEBKIT_DOM_NODE(inspan),
												 NULL);
				}
				inspan =
					webkit_dom_document_create_element(doc, "span", NULL);
				webkit_dom_node_set_text_content(WEBKIT_DOM_NODE(inspan),
												 strv[i], NULL);
				webkit_dom_node_append_child(WEBKIT_DOM_NODE(span),
											 WEBKIT_DOM_NODE(inspan),
											 NULL);
				g_strfreev(strv);
			}
			webkit_dom_node_append_child(WEBKIT_DOM_NODE(ct),
										 WEBKIT_DOM_NODE(span), NULL);
			break;
		case WQQ_QQCHAT_MESSAGE_TYPE_IMAGE:
			img = webkit_dom_document_create_element(doc, "img", NULL);
			webkit_dom_element_set_attribute(img, "class", "icon", NULL);
			webkit_dom_element_set_attribute(img, "src", mct->image, NULL);
			webkit_dom_node_append_child(WEBKIT_DOM_NODE(ct),
										 WEBKIT_DOM_NODE(img), NULL);
			break;
		case WQQ_QQCHAT_MESSAGE_TYPE_FACE:
		default:
			break;
		}
		lp = g_list_next(lp);
	}
	return ct;
}

static WebKitDOMElement
	* webkit_element_chat_content_group(WebKitDOMDocument * doc,
										const gchar * avatar,
										WqqQQChatMessage * msg)
{
	WebKitDOMElement *chat_content_group =
		webkit_dom_document_create_element(doc, "div", NULL);

	WebKitDOMElement *chat_content_avatar =
		webkit_element_avatar_img(doc, avatar);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(chat_content_group),
								 WEBKIT_DOM_NODE(chat_content_avatar),
								 NULL);

	WebKitDOMElement *chat_nick =
		webkit_dom_document_create_element(doc, "p", NULL);
	webkit_dom_element_set_attribute(chat_nick, "class", "chat_nick",
									 NULL);
	webkit_dom_node_set_text_content(WEBKIT_DOM_NODE(chat_nick), msg->nick,
									 NULL);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(chat_content_group),
								 WEBKIT_DOM_NODE(chat_nick), NULL);

	WebKitDOMElement *chat_content = webkit_element_chat_content(doc, msg);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(chat_content_group),
								 WEBKIT_DOM_NODE(chat_content), NULL);

	return chat_content_group;
}


void wqq_qqchat_append_message_from_buddy(WqqQQChat * chat,
										  const gchar * avatar,
										  WqqQQChatMessage * msg)
{
	g_return_if_fail(WQQ_IS_QQCHAT(chat) && msg != NULL);

	while (chat->loading) {
		gtk_main_iteration_do(TRUE);
	}
	WebKitDOMDocument *doc =
		webkit_web_view_get_dom_document(WEBKIT_WEB_VIEW(chat->webview));
	WebKitDOMElement *body =
		webkit_dom_document_query_selector(doc, "body", NULL);

	WebKitDOMElement *chat_content_group =
		webkit_element_chat_content_group(doc, avatar, msg);
	webkit_dom_element_set_attribute(chat_content_group,
									 "class", "chat_content_group buddy",
									 NULL);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(body),
								 WEBKIT_DOM_NODE(chat_content_group),
								 NULL);
}

void wqq_qqchat_append_message_from_self(WqqQQChat * chat,
										 const gchar * avatar,
										 WqqQQChatMessage * msg)
{
	g_return_if_fail(WQQ_IS_QQCHAT(chat) && msg != NULL);

	while (chat->loading) {
		gtk_main_iteration_do(TRUE);
	}
	WebKitDOMDocument *doc =
		webkit_web_view_get_dom_document(WEBKIT_WEB_VIEW(chat->webview));
	WebKitDOMElement *body =
		webkit_dom_document_query_selector(doc, "body", NULL);

	WebKitDOMElement *chat_content_group =
		webkit_element_chat_content_group(doc, avatar, msg);
	webkit_dom_element_set_attribute(chat_content_group,
									 "class", "chat_content_group self",
									 NULL);
	webkit_dom_node_append_child(WEBKIT_DOM_NODE(body),
								 WEBKIT_DOM_NODE(chat_content_group),
								 NULL);
}

void wqq_qqchat_set_base_uri(WqqQQChat * chat, const gchar * base)
{
	g_return_if_fail(WQQ_IS_QQCHAT(chat));
	webkit_web_view_load_string(WEBKIT_WEB_VIEW(chat->webview),
								"<html><body></body></html>",
								"text/html", "UTF-8", base);
	chat->loading = TRUE;
}
