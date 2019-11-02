#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-display.hh"
#include    "signal-data.hh"

namespace erg
{
namespace signal
{

void dumprect(signal::sGdkRectangle* _i_r)
{
    TKI("x[%i] y[%i] w[%i] h[%i]\n", _i_r->pos.x, _i_r->pos.y, _i_r->dim.w, _i_r->dim.h);
}

void dumpdim(signal::sGintDim* _i_d)
{
    TKI("w[%i] h[%i]\n", _i_d->w, _i_d->h);
}

void
Display::z_coords__widget_to_area_x(
    double  *   _o_xarea        ,
    double      _i_widget_width ,
    double      _i_xwidget      )
{
    *(_o_xarea) = atti()->area.a_xmin   + ( atti()->area.a_deltax * _i_xwidget   ) / _i_widget_width;
}
void
Display::z_coords__widget_to_area_y(
    double  *   _o_yarea        ,
    double      _i_widget_height,
    double      _i_ywidget      )
{
    *(_o_yarea) = atti()->area.a_ymin   + ( atti()->area.a_deltay * ( _i_widget_height - _i_ywidget ) ) / _i_widget_height;
}
void
Display::z_coords__widget_to_area_xy(
    double* _o_xarea        ,
    double* _o_yarea        ,
    double _i_widget_width  ,
    double _i_widget_height ,
    double _i_xwidget       ,
    double _i_ywidget       )
{
    *(_o_xarea) = atti()->area.a_xmin   + ( atti()->area.a_deltax * _i_xwidget   ) / _i_widget_width;
    *(_o_yarea) = atti()->area.a_ymin   + ( atti()->area.a_deltay * ( _i_widget_height - _i_ywidget ) ) / _i_widget_height;
}
void
Display::z_coords__area_to_widget_x(
    double  *   _o_xwidget      ,
    double      _i_widget_width ,
    double      _i_xarea        )
{
    *(_o_xwidget) = ( ( _i_xarea - atti()->area.a_xmin ) * _i_widget_width ) / atti()->area.a_deltax;
}
void
Display::z_coords__area_to_widget_y(
    double  *   _o_ywidget      ,
    double      _i_widget_height,
    double      _i_yarea        )
{
    *(_o_ywidget) = _i_widget_height - ( ( _i_yarea - atti()->area.a_ymin ) * _i_widget_height ) / atti()->area.a_deltay;
}
void
Display::z_coords__area_to_widget_xy(
    double  *   _o_xwidget      ,
    double  *   _o_ywidget      ,
    double      _i_widget_width ,
    double      _i_widget_height,
    double      _i_xarea        ,
    double      _i_yarea        )
{
    *(_o_xwidget) = ( ( _i_xarea - atti()->area.a_xmin ) * _i_widget_width ) / atti()->area.a_deltax;
    *(_o_ywidget) = _i_widget_height - ( ( _i_yarea - atti()->area.a_ymin ) * _i_widget_height ) / atti()->area.a_deltay;
}


}
}



