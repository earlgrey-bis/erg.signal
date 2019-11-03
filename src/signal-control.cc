#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-data.hh"

#include    "signal-model.hh"
#include    "signal-view.hh"
#include    "signal-control.hh"

#include    "signal-display.hh"

namespace erg
{
namespace signal
{

//  ================================================================================================
void
Control::signal__add(
    signal::IData       *   _i_idata    ,
    Model::sSignalAtt   *   _i_att      )
{
    Model::sSignalAtt   *   att =   nullptr;
    //  ............................................................................................
    att     =   ERGC_NEW0(Model::sSignalAtt);
    *att    =   *_i_att;

    model()->signal__add(_i_idata, att);

    view()->refresh();
}
void
Control::signal__add(
    signal::IData       *   _i_idata    ,
    std::string         &   _i_fname    )
{
    Model::sSignalAtt       att;
    //  ............................................................................................
    att.d.fname     =   _i_fname;
    att.d.sname     =   std::string(_i_idata->name());

    signal__add(_i_idata, &att);
}

void
Control::display__add(
    Model::sDisplayAtt  *   _i_att      )
{
    Model::sDisplayAtt  *   att =   nullptr;
    signal::Display     *   dsp =   nullptr;
    //  ............................................................................................
    att     =   ERGC_NEW0(Model::sDisplayAtt);
    *att    =   *_i_att;
    dsp     =   signal::Display::New(model(), this, nullptr, 100, 300, att);

    model()->display__add(dsp, att);

    view()->refresh();
}
void
Control::display__add(
    std::string         &   _i_name     ,
    signal::AreaDbl     *   _i_area     ,
    signal::sSztPoint   *   _i_rulers   ,
    signal::sDblPoint   *   _i_axis     ,
    signal::sColor      *   _i_bgcolor  ,
    display::sDraw      *   _i_what     )
{
    Model::sDisplayAtt      att;
    //  ............................................................................................
    att.d.name      =   _i_name;

    att.i.area      =   *_i_area;
    att.i.rulers    =   *_i_rulers;
    att.i.axis      =   *_i_axis;

    att.v.bgcolor   =   *_i_bgcolor;
    att.v.draw      =   *_i_what;

    display__add(&att);
}
//  ================================================================================================
void
Control::display__add_signal(
    size_t              _i_display_uid  ,
    size_t              _i_signal_uid   )
{
    //  ............................................................................................
    if ( ! model()->display__add_signal(_i_display_uid, _i_signal_uid) )
        return;

    view()->refresh();
    view()->refresh_display(_i_display_uid);
}
void
Control::display__del_signal(
    size_t              _i_display_uid  ,
    size_t              _i_signal_uid   )
{
}

void
Control::display__chg_area(
    size_t      _i_display_uid  ,
    double      _i_xmin         ,
    double      _i_xmax         ,
    double      _i_ymin         ,
    double      _i_ymax         )
{
    Model::sDisplayAttI     att;
    //  ............................................................................................
    if ( ! model()->display__get_att_I(_i_display_uid, &att) )
        return;

    att.area.set(_i_xmin, _i_xmax, _i_ymin, _i_ymax);

    model() ->display__set_att_I    (_i_display_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display       (_i_display_uid, true, true, true, true, true);
}

void
Control::display__chg_signal_motif(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    gint        _i_motif        )
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.style.motif = static_cast< signal::eDrawMotif >( _i_motif );

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_ix1(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    size_t      _i_ix1          )
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.ix1 = _i_ix1;

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_ix2(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    size_t      _i_ix2          )
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.ix2 = _i_ix2;

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_ep(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    gdouble     _i_ep)
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.style.motif_ep  = _i_ep;

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_join(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    bool        _i_join         )
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.style.join = _i_join;

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_join_ep(
    size_t      _i_display_uid  ,
    size_t      _i_signal_uid   ,
    gdouble     _i_ep)
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.style.join_ep = _i_ep;

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
void
Control::display__chg_signal_color(
    size_t          _i_display_uid  ,
    size_t          _i_signal_uid   ,
    GdkColor    *   _i_color)
{
    Model::sDisplayedSignalAtt  att;
    //  ............................................................................................
    if ( ! model()->display__get_signal_att(_i_display_uid, _i_signal_uid, &att) )
        return;

    att.style.color = *( _i_color );

    model() ->display__set_signal_att   (_i_display_uid, _i_signal_uid, &att);
    view()  ->refresh();
    view()  ->refresh_display           (_i_display_uid, true, false, false, false, false);
}
//  ================================================================================================
gboolean
Control::GtkEvent__pgdisplays__button_press_event__add_display(
    GtkWidget   *   _i_widget       ,
    GdkEvent    *   _i_evt          ,
    gpointer        _i_data         )
{
    View::CallbackData          *   cbk     =   (View::CallbackData*)_i_data;
    signal::Control             *   THIS    =   (signal::Control*)cbk->a_instance;
    Model::sDisplayAtt              att;
    //  ............................................................................................
    att.d.name          =   std::string("new display");

    att.i.area.set(-0.05, +0.05, -0.05, +0.05);

    att.i.rulers.x      =   4;
    att.i.rulers.y      =   2;

    att.i.axis.x        =   0.0;
    att.i.axis.y        =   0.0;

    att.v.bgcolor.r     =   255;
    att.v.bgcolor.g     =   0;
    att.v.bgcolor.b     =   0;

    att.v.draw.signals                  =   true;
    att.v.draw.axes                     =   true;
    att.v.draw.inlays.mouse_coords      =   true;

    THIS->display__add(&att);

    return FALSE;
}


/*
gboolean
Main::GtkEvent__pgsignals__changed__comboboxtext_display(
    GtkComboBox     *   _i_cbbx     ,
    gpointer            _i_data     )
{
    Main::ViewCallbackData  *   vcd     =   (Main::ViewCallbackData*)_i_data;
    Main                    *   THIS    =   (Main*)(vcd->a_instance);
    Main::Model::sIData     *   mid     =   (Main::Model::sIData*)(vcd->a_v1);
    //  ............................................................................................
    gchar   *   text    =   gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(_i_cbbx));
    size_t      did     =   0;

    if ( strlen(text) >= 2 )
    {
    if (    ( g_ascii_isdigit(text[0]) )    ||
            ( g_ascii_isdigit(text[1]) )    )
    {

    did = 10 * ( text[0] - '0' ) + ( text[1] - '0' );

    }
    }

    if ( mid->display_id == did )
    {
        return FALSE;
    }

    //  ............................................................................................
    //  remoce signal from old display

    //  ............................................................................................
    //  add signal to new display

    THIS->model()->display__add_signal(did - 1, mid);

    g_free(text);
    return FALSE;
}
*/

}
}



