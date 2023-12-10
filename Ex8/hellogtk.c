#include <gtk/gtk.h>

void button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hello, World!\n");
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hello, World!");

    GtkWidget *button = gtk_button_new_with_label("Hello World!");
    
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
