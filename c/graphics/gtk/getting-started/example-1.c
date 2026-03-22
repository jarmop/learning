#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 800);

  box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

  gtk_window_set_child (GTK_WINDOW (window), box);

  button = gtk_button_new_with_label ("Hello World");
  GtkWidget *button2 = gtk_button_new_with_label ("button2");

  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  /**
   * g_signal_connect_swapped simply adds another listener to the button, just 
   * like g_signal_connect, but it swaps the callback arguments, so that data 
   * (window) becomes the first argument, and target (button) becomes the second 
   */
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);

  gtk_box_append (GTK_BOX (box), button);
  gtk_box_append (GTK_BOX (box), button2);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
