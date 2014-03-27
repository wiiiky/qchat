#include "wqqutils.h"

static void apply_css(GtkWidget * widget, GtkStyleProvider * provider);

void gtk_widget_apply_css_from_path(GtkWidget * widget, const gchar * path)
{
	GtkStyleProvider *provider;
	provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());
	if (gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider),
										path, NULL))
		apply_css(widget, provider);
	g_object_unref(provider);
}

void gtk_entry_set_icon_from_path(GtkEntry * entry,
								  GtkEntryIconPosition pos,
								  const gchar * path)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
	if (pixbuf != NULL) {
		gtk_entry_set_icon_from_pixbuf(entry, pos, pixbuf);
		g_object_unref(pixbuf);
	}
}

static void apply_css(GtkWidget * widget, GtkStyleProvider * provider)
{
	gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
								   provider,
								   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	if (GTK_IS_CONTAINER(widget))
		gtk_container_foreach(GTK_CONTAINER(widget),
							  (GtkCallback) apply_css, provider);
}

gint wqq_message_dialog_run(GtkWindow * parent,
							GtkDialogFlags flags,
							GtkMessageType type,
							GtkButtonsType buttons,
							const gchar * message_format, ...)
{
	gchar *msg = NULL;
	va_list args;
	if (message_format) {
		va_start(args, message_format);
		msg = g_strdup_vprintf(message_format, args);
		va_end(args);
	}
	GtkWidget *dialog = gtk_message_dialog_new(parent,
											   flags, type, buttons, "%s",
											   msg != NULL ? msg : "");
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return result;
}

GtkWidget *gtk_image_new_from_file_at_size(const gchar * filepath,
										   guint width, guint height)
{
	GdkPixbuf *pixbuf =
		gdk_pixbuf_new_from_file_at_size(filepath, width, height, NULL);
	GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
	return image;
}

gchar *gtk_text_view_get_text(GtkTextView * view)
{
	g_return_val_if_fail(GTK_IS_TEXT_VIEW(view), NULL);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
	return text;
}

void gtk_text_view_clear_text(GtkTextView * view)
{
	g_return_if_fail(GTK_IS_TEXT_VIEW(view));
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
	gtk_text_buffer_set_text(buffer, "", 0);
}

gchar *g_str_replace_c(const gchar * text, gchar c, gchar * d)
{
	if (text == NULL)
		return NULL;
	GString *string = g_string_new("");
	while (*text != '\0') {
		if (*text != c)
			g_string_append_c(string, *text);
		else
			g_string_append(string, d);
		text++;
	}
	return g_string_free(string, FALSE);
}
