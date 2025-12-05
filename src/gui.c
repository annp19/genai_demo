#include <gtk/gtk.h>
#include <cairo.h>
#include <glib.h>
#include "gui.h"
#include "data.h"
#include "telemetry.h"

static GtkWidget *window, *speed_label, *battery_label, *gps_label, *speed_chart, *battery_chart, *export_button;

static void on_export_clicked(GtkButton *button, gpointer data) {
    data_export_csv("telemetry.csv");
}

static gboolean draw_speed_chart(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr, 2);
    for (int i = 1; i < chart_buffer.count; i++) {
        TelemetryData *d1 = telemetry_buffer_get(&chart_buffer, i-1);
        TelemetryData *d2 = telemetry_buffer_get(&chart_buffer, i);
        if (!d1 || !d2) continue;
        double x1 = (i-1) * (width / 99.0);
        double y1 = height - (d1->speed / 60.0) * height;
        double x2 = i * (width / 99.0);
        double y2 = height - (d2->speed / 60.0) * height;
        cairo_move_to(cr, x1, y1);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);
    return FALSE;
}

static gboolean draw_battery_chart(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_set_line_width(cr, 2);
    for (int i = 1; i < chart_buffer.count; i++) {
        TelemetryData *d1 = telemetry_buffer_get(&chart_buffer, i-1);
        TelemetryData *d2 = telemetry_buffer_get(&chart_buffer, i);
        if (!d1 || !d2) continue;
        double x1 = (i-1) * (width / 99.0);
        double y1 = height - (d1->battery / 100.0) * height;
        double x2 = i * (width / 99.0);
        double y2 = height - (d2->battery / 100.0) * height;
        cairo_move_to(cr, x1, y1);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);
    return FALSE;
}

void gui_init(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Vehicle Telemetry Dashboard");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Labels
    speed_label = gtk_label_new("Speed: 0.00 km/h");
    gtk_box_pack_start(GTK_BOX(vbox), speed_label, FALSE, FALSE, 0);

    battery_label = gtk_label_new("Battery: 0.00%");
    gtk_box_pack_start(GTK_BOX(vbox), battery_label, FALSE, FALSE, 0);

    gps_label = gtk_label_new("GPS: 0.000000, 0.000000");
    gtk_box_pack_start(GTK_BOX(vbox), gps_label, FALSE, FALSE, 0);

    // Charts
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    speed_chart = gtk_drawing_area_new();
    gtk_widget_set_size_request(speed_chart, 400, 200);
    g_signal_connect(speed_chart, "draw", G_CALLBACK(draw_speed_chart), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), speed_chart, TRUE, TRUE, 0);

    battery_chart = gtk_drawing_area_new();
    gtk_widget_set_size_request(battery_chart, 400, 200);
    g_signal_connect(battery_chart, "draw", G_CALLBACK(draw_battery_chart), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), battery_chart, TRUE, TRUE, 0);

    // Button
    export_button = gtk_button_new_with_label("Export CSV");
    g_signal_connect(export_button, "clicked", G_CALLBACK(on_export_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), export_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

void gui_update_labels(TelemetryData data) {
    char buf[64];
    sprintf(buf, "Speed: %.2f km/h", data.speed);
    gtk_label_set_text(GTK_LABEL(speed_label), buf);
    sprintf(buf, "Battery: %.2f%%", data.battery);
    gtk_label_set_text(GTK_LABEL(battery_label), buf);
    sprintf(buf, "GPS: %.6f, %.6f", data.latitude, data.longitude);
    gtk_label_set_text(GTK_LABEL(gps_label), buf);
}

void gui_redraw_charts() {
    gtk_widget_queue_draw(speed_chart);
    gtk_widget_queue_draw(battery_chart);
}

void gui_main_loop() {
    gtk_main();
}