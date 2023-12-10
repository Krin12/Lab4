#include <gtk/gtk.h>

double num1 = 0, num2 = 0, result = 0;
char operator = '\0';

GtkWidget *entry;

void on_number_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
    gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar *new_text = g_strconcat(current_text, label, NULL);
    gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    g_free(new_text);
}

void on_operator_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
    operator = label[0];
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    num1 = atof(current_text);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void on_equal_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    num2 = atof(current_text);

    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                g_print("Error: Cannot divide 0\n");
                result = 0;
            }
            break;
        default:
            break;
    }

    if (result == (int)result || result - (int)result != 0) {
        gchar result_str[50];
        g_snprintf(result_str, sizeof(result_str), "%g", result);
        gtk_entry_set_text(GTK_ENTRY(entry), result_str);
    } else {
        gchar result_str[50];
        g_snprintf(result_str, sizeof(result_str), "%d", (int)result);
        gtk_entry_set_text(GTK_ENTRY(entry), result_str);
    }
}

void on_clear_button_clicked(GtkWidget *button, gpointer data) {
    num1 = 0;
    num2 = 0;
    result = 0;
    operator = '\0';
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    const gchar *buttons[17] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+",
        "C"
    };

    for (int i = 0; i < 17; i++) {
        GtkWidget *button = gtk_button_new_with_label(buttons[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_number_button_clicked), NULL);
        gtk_grid_attach(GTK_GRID(grid), button, i % 4, i / 4 + 1, 1, 1);

        if (i == 14) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_equal_button_clicked), NULL);
        } else if (i == 3 || i == 7 || i == 11 || i == 15) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_operator_button_clicked), NULL);
        } else if (i == 16) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_clear_button_clicked), NULL);
        }
    }

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
