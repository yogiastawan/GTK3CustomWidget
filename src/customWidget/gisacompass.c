#include "gisacompass.h"
#include <math.h>

/* Properties enum*/
enum
{
    P_0,    // for padding
    P_VALUE // for value
};

/* Private data structure */
struct _GisaCompassPrivate
{
    gdouble value;
    GdkWindow *window;
};

/* Internal API */
static void gisa_compass_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gisa_compass_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void gisa_compass_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void gisa_compass_realize(GtkWidget *widget);
static gboolean gisa_compass_draw(GtkWidget *widget, cairo_t *cr);
static void gisa_compass_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height);
static void gisa_compass_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width);

/* Define type */
// G_DEFINE_TYPE(GisaCompass, gisa_compass, GTK_TYPE_WIDGET);//deprecated
G_DEFINE_TYPE_WITH_PRIVATE(GisaCompass, gisa_compass, GTK_TYPE_WIDGET);

/* Initialization */
static void gisa_compass_class_init(GisaCompassClass *klass)
{
    GObjectClass *g_class;
    GtkWidgetClass *w_class;
    GParamSpec *pspec;

    g_class = G_OBJECT_CLASS(klass);
    /* Override widget class methods */
    g_class->set_property = gisa_compass_set_property;
    g_class->get_property = gisa_compass_get_property;

    w_class = GTK_WIDGET_CLASS(klass);
    /* Override widget class methods */
    w_class->realize = gisa_compass_realize;
    w_class->get_preferred_height = gisa_compass_get_preferred_height;
    w_class->get_preferred_width = gisa_compass_get_preferred_width;
    // w_class->size_request=gisa_compass_size_request;
    w_class->size_allocate = gisa_compass_size_allocate;
    w_class->draw = gisa_compass_draw;

    /* Install Property */
    pspec=g_param_spec_double("value", "Value", "Value will show", 0, 360, 0, G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS);

    g_object_class_install_property(g_class, P_VALUE, pspec);
}

static void gisa_compass_init(GisaCompass *widget)
{
    GisaCompassPrivate *priv;
    // priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, GISA_TYPE_COMPASS, GisaCompassPrivate);//deprecated
    priv=gisa_compass_get_instance_private(widget);
    gtk_widget_set_has_window(GTK_WIDGET(widget), TRUE);

    //set default value
    priv->value = 0;

    //create cache for faster access
    widget->priv = priv;
}

/* Override Methods */
static void gisa_compass_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GisaCompass *widget = GISA_COMPASS(object);
    switch (prop_id)
    {
    case P_VALUE:
        gisa_compass_set_value(widget, g_value_get_double(value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void gisa_compass_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    GisaCompass *widget = GISA_COMPASS(object);
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
static void gisa_compass_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    GisaCompassPrivate *priv = GISA_COMPASS(widget)->priv;
    gtk_widget_set_allocation(widget, allocation);
    if (gtk_widget_get_realized(widget))
    {
        gdk_window_move_resize(priv->window, allocation->x, allocation->y, allocation->width, allocation->height);
    }
}
static void gisa_compass_realize(GtkWidget *widget)
{
    GisaCompassPrivate *priv = GISA_COMPASS(widget)->priv;
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
    attrs.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;

    attrs_mask = GDK_WA_X | GDK_WA_Y;

    priv->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attrs, attrs_mask);
    gdk_window_set_user_data(priv->window, widget);
    gtk_widget_set_window(widget, priv->window);
    //style
}
static gboolean gisa_compass_draw(GtkWidget *widget, cairo_t *cr)
{
    GisaCompassPrivate *priv = GISA_COMPASS(widget)->priv;
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    guint size;
    if (alloc.width<=alloc.height)
    {
        size=alloc.width;
    }
    else
    {
        size=alloc.height;
    }
    cairo_save(cr);
    cairo_arc(cr, size/2, size/2, (size-2)/2, 0, 2*G_PI);
    cairo_move_to(cr, (size/2)+((size-2)/3), size/2);
    cairo_arc(cr, size/2, size/2, (size-2)/3, 0, 2*G_PI);
    if (priv->value!=0)
    {
        //draw arc indicator   
    }
    //draw triangle 1
    //transform rotate
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke(cr);
    cairo_restore(cr);
    //draw triangle;
    cairo_move_to(cr, size/2, (size+4)/6);
    cairo_line_to(cr, (size/2)-(sin(G_PI/6)*size/8), ((size+4)/6)+(cos(G_PI/6)*size/8));
    cairo_line_to(cr, (size/2)+(sin(G_PI/6)*size/8), ((size+4)/6)+(cos(G_PI/6)*size/8));
    cairo_close_path(cr);
    if (priv->value==0)
    {
        cairo_set_source_rgb(cr, 0, 0, 1);
    }    
else
    {
        cairo_set_source_rgb(cr, 0.5, 1, 0);
    }

    cairo_fill(cr);



    return FALSE;
}
static void gisa_compass_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height)
{
    (void)widget;
    *minimum_height = 120;
    *natural_height = 300;
}
static void gisa_compass_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width)
{
    (void)widget;
    *minimum_width = 120;
    *natural_width = 300;
}

/* Public API */
GtkWidget *gisa_compass_new(void) {
    return (g_object_new(GISA_TYPE_COMPASS, NULL));
}

gdouble gisa_compass_get_value(GisaCompass *widget) {
    g_return_val_if_fail(GISA_IS_COMPASS(widget), 0);
    return (widget->priv->value);
}

void gisa_compass_set_value(GisaCompass *widget, gdouble value) {
    g_return_if_fail(GISA_IS_COMPASS(widget));
    widget->priv->value=value;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}