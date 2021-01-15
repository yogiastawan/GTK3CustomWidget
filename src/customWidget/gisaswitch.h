#ifndef __GISASWITCH_H__
    #define __GISASWITCH_H__
    
    #include <gtk/gtk.h>
    
    #include <cairo.h>//for drawing
    
    G_BEGIN_DECLS
    
    typedef gboolean switchValue;
    #define ON TRUE
    #define OFF FALSE

    typedef guint8 switchConfig;
    enum{
        MANUAL,
        SWITCH_OFF,
        AUTO
    };
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
    };
    
    struct _GisaSwitchClass
    {
        GtkWidgetClass parent_class;
    };
    
    /* public API */
    GType gisa_switch_get_type(void) G_GNUC_CONST;
    GtkWidget *gisa_switch_new(void);
    
    //set value widget
    void gisa_switch_set_value(GisaSwitch *widget, switchValue value);
    //get value widget
    switchValue gisa_switch_get_value(GisaSwitch *widget);
    //set switch config
    void gisa_switch_set_configuration(GisaSwitch *widget, switchConfig config);
    //get switch config
    switchConfig gisa_switch_get_configuration(GisaSwitch *widget);
    
    G_END_DECLS
    
    #endif /*__GISASWITCH_H__*/