                                                                                                    /*
NOTE 1:
======
For d_area, "size-allocate" is emitted before "expose-event", as the GdkWindow is still not
available. So the first "expose-event" callback connect "size-allocate".

NOTE 2:
======
The first "expose-event" is used too for drawing the data, else d_area remains empty.
The further "expose-event" are used to redraw the data because some actions ( ex:changing a button
label ) erase parts of d_area.

NOTE 3:
======
We want to know, when a resize occurs, what borders were moved. For that, the size (w,h) is not
enough. We need the (x,y) position of the GdkWindow too.

1) But we cant just use gdk_window_get_origin(d_area), because when moving the whole app window, no
"configure-event" is emitted for d_area. So we use the main app window "configure-event".

2) We have to filter spurious "configure-event", so we have to keep old/new vars for x,y,w,h.

NOTE 4:
======
The "...first" callbacks are used too for initalizing the new/old var couples.

NOTE 5:
======
When d_area is beeing resized, we dont want to redraw everything if the widget's size has only
slightly changed ( some pixels for example ).The purpose of the timeout is to redraw only after
an amout of time when d_area is beeing resized.

Gda::configure
Gda::expose-event first
    signal
    signal
Gda::configure-main-widget-first
Gda::configure-main-widget
    event [13] [0x560bf6074360] [1] [2730] [447] [300] [200]
Gda::configure-main-widget:no pos/dim change





                                                                                                    */
#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKW(FORMAT, ...)    ERGC_TKW(FORMAT, __VA_ARGS__);
#define     TKE(FORMAT, ...)    ERGC_TKE(FORMAT, __VA_ARGS__);
//#define     TKI(FORMAT, ...)

#include    <gdk/gdkkeysyms.h>

#include    "signal-display.hh"
#include    "signal-data.hh"
#include    "signal-control.hh"

namespace erg
{
namespace signal
{

size_t              Display::s_uid          =   0;

Display*
Display::New(
    signal::Model       *   _i_model                        ,
    signal::Control     *   _i_control                      ,
    GtkWidget           *   _i_main_widget                  ,
    guint                   _i_smart_redim_timeout_interval ,
    guint                   _i_smart_redin_timeout_redraw   ,
    Model::sDisplayAtt  *   _i_att                          )
{
    s_uid++;

    if ( _i_main_widget )
    {
        return new Display(_i_model, _i_control, _i_main_widget, s_uid, _i_smart_redim_timeout_interval, _i_smart_redin_timeout_redraw, _i_att);
    }
    else
    {
        return new Display(_i_model, _i_control, s_uid, _i_smart_redim_timeout_interval, _i_smart_redin_timeout_redraw, _i_att);
    }
}

Display::Display(
    signal::Model       *   _i_model                        ,
    signal::Control     *   _i_control                      ,
    size_t                  _i_uid                          ,
    guint                   _i_smart_redim_timeout_interval ,
    guint                   _i_smart_redin_timeout_redraw   ,
    Model::sDisplayAtt  *   _i_att                          )
    :   MemberModel(_i_model), MemberControl(_i_control)
{
    a_uid                           =   _i_uid;
    a_smart_redim_timeout_interval  =   _i_smart_redim_timeout_interval;
    a_smart_redim_timeout_redraw    =   _i_smart_redin_timeout_redraw;

    a_container     =   nullptr;
    d_container     = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    a_att           =   _i_att;

    construct();

    gtk_container_add(GTK_CONTAINER(cnt()), wgt());

    gtk_widget_show_all(cnt());
}

Display::Display(
    signal::Model       *   _i_model                        ,
    signal::Control     *   _i_control                      ,
    GtkWidget           *   _i_main_widget                  ,
    size_t                  _i_uid                          ,
    guint                   _i_smart_redim_timeout_interval ,
    guint                   _i_smart_redin_timeout_redraw   ,
    Model::sDisplayAtt  *   _i_att                          )
    :   MemberModel(_i_model), MemberControl(_i_control)
{
    a_uid                           =   _i_uid;
    a_smart_redim_timeout_interval  =   _i_smart_redim_timeout_interval;
    a_smart_redim_timeout_redraw    =   _i_smart_redin_timeout_redraw;

    a_container     =   _i_main_widget;
    d_container     =   nullptr;

    a_att           =   _i_att;

    construct();
}

void
Display::construct()
{
    a_smart_redim_timeout_gtimeout  =   0;
    a_smart_redim_timeout_runned    =   0;

    a_redraw.set(true, false);
    a_redraw.axes = true;

    a_mouse.motion.pos.x    =   0;
    a_mouse.motion.pos.y    =   0;
    //  ............................................................................................
    d_area  =   gtk_drawing_area_new();

    d_crs_1     =   nullptr;
    d_crs_2     =   nullptr;
    d_crs_3     =   nullptr;
    d_ctx       =   nullptr;
    d_ctx_1     =   nullptr;
    d_ctx_2     =   nullptr;
    d_ctx_3     =   nullptr;

    a_mouse.motion.dragging =   false;
    a_key_shift_any         =   false;
    a_key_control_any       =   false;

    a_select_mark_v.v1_set  =   false;
    a_select_mark_v.v2_set  =   false;
    a_select_mark_h.v1_set  =   false;
    a_select_mark_h.v2_set  =   false;

    construct_menu();

   /*
    g_signal_connect                    (
        a_ctx_menu_uim      ,
        "add-widget"        ,
        G_CALLBACK
        (
            +[] (                                                                                   // my first-ever lambda :)
                    GtkUIManager    * _i_uim    ,
                    GtkWidget       * _i_wgt    ,
                    void            * _i_data   )
            {
                printf("add\n");
                gtk_menu_shell_append( GTK_MENU_SHELL(_i_data), _i_wgt );
                gtk_widget_show_all(GTK_WIDGET(_i_data));
            }
        )                   ,
        a_ctx_menu_popup    );
    */
    //  ............................................................................................
    //gtk_widget_set_double_buffered(d_area, FALSE);                                                //  conflicts with cairo context optims
    gtk_widget_add_events (d_area, GDK_POINTER_MOTION_MASK  );
    gtk_widget_add_events (d_area, GDK_BUTTON_PRESS_MASK    );
    gtk_widget_add_events (d_area, GDK_BUTTON_RELEASE_MASK  );

    a_handler__configure_event_main_widget  =   g_signal_connect( G_OBJECT(cnt())           , "configure-event"     , (GCallback)GtkEvent__configure_main_widget_first  , (gpointer)this );
    a_handler__configure_event              =   g_signal_connect( G_OBJECT(d_area)          , "configure-event"     , (GCallback)GtkEvent__configure                    , (gpointer)this );
    a_handler__expose_event                 =   g_signal_connect( G_OBJECT(d_area)          , "expose-event"        , (GCallback)GtkEvent__expose_event_first           , (gpointer)this );

    a_handler__motion_notify_event          =   g_signal_connect( G_OBJECT(d_area)          , "motion-notify-event" , (GCallback)GtkEvent__motion_notify_event          , (gpointer)this );
    a_handler__event_button_press           =   g_signal_connect( G_OBJECT(d_area)          , "button-press-event"  , (GCallback)GtkEvent__button_press_event           , (gpointer)this );
    a_handler__event_button_release         =   g_signal_connect( G_OBJECT(d_area)          , "button-release-event", (GCallback)GtkEvent__button_release_event         , (gpointer)this );

                                                g_signal_connect( G_OBJECT(cnt())           , "key_press_event"     , (GCallback)GtkEvent__key_press_event              , (gpointer)this );
                                                g_signal_connect( G_OBJECT(cnt())           , "key_release_event"   , (GCallback)GtkEvent__key_release_event            , (gpointer)this );
    //  ............................................................................................
}
void
Display::construct_menu()
{
    //  popup
    GError  *   gerror = nullptr;

    a_ctx_menu_popup    =   gtk_menu_new();
    //a_ctx_menu_vbox     =   gtk_vbox_new(FALSE, 5);
    //gtk_container_add( GTK_CONTAINER(a_ctx_menu_popup), a_ctx_menu_vbox);

    a_ctx_menu_uim      =   gtk_ui_manager_new ();
    a_ctx_menu_group    =   gtk_action_group_new( "menuActionGroup" );

    a_ctx_menu_desc.assign(     "                                                                   \
    <ui>                                                                                            \
      <popup name=\"ctx-popup\" action=\"act-ctx-popup\">                                           \
        <menuitem action=\"act-mi-01\" />                                                           \
        <menu action=\"act-me-01\">                                                                 \
          <menuitem action=\"act-mi-10\" />                                                         \
          <menuitem action=\"act-mi-11\" />                                                         \
          <menuitem action=\"act-mi-12\" />                                                         \
        </menu>                                                                                     \
      </popup>                                                                                      \
    </ui>                       ");

    a_ctx_menu_entries[0]   =   { "act-ctx-popup"   , NULL          , "popup"           ,  NULL             , NULL      , NULL                      };
    a_ctx_menu_entries[1]   =   { "act-mi-01"       , GTK_STOCK_NEW , "option01"        , "<Control>A"      , "About"   , G_CALLBACK(NULL)          };
    a_ctx_menu_entries[2]   =   { "act-me-01"       , NULL          , "selection..."    ,  NULL             , NULL      , NULL                      };
    a_ctx_menu_entries[3]   =   { "act-mi-10"       , NULL          , "zoom to"         , "<Control>Z"      , "About"   , G_CALLBACK(NULL)          };
    a_ctx_menu_entries[4]   =   { "act-mi-11"       , NULL          , "Fourier"         , "<Control>A"      , "About"   , G_CALLBACK(NULL)          };
    a_ctx_menu_entries[5]   =   { "act-mi-12"       , NULL          , "discard"         , "<Control>A"      , "About"   , G_CALLBACK(NULL)          };

    gtk_action_group_add_actions        ( a_ctx_menu_group  , a_ctx_menu_entries, G_N_ELEMENTS(a_ctx_menu_entries), NULL);
    gtk_ui_manager_insert_action_group  ( a_ctx_menu_uim    , a_ctx_menu_group          , 0);
    gtk_ui_manager_add_ui_from_string   ( a_ctx_menu_uim    , a_ctx_menu_desc.c_str()   , -1, &gerror);

    if (gerror)
    {
        TKE("%s\n", gerror->message);
    }
 }
//  ################################################################################################
gboolean
Display::GtkEvent__configure_main_widget_first(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::configure-main-widget-first");

    return THIS->gtkevent__configure_main_widget_first( (GdkEventConfigure*)_i_event );
}
gboolean
Display::GtkEvent__configure_main_widget(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::configure-main-widget");

    return THIS->gtkevent__configure_main_widget( (GdkEventConfigure*)_i_event );
}
//  ************************************************************************************************
gboolean
Display::GtkEvent__size_allocate(
    GtkWidget    *  _i_widget       ,
    GdkRectangle *  _i_allocation   ,
    gpointer        _i_data         )
{
    signal::Display  *   THIS    = (signal::Display*)_i_data;
    //TKI("%s\n", "Gda::allocate");
    return THIS->gtkevent__size_allocate(_i_allocation);
}
//  ************************************************************************************************
gboolean
Display::GtkEvent__expose_event_first(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::expose-event first");

    return THIS->gtkevent__expose_event_first( (GdkEventExpose*)_i_event );
}
gboolean
Display::GtkEvent__expose_event(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::expose-event");

    return THIS->gtkevent__expose_event( (GdkEventExpose*)_i_event );
}
//  ************************************************************************************************
gboolean
Display::GtkEvent__configure(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::configure");

    return THIS->gtkevent__configure( (GdkEventConfigure*)_i_event );
}
//  ************************************************************************************************
//gboolean
//Display::GtkEvent__motion_notify_event_first(
//    GtkWidget   *   _i_widget   ,
//    GdkEvent    *   _i_event    ,
//    gpointer        _i_data     )
//{
//    signal::Display  *   THIS = (signal::Display*)_i_data;
//
//    TKI("%s\n", "Gda::motion-notify-event first");
//
//    return THIS->gtkevent__motion_notify_event_first( (GdkEventMotion*)_i_event );
//}
gboolean
Display::GtkEvent__motion_notify_event(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::motion-notify-event");

    return THIS->gtkevent__motion_notify_event( (GdkEventMotion*)_i_event );
}
//  ************************************************************************************************
gboolean
Display::GtkEvent__button_press_event(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::button-press-event");

    return THIS->gtkevent__button_press_event( (GdkEventButton*)_i_event );
}
gboolean
Display::GtkEvent__button_release_event(
    GtkWidget   *   _i_widget   ,
    GdkEvent    *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    //TKI("%s\n", "Gda::button-release-event");

    return THIS->gtkevent__button_release_event( (GdkEventButton*)_i_event );
}
//  ************************************************************************************************
gboolean
Display::GtkEvent__key_press_event(
    GtkWidget   *   _i_widget   ,
    GdkEventKey *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;
    //  ............................................................................................
    if ( ( _i_event->keyval == GDK_KEY_Shift_L ) || ( _i_event->keyval == GDK_KEY_Shift_R ) )
    {
        THIS->a_key_shift_any = true;
    }
    if ( ( _i_event->keyval == GDK_KEY_Control_L ) || ( _i_event->keyval == GDK_KEY_Control_R ) )
    {
        THIS->a_key_control_any = true;
    }

    return FALSE;
}
gboolean
Display::GtkEvent__key_release_event(
    GtkWidget   *   _i_widget   ,
    GdkEventKey *   _i_event    ,
    gpointer        _i_data     )
{
    signal::Display  *   THIS = (signal::Display*)_i_data;
    //  ............................................................................................
    if ( ( _i_event->keyval == GDK_KEY_Shift_L ) || ( _i_event->keyval == GDK_KEY_Shift_R ) )
    {
        THIS->a_key_shift_any = false;
    }
    if ( ( _i_event->keyval == GDK_KEY_Control_L ) || ( _i_event->keyval == GDK_KEY_Control_R ) )
    {
        THIS->a_key_control_any = false;
    }

    return FALSE;
}
//  ################################################################################################
gboolean
Display::Z_smart_redim_timeout(
    gpointer _i_data)
{
    signal::Display  *   THIS = (signal::Display*)_i_data;

    THIS->a_smart_redim_timeout_runned =    THIS->a_smart_redim_timeout_runned      +
                                            THIS->a_smart_redim_timeout_interval    ;

    if ( THIS->a_smart_redim_timeout_runned > THIS->a_smart_redim_timeout_redraw )
    {
        THIS->a_redraw.set(true);
        THIS->z_redraw();
        //TKI("Timeout:draw\n");
        THIS->a_smart_redim_timeout_gtimeout = 0;
        return G_SOURCE_REMOVE;
    }

    //TKI("Timeout+[%li]\n", gts);
    return G_SOURCE_CONTINUE;
}
//  ************************************************************************************************
gboolean
Display::gtkevent__configure_main_widget_first(
    GdkEventConfigure   *   _i_evt)
{

    a_cfg_rec.n.pos.x   =   _i_evt->x;
    a_cfg_rec.n.pos.y   =   _i_evt->y;
    a_cfg_rec.n.dim.w   =   _i_evt->width;
    a_cfg_rec.n.dim.h   =   _i_evt->height;

    a_cfg_rec.o         =   a_cfg_rec.n;

    g_signal_handler_disconnect(
        G_OBJECT(cnt())                         ,
        a_handler__configure_event_main_widget  );

    a_handler__configure_event_main_widget = g_signal_connect(
        G_OBJECT(cnt())                             ,
        "configure-event"                           ,
        (GCallback)GtkEvent__configure_main_widget  ,
        (gpointer)this );

    return FALSE;
}
gboolean
Display::gtkevent__configure_main_widget(
    GdkEventConfigure   *   _i_evt)
{
    /*
    TKI("event [%i] [%p] [%i] [%i] [%i] [%i] [%i]\n",
        _i_evt->type        ,
        _i_evt->window      ,
        _i_evt->send_event  ,
        _i_evt->x           ,
        _i_evt->y           ,
        _i_evt->width       ,
        _i_evt->height      );
    */
    a_cfg_rec.n.pos.x   =   _i_evt->x;
    a_cfg_rec.n.pos.y   =   _i_evt->y;
    a_cfg_rec.n.dim.w   =   _i_evt->width;
    a_cfg_rec.n.dim.h   =   _i_evt->height;

    //dumprect(&a_cfg_rec.o);
    //dumprect(&a_cfg_rec.n);

    //  filter spurious "configure-event" where nothing has changed. In that case, dont touch
    //  a_sza_rec
    if (    ( a_cfg_rec.o.pos.x ==  _i_evt->x       )   &&
            ( a_cfg_rec.o.pos.y ==  _i_evt->y       )   &&
            ( a_cfg_rec.o.dim.w ==  _i_evt->width   )   &&
            ( a_cfg_rec.o.dim.h ==  _i_evt->height  )   )
    {
        TKI("%s\n", "Gda::configure-main-widget:no pos/dim change");
        return FALSE;
    }

    //  detect moving
    if (    ( a_cfg_rec.o.dim.w ==  a_cfg_rec.n.dim.w ) &&
            ( a_cfg_rec.o.dim.h ==  a_cfg_rec.n.dim.h ) )
    {
        //TKI("%s\n", "Gda::configure-main-widget:moving");

        a_sza_rec.n.pos.x   =   a_cfg_rec.n.pos.x;
        a_sza_rec.n.pos.y   =   a_cfg_rec.n.pos.y;
        a_sza_rec.o.pos     =   a_sza_rec.n.pos;
    }
    //  detect resize
    else
    {
        //TKI("%s\n", "Gda::configure-main-widget:resize");

        a_sza_rec.o.pos =   a_sza_rec.n.pos;
        a_sza_rec.n.pos =   a_cfg_rec.n.pos;
    }

    a_cfg_rec.o = a_cfg_rec.n;

    return FALSE;                                                                                   // propagate = FALSE
}
//  ************************************************************************************************
gboolean
Display::gtkevent__size_allocate(
    GdkRectangle *  _i_allocation   )
{
    a_sza_rec.n.dim.w   =   _i_allocation->width;
    a_sza_rec.n.dim.h   =   _i_allocation->height;

    //dumprect(&a_sza_rec.o);
    //dumprect(&a_sza_rec.n);

    /*
    if ( a_redim_resize )
    {
        //  Delta(w) = -Delta(left border) + Delta(right border)
        gint dxpl, dxpr, dxpw;
        dxpw    =   a_sza_rec.n.dim.w - a_sza_rec.o.dim.w;
        dxpl    =   a_sza_rec.n.pos.x - a_sza_rec.o.pos.x;
        dxpr    =   a_sza_rec.n.dim.w - a_sza_rec.o.dim.w + dxpl;
        TKI("dxpw[%i] dxpl[%i] dxpr[%i]\n", dxpw, dxpl, dxpr);

       //  Delta(h) = -Delta(top border) + Delta(bottom border)
        gint dypt, dypb, dyph;
        dyph    =   a_sza_rec.n.dim.h - a_sza_rec.o.dim.h;
        dypt    =   a_sza_rec.n.pos.y - a_sza_rec.o.pos.y;
        dypb    =   a_sza_rec.n.dim.h - a_sza_rec.o.dim.h + dypt;
        TKI("dyph[%i] dypt[%i] dypb[%i]\n", dyph, dypt, dypb);

        double dxr, dxl, dyt, dyb;
        dxl     =   ( (double)dxpl * ( atti()->area.deltax ) ) / (double)a_sza_rec.o.dim.w;
        dxr     =   ( (double)dxpr * ( atti()->area.deltax ) ) / (double)a_sza_rec.o.dim.w;
        dyt     =   ( (double)dypt * ( atti()->area.deltay ) ) / (double)a_sza_rec.o.dim.h;
        dyb     =   ( (double)dypb * ( atti()->area.deltay ) ) / (double)a_sza_rec.o.dim.h;

        TKI("dxl[%lf] dxr[%lf] dyt[%lf] dyb[%lf]\n", dxl, dxr, dyt, dyb);

        set_area(
            atti()->area.a_xmin + dxl   ,
            atti()->area.a_xmax + dxr   ,
            atti()->area.a_ymin - dyb   ,
            atti()->area.a_ymax - dyt   );
    }
    */
    a_sza_rec.o.dim = a_sza_rec.n.dim;
    //  ............................................................................................
    a_smart_redim_timeout_runned = 0;

    if ( a_smart_redim_timeout_gtimeout == 0 )
    {
        //TKI("creating timeout\n");
        a_smart_redim_timeout_gtimeout = g_timeout_add(
            a_smart_redim_timeout_interval              ,
            signal::Display::Z_smart_redim_timeout   ,
            (gpointer)this                              );
    }

    return FALSE;
}
//  ************************************************************************************************
gboolean
Display::gtkevent__configure(
    GdkEventConfigure*  _i_evt)
{
    return FALSE;
}
//  ************************************************************************************************
gboolean
Display::gtkevent__expose_event_first(
    GdkEventExpose  *   _i_expose)
{
    //  because we are called before the first "configure-event" on main widget, we have to get
    //  main app window parameters here
    gdk_window_get_origin(gtk_widget_get_window(cnt()), &a_sza_rec.n.pos.x, &a_sza_rec.n.pos.y);
    a_sza_rec.n.dim.w   =   _i_expose->area.width;
    a_sza_rec.n.dim.h   =   _i_expose->area.height;
    a_sza_rec.o         =   a_sza_rec.n;

    //  cairo stuff
    a_gdk_window                =   gtk_widget_get_window(d_area);

    //  switch signal handlers
    g_signal_handler_disconnect(G_OBJECT(d_area), a_handler__expose_event);
    a_handler__expose_event     =   g_signal_connect( G_OBJECT(d_area), "expose-event"      , (GCallback)GtkEvent__expose_event , (gpointer)this );
    a_handler__size_allocate    =   g_signal_connect( G_OBJECT(d_area), "size-allocate"     , (GCallback)GtkEvent__size_allocate, (gpointer)this );

    z_draw();

    return TRUE;
}
gboolean
Display::gtkevent__expose_event(
    GdkEventExpose  *   _i_evt)
{
    //TKI("expose\n");

    if ( a_smart_redim_timeout_gtimeout )                                                           // redraw when resize is handled by size-allocate
        return FALSE;

    z_draw();

    return FALSE;
}
//  ************************************************************************************************
//gboolean
//Display::gtkevent__motion_notify_event_first(
//    GdkEventMotion  *   _i_evt)
//{
//    //printf("state[%04x] x[%lf] y[%lf]\n", _i_evt->state, _i_evt->x, _i_evt->y);
//
//    a_mouse.coords.x    =   (gint)_i_evt->x;
//    a_mouse.coords.y    =   (gint)_i_evt->y;
//
//    //  switch signal handlers
//    g_signal_handler_disconnect(
//        G_OBJECT(d_area)                ,
//        a_handler__motion_notify_event  );
//
//    a_handler__motion_notify_event  =   g_signal_connect(
//        G_OBJECT(d_area)                            ,
//        "motion-notify-event"                       ,
//        (GCallback)GtkEvent__motion_notify_event    ,
//        (gpointer)this                              );
//
//    return TRUE;
//}
gboolean
Display::gtkevent__motion_notify_event(
    GdkEventMotion  *   _i_evt)
{
    //  ............................................................................................
    //  queue redraw for mouse coords
    a_redraw.set(true, false);
    a_redraw.inlays.mouse_coords = true;
    //  ............................................................................................
    //  handle drag
    if ( a_mouse.click.b1 )
    {
        a_mouse.motion.dragging = true;

        double  dx  =   _i_evt->x - a_mouse.motion.pos.x;
        double  dy  =   _i_evt->y - a_mouse.motion.pos.y;

        double  rx  =   atti()->area.a_deltax * 1.0 / 100.0;
        double  ry  =   atti()->area.a_deltay * 1.0 / 100.0;

        //TKI("drag [%+7.2lf]\n", dx);

        if ( dx > 0.0 )
        {
            if ( a_mouse.click.dragx == 1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax - rx, atti()->area.a_ymin , atti()->area.a_ymax  );
            if ( a_mouse.click.dragx == -1 )
                set_area(atti()->area.a_xmin - rx, atti()->area.a_xmax, atti()->area.a_ymin, atti()->area.a_ymax    );
        }
        if ( dx < 0.0 )
        {
            if ( a_mouse.click.dragx == 1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax + rx, atti()->area.a_ymin , atti()->area.a_ymax  );
            if ( a_mouse.click.dragx == -1 )
                set_area(atti()->area.a_xmin + rx, atti()->area.a_xmax, atti()->area.a_ymin, atti()->area.a_ymax    );
        }
        if ( dy > 0.0 )
        {
            if ( a_mouse.click.dragy == 1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax, atti()->area.a_ymin , atti()->area.a_ymax + ry  );
            if ( a_mouse.click.dragy == -1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax, atti()->area.a_ymin + ry , atti()->area.a_ymax  );

        }
        if ( dy < 0.0 )
        {
            if ( a_mouse.click.dragy == 1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax, atti()->area.a_ymin , atti()->area.a_ymax - ry  );
            if ( a_mouse.click.dragy == -1 )
                set_area(atti()->area.a_xmin , atti()->area.a_xmax, atti()->area.a_ymin - ry , atti()->area.a_ymax  );
        }

        a_redraw.set(true, true);
    }
    //  ............................................................................................
    a_mouse.motion.pos.x    =   _i_evt->x;
    a_mouse.motion.pos.y    =   _i_evt->y;

    z_redraw();
    return TRUE;
}
//  ************************************************************************************************
gboolean
Display::gtkevent__button_press_event(
    GdkEventButton  *   _i_evt)
{
    double  ax  =   0.0;
    double  ay  =   0.0;
    //  ............................................................................................
    a_mouse.click.bt        =   _i_evt->button;
    a_mouse.click.b1        =   ( _i_evt->button == 1 );
    a_mouse.click.b2        =   ( _i_evt->button == 2 );
    a_mouse.click.b3        =   ( _i_evt->button == 3 );
    a_mouse.click.pos.x     =   _i_evt->x;
    a_mouse.click.pos.y     =   _i_evt->y;
    a_mouse.click.shft      =   a_key_shift_any;
    a_mouse.click.ctrl      =   a_key_control_any;
    //  ............................................................................................
    if ( _i_evt->button == 1 )
    {

    ax              =   (double)wgt()->allocation.width;
    ay              =   (double)wgt()->allocation.height;

    a_mouse.click.dragx = 0;
    a_mouse.click.dragy = 0;

    if ( a_mouse.click.pos.x < ( ax / 3 ) )
        a_mouse.click.dragx = -1;
    if ( a_mouse.click.pos.x > ( 2 * ax / 3 ) )
        a_mouse.click.dragx = 1;

    if ( a_mouse.click.pos.y < ( ay / 3 ) )
        a_mouse.click.dragy = 1;
    if ( a_mouse.click.pos.y > ( 2 * ay / 3 ) )
        a_mouse.click.dragy = -1;

    }
    //  ............................................................................................
    if ( _i_evt->button == 3 )
    {
        ctx_menu_popup(3, _i_evt->time);
    }
    return FALSE;
}
gboolean
Display::gtkevent__button_release_event(
    GdkEventButton  *   _i_evt)
{
    double  x   =   0.0;
    double  y   =   0.0;
    double  ax  =   0.0;
    double  ay  =   0.0;
    //  ............................................................................................
    ax  =   (double)wgt()->allocation.width;
    ay  =   (double)wgt()->allocation.height;

    if ( _i_evt->button != 1 )
        return FALSE;

    if ( ! a_mouse.motion.dragging )
    {

    if ( ! a_mouse.click.ctrl )
    {
    if ( ! a_mouse.click.shft )
    {
        z_coords__widget_to_area_x(
            &x                  ,
            ax                  ,
            (double)_i_evt->x   );

        select_mark_set(&a_select_mark_v, x);

        a_redraw.set(true, false);
        a_redraw.selected = true;

        gtk_widget_queue_draw( wgt() );
    }
    if ( a_mouse.click.shft )
    {
        z_coords__widget_to_area_y(
            &y                  ,
            ay                  ,
            (double)_i_evt->y   );

        select_mark_set(&a_select_mark_h, y);

        a_redraw.set(true, false);
        a_redraw.selected = true;

        gtk_widget_queue_draw( wgt() );
    }
    }

    }

    //TKI("state[%04x]\n", _i_evt->state);

    a_mouse.click.b1        = false;
    a_mouse.motion.dragging = false;

    return FALSE;
}
//  ################################################################################################
void
Display::z_redraw()
{
    gtk_widget_queue_draw( wgt() );
}
void
Display::refresh(
    display::Redraw     *   _i_redraw)
{
    a_redraw    =   *( _i_redraw );
    z_redraw();
}
void
Display::refresh(
    display::Redraw     &   _io_redraw)
{
    a_redraw    =   _io_redraw;
    z_redraw();
}

void
Display::set_area(double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax)
{
    control()->display__chg_area( uid(), _i_xmin, _i_xmax, _i_ymin, _i_ymax );
}


}
}



