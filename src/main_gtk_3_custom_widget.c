#include <gtk/gtk.h>

#include "./customWidget/gisagauge.h"

void increaseValue(GtkWidget *widget,gpointer *data);
void decreaseValue(GtkWidget *widget,gpointer *data);
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
	gtk_window_set_title(GTK_WINDOW(window), (gchar*)user_data);

	GtkWidget *button, *button2, *button_box, *box;
	GtkWidget *myGauge;	

	button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	box=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
	gtk_container_add(GTK_CONTAINER(window), box);

	myGauge=gisa_gauge_new();

	button = gtk_button_new_with_label("Increase");
	g_signal_connect(button, "clicked", G_CALLBACK(increaseValue), myGauge);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	button2=gtk_button_new_with_label("Decrease");
	g_signal_connect(button2,"clicked",G_CALLBACK(decreaseValue),myGauge);
	gtk_container_add(GTK_CONTAINER(button_box), button2);
	
	gtk_box_pack_start(GTK_BOX(box),myGauge,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(box),button_box,FALSE,FALSE,0);

	gtk_widget_show_all(window);
}

void increaseValue(GtkWidget *widget,gpointer *data)
{
	(void) widget;
	gisa_gauge_set_value(GISA_GAUGE(data),gisa_gauge_get_value(GISA_GAUGE(data))+5);
}

void decreaseValue(GtkWidget *widget,gpointer *data)
{
	(void) widget;
	gisa_gauge_set_value(GISA_GAUGE(data),gisa_gauge_get_value(GISA_GAUGE(data))-5);
}