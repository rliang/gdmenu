/*
 * Copyright (C) 2013 Ricardo Liang <ricardoliang@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

const gchar *title = "gdmenu";
gint max_entries = G_MAXINT;

GOptionEntry option_entries[] = {
	{
		"title", 't', 0,
		G_OPTION_ARG_STRING, &title,
		"Set window title to TITLE", "TITLE"
	}, {
		"max-entries", 'm', 0,
		G_OPTION_ARG_INT, &max_entries,
		"Display MAX entries at a time", "MAX"
	}, { NULL }
};

GSList *input = NULL;

GtkEntry *entry = NULL;
GtkListStore *list_store = NULL;
GtkTreeSelection *tree_selection = NULL;

gint selected_row(void)
{
	GtkTreeIter i;
	if (!gtk_tree_selection_get_selected(tree_selection, NULL, &i))
		return -1;
	GtkTreePath *p = gtk_tree_model_get_path(GTK_TREE_MODEL(list_store), &i);
	gint row = gtk_tree_path_get_indices(p)[0];
	gtk_tree_path_free(p);
	return row;
}

void select_row(gint row)
{
	if (row < 0)
		return;
	GtkTreePath *p = gtk_tree_path_new_from_indices(row, -1);
	gtk_tree_selection_select_path(tree_selection, p);
	gtk_tree_path_free(p);
}

const gchar *row_text(gint row)
{
	const gchar *s = "";
	if (row < 0)
		return s;

	GtkTreePath *p = gtk_tree_path_new_from_indices(row, -1);
	GtkTreeIter i;
	if (!gtk_tree_model_get_iter((GTK_TREE_MODEL(list_store)), &i, p))
		return s;

	gtk_tree_model_get(GTK_TREE_MODEL(list_store), &i, 0, &s, -1);
	gtk_tree_path_free(p);
	return s;
}

const gchar *entry_text(void)
{
	return gtk_entry_get_text(entry);
}

void output(const gchar *s)
{
	g_print("%s\n", s[0] == '\0' ? entry_text() : s);
	gtk_main_quit();
}

void on_entry_changed(void)
{
	gtk_list_store_clear(list_store);

	gint count = 0;
	GSList *i = input;
	for (; i != NULL && count < max_entries; i = g_slist_next(i))
		if (strstr(i->data, entry_text()) != NULL) {
			gtk_list_store_insert_with_values(list_store, NULL, -1,
					0, i->data, -1);
			count += 1;
		}

	select_row(0);
}

gboolean on_entry_key_press_event(GtkEntry *entry, GdkEvent *event)
{
	guint k = GDK_KEY_VoidSymbol;
	gdk_event_get_keyval(event, &k);

	GdkModifierType s;
	gdk_event_get_state(event, &s);

#define KEY(S) (k == S)
#define CTRL(S) (KEY(S) && s & GDK_CONTROL_MASK)
#define SHIFT(S) (KEY(S) && s & GDK_SHIFT_MASK)

	if (KEY(GDK_KEY_Escape) || CTRL(GDK_KEY_c))
		gtk_main_quit();
	else if (KEY(GDK_KEY_Tab) || CTRL(GDK_KEY_i))
		gtk_entry_set_text(entry, row_text(selected_row()));
	else if (KEY(GDK_KEY_Down) || CTRL(GDK_KEY_f))
		select_row(selected_row() + 1);
	else if (KEY(GDK_KEY_Up) || CTRL(GDK_KEY_b))
		select_row(selected_row() - 1);
	else if (KEY(GDK_KEY_Return) || CTRL(GDK_KEY_j))
		output(row_text(selected_row()));
	else if (SHIFT(GDK_KEY_Return) || CTRL(GDK_KEY_J))
		output(entry_text());

	return FALSE;
}

void parse_options(int *argc, char **argv[])
{
	GOptionContext *oc = g_option_context_new(NULL);
	g_option_context_add_main_entries(oc, option_entries, NULL);
	g_option_context_add_group(oc, gtk_get_option_group(TRUE));
	g_option_context_parse(oc, argc, argv, NULL);
	g_option_context_free(oc);
}

void build_interface(void)
{
	GtkWindow *w = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_window_set_title(w, title);
	gtk_window_set_gravity(w, GDK_GRAVITY_CENTER);
	gtk_window_set_position(w, GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_type_hint(w, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_icon_name(w, "dialog-question");

	GtkBox *b = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));

	entry = GTK_ENTRY(gtk_entry_new());
	gtk_widget_grab_focus(GTK_WIDGET(entry));

	list_store = gtk_list_store_new(1, G_TYPE_STRING);

	GtkTreeView *t = GTK_TREE_VIEW(gtk_tree_view_new_with_model(
				GTK_TREE_MODEL(list_store)));
	gtk_tree_view_set_headers_visible(t, FALSE);
	gtk_tree_view_append_column(t, gtk_tree_view_column_new_with_attributes("",
				gtk_cell_renderer_text_new(), "text", 0, NULL));

	tree_selection = gtk_tree_view_get_selection(t);

#define NO_FOCUS(W) gtk_widget_set_can_focus(GTK_WIDGET(W), FALSE) 

	NO_FOCUS(w);
	NO_FOCUS(b);
	NO_FOCUS(t);

#define BOX_APPEND(B, W, E, F) gtk_box_pack_start(b, GTK_WIDGET(W), E, F, 0)
	
	BOX_APPEND(b, entry, FALSE, TRUE);
	BOX_APPEND(b, t, TRUE, TRUE);
	gtk_container_add(GTK_CONTAINER(w), GTK_WIDGET(b));

#define CONNECT(O, S, C) g_signal_connect(G_OBJECT(O), S, G_CALLBACK(C), NULL)

	CONNECT(w, "destroy", gtk_main_quit);
	CONNECT(entry, "changed", on_entry_changed);
	CONNECT(entry, "key-press-event", on_entry_key_press_event);

	gtk_widget_show_all(GTK_WIDGET(w));
}

void read_input(void)
{
	while (TRUE) {
		gchar buf[BUFSIZ];
		if (fgets(buf, BUFSIZ, stdin) == NULL)
			break;
		buf[strlen(buf) - 1] = '\0';

		input = g_slist_append(input, g_strdup(buf));
	}

	on_entry_changed();
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	parse_options(&argc, &argv);
	build_interface();
	read_input();
	gtk_main();
	return 0;
}

/* vim: set ts=4 sw=4 noet: */
