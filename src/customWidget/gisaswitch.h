#ifndef __GISASWITCH_H__
    #define __GISASWITCH_H__
    
    #include <gtk/gtk.h>
    
    #include <cairo.h>//for drawing
    
    G_BEGIN_DECLS
    
    /* Standart GObject macros */
    #define GISA_TYPE_SWITCH (gisa_switch_get_type())
    #define GISA_SWITCH(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GISA_TYPE_SWITCH, GisaSwitch))
    #define GISA_SWITCH_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST((klass),GISA_TYPE_SWITCH, GisaSwitchClass))
    #define GISA_IS_SWITCH(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GISA_TYPE_SWITCH))
    #define GISA_IS_SWITCH_CLASS(klass) (G_TYPE_CHECk_CLASS_TYPE((klass),GISA_TYPE_SWITCH))
    #define GISA_SWITCH_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj),GISA_TYPE_SWITCH, GisaSwitchClass))
    
    /* Type definition */
    typedef struct _GisaSwitch GisaSwitch;
    typedef struct _GisaSwitchClass GisaSwitchClass;
    typedef struct _GisaSwitchPrivate GisaSwitchPrivate;
    
    struct _GisaSwitch
    {
        GtkWidget parent;
        /*< Private >*/
        GisaSwitchPrivate *priv;

        guint8 button;
    };
    
    struct _GisaSwitchClass
    {
        GtkWidgetClass parent_class;
    };
    
    /* public API */
    GType gisa_switch_get_type(void) G_GNUC_CONST;
    GtkWidget *gisa_switch_new(void);
    
    //set value widget
    void gisa_switch_set_value(GisaSwitch *widget, gdouble value);
    //get value widget
    gdouble gisa_switch_get_value(GisaSwitch *widget);
    
    G_END_DECLS
    
    #endif /*__GISASWITCH_H__*/