#include "gisagauge.h"
#include <math.h>

/* Properties enum*/
enum
{
    P_0,     // for padding
    P_VALUE, // for value
    P_MAX_VALUE,
    P_MIN_VALUE
};

/* Private data structure */
struct _GisaGaugePrivate
{
    gdouble value;
    gdouble maxValue;
    gdouble minvalue;
    GdkWindow *window;
};

/* Internal API */
static void gisa_gauge_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gisa_gauge_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void gisa_gauge_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void gisa_gauge_realize(GtkWidget *widget);
static gboolean gisa_gauge_draw(GtkWidget *widget, cairo_t *cr);
static void gisa_gauge_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height);
static void gisa_gauge_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width);

/* Define type */
// G_DEFINE_TYPE(GisaGauge, gisa_gauge, GTK_TYPE_WIDGET);//deprecated
G_DEFINE_TYPE_WITH_PRIVATE(GisaGauge, gisa_gauge, GTK_TYPE_WIDGET);

/* Initialization */
static void gisa_gauge_class_init(GisaGaugeClass *klass)
{
    GObjectClass *g_class;
    GtkWidgetClass *w_class;
    GParamSpec *pspec;
    GParamSpec *pspec_maxValue;
    GParamSpec *pspec_minValue;

    GParamSpec *colorstyle;

    g_class = G_OBJECT_CLASS(klass);
    /* Override widget class methods */
    g_class->set_property = gisa_gauge_set_property;
    g_class->get_property = gisa_gauge_get_property;

    w_class = GTK_WIDGET_CLASS(klass);
    /* Override widget class methods */
    w_class->realize = gisa_gauge_realize;
    w_class->get_preferred_height = gisa_gauge_get_preferred_height;
    w_class->get_preferred_width = gisa_gauge_get_preferred_width;
    // w_class->size_request=gisa_gauge_size_request;
    w_class->size_allocate = gisa_gauge_size_allocate;
    w_class->draw = gisa_gauge_draw;

    /* Install Property */
    pspec = g_param_spec_double("gisa-value", "Value", "Value will show", -G_MINDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_class, P_VALUE, pspec);
    pspec_maxValue = g_param_spec_double("gisa-max-value", "MaxValue", "Max value will hold", -G_MINDOUBLE, G_MAXDOUBLE, 100.0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_class, P_MAX_VALUE, pspec_maxValue);
    pspec_minValue = g_param_spec_double("gisa-min-value", "MinValue", "Min value will hold", -G_MINDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_class, P_MIN_VALUE, pspec_minValue);
    colorstyle = g_param_spec_boxed("gisa-gauge-color", "Color Gauge", "Gauge Color", GDK_TYPE_RGBA, G_PARAM_READABLE|G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, colorstyle);
    gtk_widget_class_set_css_name(w_class,"gisa-gauge");
}

static void gisa_gauge_init(GisaGauge *widget)
{
    GisaGaugePrivate *priv;
    // priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, GISA_TYPE_GAUGE, GisaGaugePrivate);//deprecated
    priv = gisa_gauge_get_instance_private(widget);
    gtk_widget_set_has_window(GTK_WIDGET(widget), TRUE);

    //set default value
    priv->value = 0;
    priv->maxValue = 100;
    priv->minvalue = 0;

    //create cache for faster access
    widget->priv = priv;
}

/* Override Methods */
static void gisa_gauge_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GisaGauge *widget = GISA_GAUGE(object);
    switch (prop_id)
    {
    case P_VALUE:
        gisa_gauge_set_value(widget, g_value_get_double(value));
        break;
    case P_MAX_VALUE:
        gisa_gauge_set_max_value(widget, g_value_get_double(value));
        break;
    case P_MIN_VALUE:
        gisa_gauge_set_min_value(widget, g_value_get_double(value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void gisa_gauge_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    GisaGauge *widget = GISA_GAUGE(object);
    switch (prop_id)
    {
    case P_VALUE:
        g_value_set_double(value, widget->priv->value);
        break;
    case P_MAX_VALUE:
        g_value_set_double(value, widget->priv->maxValue);
        break;
    case P_MIN_VALUE:
        g_value_set_double(value, widget->priv->minvalue);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void gisa_gauge_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    GisaGaugePrivate *priv = GISA_GAUGE(widget)->priv;
    gtk_widget_set_allocation(widget, allocation);
    if (gtk_widget_get_realized(widget))
    {
        gdk_window_move_resize(priv->window, allocation->x, allocation->y, allocation->width, allocation->height);
    }
}
static void gisa_gauge_realize(GtkWidget *widget)
{
    GisaGaugePrivate *priv = GISA_GAUGE(widget)->priv;
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
static gboolean gisa_gauge_draw(GtkWidget *widget, cairo_t *cr)
{
    GisaGaugePrivate *priv = GISA_GAUGE(widget)->priv;
    GtkAllocation alloc;
    guint size = 0;
    gtk_widget_get_allocation(widget, &alloc);

    if (alloc.height <= alloc.width)
    {
        size = alloc.height;
    }
    else
    {
        size = alloc.width;
    }
    // GValue *a;
    // gtk_widget_style_get_property(widget,"color-gauge",a);
    // g_value_get
    // g_print("linei: %s",);//2
    guint ro = (size - 2) / 2;
    guint ri = (size - 2) / 3;
    cairo_arc(cr, size / 2, size / 2, ro, 120 * G_PI / 180, 420 * G_PI / 180);
    cairo_arc(cr, (size / 2) - (((ro + ri) / 2) * sin(330 * G_PI / 180)), (size / 2) + (((ro + ri) / 2) * cos(330 * G_PI / 180)), (ro - ri) / 2, 60 * G_PI / 180, 240 * G_PI / 180);
    cairo_arc_negative(cr, size / 2, size / 2, ri, 60 * G_PI / 180, 120 * G_PI / 180);
    cairo_arc(cr, (size / 2) - (((ro + ri) / 2) * sin(30 * G_PI / 180)), (size / 2) + (((ro + ri) / 2) * cos(30 * G_PI / 180)), (ro - ri) / 2, 300 * G_PI / 180, 120 * G_PI / 180);
    cairo_close_path(cr);
    // cairo_set_source_rgb(cr,0,0.6,0.8);
    // cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke(cr);
    // value
    // gdouble deg=((3*priv->value)+30)*G_PI/180;
    if (priv->value > priv->minvalue)
    {
        // g_print("val: %f\n", priv->value);
        cairo_arc(cr, size / 2, size / 2, ro - 2, 120 * G_PI / 180, ((3 * priv->value) + 120) * G_PI / 180);
        cairo_arc(cr, (size / 2) - (((ro + ri) / 2) * sin(((3 * priv->value) + 30) * G_PI / 180)), (size / 2) + (((ro + ri) / 2) * cos(((3 * priv->value) + 30) * G_PI / 180)), (ro - ri - 4) / 2, (120 + (3 * priv->value)) * G_PI / 180, (300 + (3 * priv->value)) * G_PI / 180);
        cairo_arc_negative(cr, size / 2, size / 2, ri + 2, (120 + (3 * priv->value)) * G_PI / 180, 120 * G_PI / 180);
        cairo_arc(cr, (size / 2) - (((ro + ri) / 2) * sin(30 * G_PI / 180)), (size / 2) + (((ro + ri) / 2) * cos(30 * G_PI / 180)), (ro - ri - 4) / 2, 300 * G_PI / 180, 120 * G_PI / 180);
        cairo_close_path(cr);
        cairo_set_source_rgb(cr, 0, 0.6, 0.8);
        cairo_fill_preserve(cr);
        cairo_set_line_width(cr, 0);
    }
    return FALSE;
}
static void gisa_gauge_get_preferred_height(GtkWidget *widget, gint *minimum_height, gint *natural_height)
{
    (void)widget;
    *minimum_height = 150;
    *natural_height = 300;
}
static void gisa_gauge_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width)
{
    (void)widget;
    *minimum_width = 150;
    *natural_width = 300;
}

/* Public API */
GtkWidget *gisa_gauge_new(void)
{
    return (g_object_new(GISA_TYPE_GAUGE, NULL));
}

gdouble gisa_gauge_get_value(GisaGauge *widget)
{
    g_return_val_if_fail(GISA_IS_GAUGE(widget), 0);
    return (widget->priv->value);
}

void gisa_gauge_set_value(GisaGauge *widget, gdouble value)
{
    g_return_if_fail(GISA_IS_GAUGE(widget));
    if (value < widget->priv->minvalue)
    {
        value = widget->priv->minvalue;
    }
    else if (value > widget->priv->maxValue)
    {
        value = widget->priv->maxValue;
    }
    widget->priv->value = value;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}

void gisa_gauge_set_max_value(GisaGauge *widget, gdouble maxValue)
{
    g_return_if_fail(GISA_IS_GAUGE(widget));
    widget->priv->maxValue = maxValue;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}
gdouble gisa_gauge_get_max_value(GisaGauge *widget)
{
    g_return_val_if_fail(GISA_IS_GAUGE(widget), 0);
    return (widget->priv->maxValue);
}

void gisa_gauge_set_min_value(GisaGauge *widget, gdouble minValue)
{
    g_return_if_fail(GISA_IS_GAUGE(widget));
    widget->priv->minvalue = minValue;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}

gdouble gisa_gauge_get_min_value(GisaGauge *widget)
{
    g_return_val_if_fail(GISA_IS_GAUGE(widget), 0);
    return (widget->priv->minvalue);
}
