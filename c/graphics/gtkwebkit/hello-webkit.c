#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

int main(int argc, char *argv[])
{
    // gtk_init(&argc, &argv);
    gtk_init(&argc, &argv);

    /* Create a top-level window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_title(GTK_WINDOW(window), "WebKitGTK Hello World");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Create the WebKit web view */
    WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new());

    /* Load simple HTML */
    const char *html =
        "<!doctype html>"
        "<html>"
        "<body style='font-family:sans-serif'>"
        "<h1>Hello, WebKitGTK!</h1>"
        "<p>This page is rendered by WebKit.</p>"
        "</body>"
        "<script>alert('ehet')</script>"
        "</html>";

    webkit_web_view_load_html(webview, html, NULL);

    /* Put the web view into the window */
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webview));

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
