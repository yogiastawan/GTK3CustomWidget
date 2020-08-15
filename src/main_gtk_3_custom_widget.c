#include <gtk/gtk.h>

#include "./customWidget/gisagauge.h"
#include "./customWidget/gisacompass.h"

void gaugeTest(GtkWidget *widget, gpointer *data);
void compassTest(GtkWidget *widget, gpointer *data);
void main_ui(GtkApplication *app, gpointer *user_data);

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;
	//change id & flags of your app
	app = gtk_application_new("id.yogiastawan.gtk3customwidget", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(main_ui), "GTK 3 Custom Widget");
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}

void main_ui(GtkApplication *app, gpointer *user_data)
{
	GtkWidget *window;
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), (gchar *)user_data);

	GtkWidget *slider1, *slider2, *slider_box, *box, *boxh;
	GtkWidget *myGauge;
	GtkWidget *myCompass;

	slider_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(window), box);

	boxh = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);

	myGauge = gisa_gauge_new();
	myCompass = gisa_compass_new();
	gtk_box_pack_start(GTK_BOX(box), boxh, TRUE, TRUE, 0);

	GtkAdjustment *scale1Adj = gtk_adjustment_new(0, 0, 100, 1, 5, 0);
	slider1 = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, scale1Adj);
	g_signal_connect(slider1, "value-changed", G_CALLBACK(gaugeTest), myGauge);
	// gtk_container_add(GTK_CONTAINER(slider_box), slider1);
	gtk_box_pack_start(GTK_BOX(slider_box), slider1, FALSE, TRUE, 0);

	GtkAdjustment *scale2Adj = gtk_adjustment_new(0, -360, 360, 1, 5, 0);
	slider2 = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, scale2Adj);
	g_signal_connect(slider2, "value-changed", G_CALLBACK(compassTest), myCompass);
	gtk_box_pack_start(GTK_BOX(slider_box), slider2, FALSE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(boxh), myGauge, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(boxh), myCompass, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), slider_box, TRUE, TRUE, 0);

	//style
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "/home/yogiastawan/Programming/C/GTK3CustomWidget/res/style/style.css", NULL);
	GtkStyleContext *ctx;
	ctx = gtk_widget_get_style_context(myGauge);
	gtk_style_context_add_provider(ctx, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gtk_widget_show_all(window);
}

void gaugeTest(GtkWidget *widget, gpointer *data)
{
	gisa_gauge_set_value(GISA_GAUGE(data), gtk_range_get_value(GTK_RANGE(widget)));
}

void compassTest(GtkWidget *widget, gpointer *data)
{
	gisa_compass_set_value(GISA_COMPASS(data), gtk_range_get_value(GTK_RANGE(widget)));
}