#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-types.hh"
#include    "signal-data.hh"
#include    "signal-main.hh"
#include    "signal-model.hh"
#include    "signal-view.hh"
#include    "signal-control.hh"

size_t                      erg::signal::IData::s_uid   =   0;

GdkColor                    erg::signal::A_GdkColor_black   = { 0,0,0,0 };

erg::signal::sDrawStyle     erg::signal::A_draw_style_default  =
    {
        A_GdkColor_black                ,
        erg::signal::eCross02           ,
        0.5                             ,
        false                           ,
        0.5
    };

namespace erg
{
namespace signal
{

void    Get_GdkColor(GdkColor& _o_gdkcolor, signal::sColor _i_scolor)
{
    _o_gdkcolor.red     = (65535/_i_scolor.r) * 255;
    _o_gdkcolor.green   = (65535/_i_scolor.g) * 255;
    _o_gdkcolor.blue    = (65535/_i_scolor.b) * 255;
    _o_gdkcolor.pixel   = (gulong)(_o_gdkcolor.red * 65536 + _o_gdkcolor.green * 256 + _o_gdkcolor.blue);
}




bool    Main::start()
{
    d_model     =   new signal::Model();
    d_control   =   new signal::Control();
    d_view      =   new signal::View(d_model, d_control);

    d_view->set_model   ( d_model   );
    d_view->set_control ( d_control );

    d_control->set_model  ( d_model   );
    d_control->set_view   ( d_view    );


    d_view->construct();
    d_view->show();

    return true;
}

void    Main::signal_add(
    signal::IData       *   _i_idata    ,
    std::string         &   _i_fname    )
{
    d_control->signal__add(_i_idata, _i_fname);
}


}
}



