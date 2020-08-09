#ifndef __GISACOMPASS_H__
    #define __GISACOMPASS_H__
    
    #include <gtk/gtk.h>
    
    #include <cairo.h>//for drawing
    
    G_BEGIN_DECLS
    
    /* Standart GObject macros */
    #define GISA_TYPE_COMPASS (gisa_compass_get_type())
    #define GISA_COMPASS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GISA_TYPE_COMPASS, GisaCompass))
    #define GISA_COMPASS_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST((klass),GISA_TYPE_COMPASS, GisaCompassClass))
    #define GISA_IS_COMPASS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GISA_TYPE_COMPASS))
    #define GISA_IS_COMPASS_CLASS(klass) (G_TYPE_CHECk_CLASS_TYPE((klass),GISA_TYPE_COMPASS))
    #define GISA_COMPASS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj),GISA_TYPE_COMPASS, GisaCompassClass))
    
    /* Type definition */
    typedef struct _GisaCompass GisaCompass;
    typedef struct _GisaCompassClass GisaCompassClass;
    typedef struct _GisaCompassPrivate GisaCompassPrivate;
    
    struct _GisaCompass
    {
        GtkWidget parent;
        /*< Private >*/
        GisaCompassPrivate *priv;
    };
    
    struct _GisaCompassClass
    {
        GtkWidgetClass parent_class;
    };
    
    /* public API */
    GType gisa_compass_get_type(void) G_GNUC_CONST;
    GtkWidget *gisa_compass_new(void);
    
    //set value widget
    void gisa_compass_set_value(GisaCompass *widget, gdouble value);
    //get value widget
    gdouble gisa_compass_get_value(GisaCompass *widget);
    
    G_END_DECLS
    
    #endif /*__GISACOMPASS_H__*/