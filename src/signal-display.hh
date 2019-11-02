#ifndef __ERG_SIGNAL_DISPLAY_HH__
#define __ERG_SIGNAL_DISPLAY_HH__

#include    <gtk/gtk.h>

#include    "signal-types.hh"
#include    "signal-model.hh"

#include    <vector>

namespace erg
{
namespace signal
{

class   IData;

class   Display : public signal::MemberModel, public signal::MemberControl
{
    //  --------------------------------------------------------------------------------------------
    //  struct
    //  --------------------------------------------------------------------------------------------
  protected:
    typedef     struct  _sGdkRectangleOldNew
    {
        sGdkRectangle  o;
        sGdkRectangle  n;
    } sGdkRectangleOldNew;

    typedef     struct  _sMouseInfo
    {
        struct
        {
            guint       bt;
            bool        b1;
            bool        b2;
            bool        b3;
            bool        shft;
            bool        ctrl;
            GdkPoint    pos;

            gint        dragx;
            gint        dragy;
        } click;

        struct
        {
            GdkPoint    pos;
            bool        dragging;
        } motion;


    } sMouseInfo;

    typedef     struct _sSelMarks
    {
        gdouble     v1;
        gdouble     v2;
        bool        v1_set;
        bool        v2_set;
    } sSelMarks;
    //  --------------------------------------------------------------------------------------------
    //  view : gtk callbacks, smart redraw
    //  --------------------------------------------------------------------------------------------
  protected:
    GtkWidget               *   d_area;
    GtkWidget               *   a_container;
    GtkWidget               *   d_container;

    guint                       a_smart_redim_timeout_gtimeout;
    guint                       a_smart_redim_timeout_runned;
    guint                       a_smart_redim_timeout_interval;
    guint                       a_smart_redim_timeout_redraw;

    sGdkRectangleOldNew         a_cfg_rec;                                                          // size allocation of main widget
    sGdkRectangleOldNew         a_sza_rec;                                                          // size allocation of d_area

    gulong                      a_handler__configure_event_main_widget;

    gulong                      a_handler__expose_event;
    gulong                      a_handler__configure_event;
    gulong                      a_handler__size_allocate;
    gulong                      a_handler__motion_notify_event;
    gulong                      a_handler__event_button_press;
    gulong                      a_handler__event_button_release;
    //  --------------------------------------------------------------------------------------------
            gboolean    gtkevent__configure_main_widget_first   (GdkEventConfigure*);
            gboolean    gtkevent__configure_main_widget         (GdkEventConfigure*);

            gboolean    gtkevent__size_allocate                 (GdkRectangle*);
            gboolean    gtkevent__configure                     (GdkEventConfigure*);
            gboolean    gtkevent__expose_event_first            (GdkEventExpose*);
            gboolean    gtkevent__expose_event                  (GdkEventExpose*);
            //gboolean    gtkevent__motion_notify_event_first     (GdkEventMotion*);
            gboolean    gtkevent__motion_notify_event           (GdkEventMotion*);
            gboolean    gtkevent__button_press_event            (GdkEventButton*);
            gboolean    gtkevent__button_release_event          (GdkEventButton*);

    static  gboolean    Z_smart_redim_timeout                   (gpointer);

    static  gboolean    GtkEvent__configure_main_widget_first   (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__configure_main_widget         (GtkWidget*, GdkEvent*      , gpointer);

    static  gboolean    GtkEvent__size_allocate                 (GtkWidget*, GdkRectangle*  , gpointer);
    static  gboolean    GtkEvent__configure                     (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__expose_event_first            (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__expose_event                  (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__motion_notify_event_first     (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__motion_notify_event           (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__button_press_event            (GtkWidget*, GdkEvent*      , gpointer);
    static  gboolean    GtkEvent__button_release_event          (GtkWidget*, GdkEvent*      , gpointer);

    static  gboolean    GtkEvent__key_press_event               (GtkWidget*, GdkEventKey*   , gpointer);
    static  gboolean    GtkEvent__key_release_event             (GtkWidget*, GdkEventKey*   , gpointer);
    //  --------------------------------------------------------------------------------------------
  public:
    inline      size_t              uid()   const   {   return a_uid;                       }

    inline      GtkWidget       *   cnt()   const   {   return a_container ? a_container : d_container;     }
    inline      GtkWidget       *   wgt()   const   {   return d_area;                      }
    inline      GtkDrawingArea  *   wda()   const   {   return GTK_DRAWING_AREA(d_area);    }
    //  --------------------------------------------------------------------------------------------
    //  model
    //  --------------------------------------------------------------------------------------------
  private:
    bool                a_key_shift_any;
    bool                a_key_control_any;

  private:
    sSelMarks           a_select_mark_v;
    sSelMarks           a_select_mark_h;

    void                select_mark_set     (Display::sSelMarks*,  double);


  private:
    GtkWidget       *   a_ctx_menu_popup;

    GtkUIManager    *   a_ctx_menu_uim;
    std::string         a_ctx_menu_desc;
    GtkActionEntry      a_ctx_menu_entries  [6];
    GtkActionGroup  *   a_ctx_menu_group;

    void                ctx_menu_popup(gint _i_button, guint32 _i_event_time);

  private:
    signal::Model::sDisplayAtt      const   *   a_att;
    inline  Model::sDisplayAtt      const   *   att()   const { return a_att;       }
    inline  Model::sDisplayAttI     const   *   atti()  const { return &(a_att->i); }
    inline  Model::sDisplayAttV     const   *   attv()  const { return &(a_att->v); }
    inline  Model::sDisplayAttD     const   *   attd()  const { return &(a_att->d); }

  private:
    void    set_area(double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax);
    //  --------------------------------------------------------------------------------------------
    //  view : draw
    //  --------------------------------------------------------------------------------------------
  protected:
    GdkWindow                   *   a_gdk_window;                                                   // of d_area

    cairo_surface_t             *   d_crs_1;                                                        // source 1 surface = signals
    cairo_surface_t             *   d_crs_2;                                                        // source 2 surface = axes + selection
    cairo_surface_t             *   d_crs_3;                                                        // source 3 surface = inlays
    cairo_t                     *   d_ctx;                                                          // cairo context : GdkWindow of Widget
    cairo_t                     *   d_ctx_1;                                                        // cairo context :surface 1
    cairo_t                     *   d_ctx_2;                                                        // cairo context :surface 2
    cairo_t                     *   d_ctx_3;                                                        // cairo context :surface 3

    Display::sMouseInfo             a_mouse;
    display::Redraw                 a_redraw;
    double const                    a_dash_selmarks [2] = { 8.0, 4.0 };

    void        z_draw                      ();
    void        z_draw_signal               (cairo_t*   _i_cr, IData const * _i_data, sDrawStyle const * _i_style, size_t _i_sample_ix1, size_t _i_sample_ix2);
    void        z_draw_signals              (cairo_t*   _i_cr);
    void        z_draw_select_marks         (cairo_t*   _i_cr);
    void        z_draw_axes                 (cairo_t*   _i_cr);
    void        z_draw_mouse_coords         (cairo_t*   _i_cr, gint _i_mouse_x, gint _i_mouse_y);
    void        z_draw_x_interval_selection (cairo_t*   _i_cr);

  private:
    void        z_redraw();

  public:
    void        refresh(display::Redraw*);
    void        refresh(display::Redraw&);
    //  --------------------------------------------------------------------------------------------
    //  controller : utils
    //  --------------------------------------------------------------------------------------------
  private:
    static  size_t              s_uid;

  protected:
    size_t                      a_uid;

  protected:
    void    z_coords__widget_to_area_x  (double* _o_xarea, double _i_widget_width , double _i_xwidget);
    void    z_coords__widget_to_area_y  (double* _o_yarea, double _i_widget_height, double _i_ywidget);
    void    z_coords__widget_to_area_xy (double* _o_xarea, double* _o_yarea, double _i_widget_width, double _i_widget_height, double _i_xwidget, double _i_ywidget);

    void    z_coords__area_to_widget_x  (double* _o_xwidget, double _i_widget_width , double _i_xarea);
    void    z_coords__area_to_widget_y  (double* _o_ywidget, double _i_widget_height, double _i_yarea);
    void    z_coords__area_to_widget_xy (double* _o_xwidget, double* _o_ywidget, double _i_widget_width, double _i_widget_height, double _i_xarea, double _i_yarea);
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
    static  Display *   New(
        signal::Model       *   _i_model                        ,
        signal::Control     *   _i_control                      ,
        GtkWidget           *   _i_main_widget                  ,
        guint                   _i_smart_redim_timeout_interval ,
        guint                   _i_smart_redin_timeout_redraw   ,
        Model::sDisplayAtt *   _i_att                           );

  private:
     Display(
        signal::Model       *   _i_model                        ,
        signal::Control     *   _i_control                      ,
        size_t                  _i_iud                          ,
        guint                   _i_smart_redim_timeout_interval ,
        guint                   _i_smart_redin_timeout_redraw   ,
        Model::sDisplayAtt  *   _i_att                           );

     Display(
        signal::Model       *   _i_model                        ,
        signal::Control     *   _i_control                      ,
        GtkWidget           *   _i_main_widget                  ,
        size_t                  _i_iud                          ,
        guint                   _i_smart_redim_timeout_interval ,
        guint                   _i_smart_redin_timeout_redraw   ,
        Model::sDisplayAtt *   _i_att                           );

    void    construct();

    ~Display ()    {}


};



}
}










#endif
