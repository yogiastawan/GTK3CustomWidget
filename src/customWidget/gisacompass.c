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
    GParamSpec *styleSpec;

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
    pspec = g_param_spec_double("value", "Value", "Value will show", -360, 360, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_class, P_VALUE, pspec);

    /* Install Style Property */
    styleSpec = g_param_spec_boxed("base-fill-color", "Base Fill Color", "Fill color of base", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("base-stroke-color", "Base stroke Color", "Stroke color of base", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("tick-normal-color", "Tick Normal Color", "Color of tick normal", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("tick-big-color", "Tick Big Color", "Color of tick big", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("tick-char-color", "Tick Char Color", "Color of tick char", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("tick-highlight-color", "Tick Highlight Color", "Color of tick when highlight", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("delta-curve-color", "Delta Curve Color", "Color of curve delta", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("north-indicator-color", "North Indicator Color", "Color of north indicator", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("direction-indicator-color", "Direction Indicator Color", "Color of direction indicator", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("direction-indicator-highlight-color", "Direction Indicator Highlight Color", "Color of direction indicator when degree value not 0", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    styleSpec = g_param_spec_boxed("value-text-color", "Value Text Color", "Color of text value", GDK_TYPE_RGBA, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    gtk_widget_class_install_style_property(w_class, styleSpec);
    gtk_widget_class_set_css_name(w_class, "gisa-compass");
}

static void gisa_compass_init(GisaCompass *widget)
{
    GisaCompassPrivate *priv;
    // priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, GISA_TYPE_COMPASS, GisaCompassPrivate);//deprecated
    priv = gisa_compass_get_instance_private(widget);
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
    char *lbl[] = {"N", "E", "S", "W"};
    guint size;
    if (alloc.width <= alloc.height)
    {
        size = alloc.width;
    }
    else
    {
        size = alloc.height;
    }

    if (priv->value == -360 || priv->value == 360)
    {
        priv->value = 0;
    }

    GdkRGBA *color;
    GValue style = G_VALUE_INIT;

    cairo_save(cr);
    //transform rotate
    cairo_translate(cr, size / 2, size / 2);
    cairo_rotate(cr, -priv->value * G_PI / 180);
    cairo_translate(cr, (double)-1 * size / 2, (double)-1 * size / 2);

    cairo_save(cr);
    cairo_arc(cr, size / 2, size / 2, (size - 2) / 2, 0, 2 * G_PI);
    cairo_move_to(cr, (size / 2) + ((size - 2) / 3), size / 2);
    cairo_arc(cr, size / 2, size / 2, (size - 2) / 3, 0, 2 * G_PI);
    g_value_init(&style, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "base-fill-color", &style);
    if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
    {
        color = g_value_get_boxed(&style);
    }
    if (color != NULL)
    {
        gdk_cairo_set_source_rgba(cr, color);
    }
    else
    {
        cairo_set_source_rgba(cr, 1, 1, 1, 1);
    }
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_fill_preserve(cr);
    g_value_unset(&style);
    g_value_init(&style, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "base-stroke-color", &style);
    if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
    {
        color = g_value_get_boxed(&style);
    }
    if (color != NULL)
    {
        gdk_cairo_set_source_rgba(cr, color);
    }
    else
    {
        cairo_set_source_rgba(cr, 0, 0, 0, 1);
    }
    cairo_stroke(cr);
    cairo_restore(cr);
    //draw tick
    GValue tickNormal = G_VALUE_INIT;
    GValue tickBig = G_VALUE_INIT;
    GValue tickHighlight = G_VALUE_INIT;
    GdkRGBA *colorNormal = NULL, *colorBig = NULL, *colorHigh = NULL;
    //get color style
    g_value_unset(&style);
    g_value_init(&style, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "tick-char-color", &style);
    if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
    {
        color = g_value_get_boxed(&style);
    }
    g_value_init(&tickNormal, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "tick-normal-color", &tickNormal);
    if (G_VALUE_HOLDS(&tickNormal, GDK_TYPE_RGBA))
    {
        colorNormal = g_value_get_boxed(&tickNormal);
    }
    g_value_init(&tickBig, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "tick-big-color", &tickBig);
    if (G_VALUE_HOLDS(&tickBig, GDK_TYPE_RGBA))
    {
        colorBig = g_value_get_boxed(&tickBig);
    }
    g_value_init(&tickHighlight, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "tick-highlight-color", &tickHighlight);
    if (G_VALUE_HOLDS(&tickNormal, GDK_TYPE_RGBA))
    {
        colorHigh = g_value_get_boxed(&tickHighlight);
    }
    gint16 i;
    for (i = 0; i < 144; i++)
    {
        if (i % 36 == 0)
        {
            cairo_save(cr);
            cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_text_extents_t text_extents;
            cairo_set_font_size(cr, size / 15);
            cairo_text_extents(cr, lbl[i / 36], &text_extents);
            gdouble cx = (size / 2) + (((5 * size) / 12) * sin(i * 2.5 * G_PI / 180));
            gdouble cy = (size / 2) - (((5 * size) / 12) * cos(i * 2.5 * G_PI / 180));
            cairo_translate(cr, cx, cy);
            cairo_rotate(cr, i * 2.5 * G_PI / 180);
            cairo_translate(cr, -cx, -cy);
            cairo_move_to(cr, cx - ((text_extents.width / 2) + text_extents.x_bearing), cy - ((text_extents.height / 2) + text_extents.y_bearing));
            // cairo_show_text(cr, lbl[i / 36]);
            cairo_text_path(cr, lbl[i / 36]);
            gdouble abs = priv->value;
            if (priv->value < 0)
            {
                abs = 360 + priv->value;
            }

            if (abs == (double)i * 2.5)
            {
                if (colorHigh != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, colorHigh);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.9, 0.5, 0, 1);
                }
            }
            else
            {
                if (color != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, color);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.3, 0.3, 0.3, 1);
                }
            }
            cairo_fill(cr);
            cairo_restore(cr);
        }
        else if (i % 36 == 35 || i % 36 == 1)
        {
            // do not draw to make more space for label N,E,S,W
        }

        else if (i % 6 == 0 && i > 0)
        {
            //draw bigger tick (every 15 deg)
            cairo_save(cr);
            cairo_move_to(cr, (size / 2) + ((((size - 2) / 2) - size / 30) * sin(i * 2.5 * G_PI / 180)), (size / 2) - ((((size - 2) / 2) - size / 30) * cos(i * 2.5 * G_PI / 180)));
            cairo_line_to(cr, (size / 2) + ((((size - 2) / 3) + size / 30) * sin(i * 2.5 * G_PI / 180)), (size / 2) - ((((size - 2) / 3) + size / 30) * cos(i * 2.5 * G_PI / 180)));
            gdouble abs = priv->value;
            if (priv->value < 0)
            {
                abs = 360 + priv->value;
            }
            if (abs == (double)i * 2.5)
            {
                cairo_set_line_width(cr, 1.5);
                if (colorHigh != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, colorHigh);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.9, 0.5, 0, 1);
                }
            }
            else
            {
                cairo_set_line_width(cr, 1);
                if (colorBig != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, colorBig);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.3, 0.3, 0.3, 1);
                }
            }
            cairo_stroke(cr);
            cairo_restore(cr);
        }
        else
        {
            //draw tick normal
            cairo_save(cr);
            cairo_move_to(cr, (size / 2) + ((((size - 2) / 2) - size / 20) * sin(i * 2.5 * G_PI / 180)), (size / 2) - ((((size - 2) / 2) - size / 20) * cos(i * 2.5 * G_PI / 180)));
            cairo_line_to(cr, (size / 2) + ((((size - 2) / 3) + size / 20) * sin(i * 2.5 * G_PI / 180)), (size / 2) - ((((size - 2) / 3) + size / 20) * cos(i * 2.5 * G_PI / 180)));
            gdouble abs = priv->value;
            if (priv->value < 0)
            {
                abs = 360 + priv->value;
            }

            if (abs == (double)i * 2.5)
            {
                cairo_set_line_width(cr, 1.5);
                if (colorHigh != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, colorHigh);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.9, 0.5, 0, 1);
                }
            }
            else
            {
                cairo_set_line_width(cr, 1);
                if (colorNormal != NULL)
                {
                    gdk_cairo_set_source_rgba(cr, colorNormal);
                }
                else
                {
                    cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1);
                }
            }

            cairo_stroke(cr);
            cairo_restore(cr);
        }
    }
    if (priv->value != 0)
    {
        cairo_save(cr);
        if (priv->value > 0 && priv->value <= 180)
        {
            cairo_arc(cr, size / 2, size / 2, (size - 2) / 3, 3 * G_PI / 2, (270 + priv->value) * G_PI / 180);
        }
        else if (priv->value > 0 && priv->value > 180)
        {
            cairo_arc_negative(cr, size / 2, size / 2, (size - 2) / 3, 3 * G_PI / 2, (priv->value - 90) * G_PI / 180);
        }
        else if (priv->value < 0 && priv->value >= -180)
        {
            cairo_arc_negative(cr, size / 2, size / 2, (size - 2) / 3, 3 * G_PI / 2, (270 + priv->value) * G_PI / 180);
        }
        else if (priv->value < 0 && priv->value < -180)
        {
            cairo_arc(cr, size / 2, size / 2, (size - 2) / 3, 3 * G_PI / 2, (priv->value + 270) * G_PI / 180);
        }
        cairo_set_line_width(cr, 1.5);
        g_value_unset(&style);
        g_value_init(&style, GDK_TYPE_RGBA);
        gtk_widget_style_get_property(widget, "delta-curve-color", &style);
        if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
        {
            color = g_value_get_boxed(&style);
        }
        if (color != NULL)
        {
            gdk_cairo_set_source_rgba(cr, color);
        }
        else
        {
            cairo_set_source_rgba(cr, 0.9, 0, 0.5, 1);
        }
        cairo_stroke(cr);
        cairo_restore(cr);

        //draw triangle 1
        cairo_save(cr);
        cairo_move_to(cr, size / 2, (size + 4) / 6);
        cairo_line_to(cr, (size / 2) - (sin(G_PI / 6) * size / 10), ((size + 4) / 6) + (cos(G_PI / 6) * size / 10));
        cairo_line_to(cr, (size / 2) + (sin(G_PI / 6) * size / 10), ((size + 4) / 6) + (cos(G_PI / 6) * size / 10));
        cairo_close_path(cr);
        g_value_unset(&style);
        g_value_init(&style, GDK_TYPE_RGBA);
        gtk_widget_style_get_property(widget, "north-indicator-color", &style);
        if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
        {
            color = g_value_get_boxed(&style);
        }
        if (color != NULL)
        {
            gdk_cairo_set_source_rgba(cr, color);
        }
        else
        {
            cairo_set_source_rgb(cr, 0, 0.6, 0.8);
        }
        cairo_fill(cr);
        cairo_restore(cr);
    }

    cairo_restore(cr);
    //draw triangle;
    cairo_move_to(cr, size / 2, (size + 4) / 6);
    cairo_line_to(cr, (size / 2) - (sin(G_PI / 6) * size / 8), ((size + 4) / 6) + (cos(G_PI / 6) * size / 8));
    cairo_line_to(cr, (size / 2) + (sin(G_PI / 6) * size / 8), ((size + 4) / 6) + (cos(G_PI / 6) * size / 8));
    cairo_close_path(cr);
    if (priv->value == 0)
    {
        g_value_unset(&style);
        g_value_init(&style, GDK_TYPE_RGBA);
        gtk_widget_style_get_property(widget, "direction-indicator-color", &style);
        if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
        {
            color = g_value_get_boxed(&style);
        }
        if (color != NULL)
        {
            gdk_cairo_set_source_rgba(cr, color);
        }
        else
        {
            cairo_set_source_rgb(cr, 0, 0.6, 0.8);
        }
    }
    else
    {
        g_value_unset(&style);
        g_value_init(&style, GDK_TYPE_RGBA);
        gtk_widget_style_get_property(widget, "direction-indicator-highlight-color", &style);
        if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
        {
            color = g_value_get_boxed(&style);
        }
        if (color != NULL)
        {
            gdk_cairo_set_source_rgba(cr, color);
        }
        else
        {
            cairo_set_source_rgb(cr, 0.8, 0, 0.6);
        }
    }

    cairo_fill(cr);

    //draw text value
    cairo_text_extents_t extent;
    char *val;
    val = (char *)malloc(sizeof(char) * 8);
    sprintf(val, "%.1f\302\260", priv->value);
    cairo_select_font_face(cr, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36 * size / 300);
    cairo_text_extents(cr, val, &extent);
    cairo_move_to(cr, (size / 2) - ((extent.width / 2) + extent.x_bearing), (size / 2) - ((extent.height / 2) + extent.y_bearing));
    cairo_text_path(cr, val);
    free(val);
    g_value_unset(&style);
    g_value_init(&style, GDK_TYPE_RGBA);
    gtk_widget_style_get_property(widget, "value-text-color", &style);
    if (G_VALUE_HOLDS(&style, GDK_TYPE_RGBA))
    {
        color = g_value_get_boxed(&style);
    }
    if (color != NULL)
    {
        gdk_cairo_set_source_rgba(cr, color);
    }
    else
    {
        cairo_set_source_rgba(cr, 0.3, 0.3, 0.3, 1);
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
GtkWidget *gisa_compass_new(void)
{
    return (g_object_new(GISA_TYPE_COMPASS, NULL));
}

gdouble gisa_compass_get_value(GisaCompass *widget)
{
    g_return_val_if_fail(GISA_IS_COMPASS(widget), 0);
    return (widget->priv->value);
}

void gisa_compass_set_value(GisaCompass *widget, gdouble value)
{
    g_return_if_fail(GISA_IS_COMPASS(widget));
    widget->priv->value = value;
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}