#include "gisaswitch.h"

#define GISA_RADIUS (size / 10)

/* Properties enum*/
enum
{
    P_0,    // for padding
    P_VALUE // for value
};

/* Private data structure */
struct _GisaSwitchPrivate
{
    gdouble value;
    GdkWindow *window;
};

/* Internal API */
static void gisa_switch_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gisa_switch_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void gisa_switch_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void gisa_switch_realize(GtkWidget *widget);
static gboolean gisa_switch_draw(GtkWidget *widget, cairo_t *cr);
static void gisa_switch_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height);
static void gisa_switch_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width);

static gboolean gisa_switch_button_press(GtkWidget *widget, GdkEventButton *event);
static gboolean gisa_switch_button_release(GtkWidget *widget, GdkEventButton *event);

/* Define type */
// G_DEFINE_TYPE(GisaSwitch, gisa_switch, GTK_TYPE_WIDGET);//deprecated
G_DEFINE_TYPE_WITH_PRIVATE(GisaSwitch, gisa_switch, GTK_TYPE_WIDGET);

/* Initialization */
static void gisa_switch_class_init(GisaSwitchClass *klass)
{
    GObjectClass *g_class;
    GtkWidgetClass *w_class;
    GParamSpec *pspec;

    g_class = G_OBJECT_CLASS(klass);
    /* Override widget class methods */
    g_class->set_property = gisa_switch_set_property;
    g_class->get_property = gisa_switch_get_property;

    w_class = GTK_WIDGET_CLASS(klass);
    /* Override widget class methods */
    w_class->realize = gisa_switch_realize;
    w_class->get_preferred_height = gisa_switch_get_preferred_height;
    w_class->get_preferred_width = gisa_switch_get_preferred_width;
    // w_class->size_request=gisa_switch_size_request;
    w_class->size_allocate = gisa_switch_size_allocate;
    w_class->draw = gisa_switch_draw;

    //press event
    w_class->button_press_event = gisa_switch_button_press;
    w_class->button_release_event = gisa_switch_button_release;

    /* Install Property */
    pspec = g_param_spec_double("value", "Value", "Value will show", 0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    g_object_class_install_property(g_class, P_VALUE, pspec);
}

static void gisa_switch_init(GisaSwitch *widget)
{
    GisaSwitchPrivate *priv;
    // priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, GISA_TYPE_SWITCH, GisaSwitchPrivate);//deprecated
    priv = gisa_switch_get_instance_private(widget);
    gtk_widget_set_has_window(GTK_WIDGET(widget), TRUE);

    //set default value
    priv->value = 0;

    //create cache for faster access
    widget->priv = priv;
    widget->button = 0;
}

/* Override Methods */
static void gisa_switch_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GisaSwitch *widget = GISA_SWITCH(object);
    switch (prop_id)
    {
    case P_VALUE:
        gisa_switch_set_value(widget, g_value_get_double(value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void gisa_switch_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    GisaSwitch *widget = GISA_SWITCH(object);
    switch (prop_id)
    {
    case P_VALUE:
        g_value_set_double(value, widget->priv->value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void gisa_switch_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    GisaSwitchPrivate *priv = GISA_SWITCH(widget)->priv;
    gtk_widget_set_allocation(widget, allocation);
    if (gtk_widget_get_realized(widget))
    {
        gdk_window_move_resize(priv->window, allocation->x, allocation->y, allocation->width, allocation->height);
    }
}
static void gisa_switch_realize(GtkWidget *widget)
{
    GisaSwitchPrivate *priv = GISA_SWITCH(widget)->priv;
    GtkAllocation alloc;
    GdkWindowAttr attrs;
    gint attrs_mask;

    gtk_widget_set_realized(widget, TRUE);
    gtk_widget_get_allocation(widget, &alloc);
    attrs.x = alloc.x;
    attrs.y = alloc.y;
    attrs.width = alloc.width;
    attrs.height = alloc.height;
    attrs.window_type = GDK_WINDOW_CHILD;
    attrs.wclass = GDK_INPUT_OUTPUT;
    attrs.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK;

    attrs_mask = GDK_WA_X | GDK_WA_Y;

    priv->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attrs, attrs_mask);
    gdk_window_set_user_data(priv->window, widget);
    gtk_widget_set_window(widget, priv->window);
    //style
}
static gboolean gisa_switch_draw(GtkWidget *widget, cairo_t *cr)
{
    GisaSwitchPrivate *priv = GISA_SWITCH(widget)->priv;
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    guint size = 0 * priv->value;
    if (alloc.height <= alloc.width)
    {
        size = alloc.height;
    }
    else
    {
        size = alloc.width;
    }
    //draw border
    guint radius = size / 10;
    cairo_arc(cr, GISA_RADIUS + 1, GISA_RADIUS + 1, GISA_RADIUS, 180 * G_PI / 180, 270 * G_PI / 180);
    cairo_arc(cr, size - (GISA_RADIUS + 1), GISA_RADIUS + 1, GISA_RADIUS, -90 * G_PI / 180, 0);
    cairo_arc(cr, size - (GISA_RADIUS + 1), size - (GISA_RADIUS + 1), GISA_RADIUS, 0, 90 * G_PI / 180);
    cairo_arc(cr, GISA_RADIUS + 1, size - (GISA_RADIUS + 1), radius, 90 * G_PI / 180, 180 * G_PI / 180);
    cairo_close_path(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_stroke(cr);
    //draw icon
    cairo_arc(cr, size / 2, size / 2, size * 3 / 8, 0, 2 * G_PI);
    //draw sub arc
    cairo_arc(cr, size / 2, size / 2, size * 1 / 4, 0, 2 * G_PI);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_stroke(cr);
    return FALSE;
}
static void gisa_switch_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height)
{
    (void)widget;
    *minimum_height = 90;
    *natural_height = 120;
}
static void gisa_switch_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width)
{
    (void)widget;
    *minimum_width = 90;
    *natural_width = 120;
}

/* Public API */
GtkWidget *gisa_switch_new(void)
{
    return (g_object_new(GISA_TYPE_SWITCH, NULL));
}

gdouble gisa_switch_get_value(GisaSwitch *widget)
{
    g_return_val_if_fail(GISA_IS_SWITCH(widget), 0);
    return (widget->priv->value);
}

void gisa_switch_set_value(GisaSwitch *widget, gdouble value)
{
    g_return_if_fail(GISA_IS_SWITCH(widget));
    widget->priv->value = value;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}

gboolean gisa_switch_button_press(GtkWidget *widget, GdkEventButton *event)
{
    g_return_val_if_fail(GISA_IS_SWITCH(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    if (!GISA_SWITCH(widget)->button)
    {
        gtk_grab_add(widget);
        GISA_SWITCH(widget)->button = event->button;
        g_print("event: %d || GisaSwitch: %d", event->button, GISA_SWITCH(widget)->button);
    }

    return FALSE;
}

static gboolean gisa_switch_button_release(GtkWidget *widget, GdkEventButton *event)
{
    g_return_val_if_fail(GISA_IS_SWITCH(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);
    if (GISA_SWITCH(widget)->button == event->button)
    {
        gtk_grab_remove(widget);
        g_print("Button released");
        GISA_SWITCH(widget)->button = 0;
    }
    return FALSE;
}