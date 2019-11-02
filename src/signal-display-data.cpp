#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

#include    "signal-display.hh"
#include    "signal-data.hh"

namespace erg
{
namespace signal
{

/*
size_t  Display::add_data(
    erg::signal::IData  const   *   _i_idata    ,
    signal::eDrawMotif              _i_motif    ,
    guint8                          _i_r        ,
    guint8                          _i_g        ,
    guint8                          _i_b        )
{
    GdkColor color;
    color.red       = (65535/255) * _i_r;
    color.green     = (65535/255) * _i_g;
    color.blue      = (65535/255) * _i_b;
    color.pixel = (gulong)(color.red * 65536 + color.green * 256 + color.blue);

    return add_data(_i_idata, _i_motif, color);
}


size_t  Display::add_data(
    erg::signal::IData  const   *   _i_idata    ,
    signal::eDrawMotif              _i_motif    ,
    GdkColor                        _i_color    )
{
    sDrawStyle  *   ds  =   ERGC_NEW0(sDrawStyle);

    ds->color   =   _i_color;
    ds->motif   =   _i_motif;

    a_idata.push_back(_i_idata);
    a_idata_draw_style.push_back(ds);

    return a_idata.size() - 1;
}
*/
/*
void
Display::add_signal(
    Model::sSignal  *   _i_ssignal  )
{
    a_signals.push_back(_i_ssignal);
}

void
Display::del_signal(
    Model::sSignal  *   _i_ssignal  )
{
}
*/

void
Display::select_mark_set(
    Display::sSelMarks      *   _i_sl   ,
    double                      _i_v    )
{
    if ( _i_sl->v1_set && _i_sl->v2_set )
    {
        _i_sl->v1_set = false;
        _i_sl->v2_set = false;
    }

    if ( ! _i_sl->v1_set )
    {
        _i_sl->v1       = _i_v;
        _i_sl->v1_set   = true;
        return;
    }

    if ( ! _i_sl->v2_set )
    {
        if ( _i_v < _i_sl->v1 )
        {
            _i_sl->v2 = _i_sl->v1;
            _i_sl->v1 = _i_v;
        }
        else
        {
            _i_sl->v2 = _i_v;
        }
        _i_sl->v2_set = true;
        return;
    }
}

void
Display::ctx_menu_popup(gint _i_button, guint32 _i_event_time)
{
    printf("popup\n");
    gtk_menu_popup(GTK_MENU(gtk_ui_manager_get_widget(a_ctx_menu_uim, "/ctx-popup")), NULL, NULL, NULL, NULL, _i_button,  _i_event_time);
}


}
}



