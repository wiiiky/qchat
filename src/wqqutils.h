#ifndef __WQQ_UTILS_H__
#define __WQQ_UTILS_H__
#include <gtk/gtk.h>

void gtk_widget_apply_css_from_path(GtkWidget * widget,
									const gchar * path);
void gtk_entry_set_icon_from_path(GtkEntry * entry,
								  GtkEntryIconPosition pos,
								  const gchar * path);
gint wqq_message_dialog_run(GtkWindow * parent,
							GtkDialogFlags flags,
							GtkMessageType type,
							GtkButtonsType buttons,
							const gchar * message_format,
							...) G_GNUC_PRINTF(5, 6);
GtkWidget *gtk_image_new_from_file_at_size(const gchar * filepath,
										   guint width, guint height);

gchar *gtk_text_view_get_text(GtkTextView * view);
void gtk_text_view_clear_text(GtkTextView * view);
gchar *g_str_replace_c(const gchar * text, gchar c, gchar * d);
#endif
