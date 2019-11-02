#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-data.hh"
#include    "signal-model.hh"
#include    "signal-display.hh"

namespace erg
{
namespace signal
{

//  ################################################################################################
//  Datas visualization
//  ################################################################################################
Model::sSignal *
Model::z_signal__get_from_uid(
    size_t  _i_signal_uid   )
{
    Model::sSignal  *   ss  =   nullptr;
    //  ............................................................................................
    for ( tVSgn::iterator it = a_signals.begin() ; it != a_signals.end() ; it++ )
    {
        ss = *it;
        if  ( ss->idata->uid() == _i_signal_uid )
        {
            return ss;
        }
    }
    return nullptr;
}
Model::sCSignal const *
Model::signal__get_from_uid(size_t _i_signal_uid)
{
    return (Model::sCSignal*)( z_signal__get_from_uid(_i_signal_uid) );
}

void
Model::signal__add(
    signal::IData       *   _i_idata    ,
    Model::sSignalAtt   *   _i_att      )
{
    Model::sSignal          *   sgn =   ERGC_NEW0(Model::sSignal);
    //  ............................................................................................
    sgn->idata                  =   _i_idata;
    sgn->att                    =   _i_att;

    a_signals.push_back(sgn);
}
//  ################################################################################################
//  Displays
//  ################################################################################################
Model::sDisplay   *
Model::z_display__get_from_uid(
    size_t  _i_display_uid  )
{
    Model::sDisplay     *   sd  =   nullptr;
    //  ............................................................................................
    for ( tVDsp::iterator it = a_displays.begin() ; it != a_displays.end() ; it++ )
    {
        sd = *it;
        if  ( sd->display->uid() == _i_display_uid )
        {
            return sd;
        }
    }
    return nullptr;
}
Model::sCDisplay const *
Model::display__get_from_uid(
    size_t  _i_display_uid  )
{
    return (Model::sCDisplay*)( z_display__get_from_uid(_i_display_uid) );
}

void    Model::display__add(
    signal::Display     *   _i_display          ,
    Model::sDisplayAtt  *   _i_allocated_att    )
{
    Model::sDisplay     *   dsp =   ERGC_NEW0(Model::sDisplay);
    //  ............................................................................................
    dsp->display    =   _i_display;
    dsp->att        =   _i_allocated_att;

    a_displays.push_back(dsp);
}
//  ################################################################################################
//  Displays
//  ################################################################################################
bool    Model::display__del_signal(
    size_t              _i_display_uid  ,
    size_t              _i_signal_uid   )
{
    /*
    Model::sSgnDpl  *   sd;
    //  ............................................................................................
    VPSTORAGE_ENUM_START(&a_sgn_dpl, sd)
    {
        if  (   ( sd->idata     ==  _i_idata    )   &&
                ( sd->display   ==  _i_display  )   )
        {
            a_sgn_dpl.del(sd);
            free(sd);
        }

        VPSTORAGE_ENUM_NEXT(&a_sgn_dpl, sd)
    }
    */
    return false;
}
bool    Model::display__add_signal(
    size_t              _i_display_uid  ,
    size_t              _i_signal_uid   )
{
    Model::sDisplay     *   sd  =   nullptr;
    Model::sSignal      *   ss  =   nullptr;
    //  ............................................................................................
    sd  =   z_display__get_from_uid (_i_display_uid);
    ss  =   z_signal__get_from_uid  (_i_signal_uid);

    if ( ( !ss ) || (!sd) )
        return false;

    Model::sDisplayedSignal sds =   { _i_signal_uid, A_draw_style_default, 0, ss->idata->crd() - 1 };

    sd->att->signals.push_back( sds );

    return true;
}

Model::sDisplayedSignal*
Model::z_display__get_signal(
    sDisplay    *   _i_sdp          ,
    size_t          _i_signal_uid   )
{
    for ( size_t i = 0 ; i != _i_sdp->att->signals.size() ; i++ )
    {
        if ( _i_sdp->att->signals.at(i).uid == _i_signal_uid )
        {
            return &( _i_sdp->att->signals.at(i) );
        }
    }
    return nullptr;
}
Model::sDisplayedSignal const *
Model::display__get_signal(
    sCDisplay   *   _i_sdp          ,
    size_t          _i_signal_uid   )
{
    for ( size_t i = 0 ; i != _i_sdp->att->signals.size() ; i++ )
    {
        if ( _i_sdp->att->signals.at(i).uid == _i_signal_uid )
        {
            return &( _i_sdp->att->signals.at(i) );
        }
    }
    return nullptr;
}

bool
Model::display__get_signal_att(
    size_t                          _i_display_uid      ,
    size_t                          _i_signal_uid       ,
    Model::sDisplayedSignalAtt  *   _o_att              )
{
    Model::sDisplay             *   sdp     =   nullptr;
    Model::sDisplayedSignal     *   sds     =   nullptr;
    //  ............................................................................................
    sdp =   z_display__get_from_uid(_i_display_uid);
    if ( ! sdp )    return false;

    sds = z_display__get_signal(sdp, _i_signal_uid);
    if ( ! sds )    return false;

    (*_o_att) = sds->att;
    return true;
}
bool
Model::display__set_signal_att(
    size_t                          _i_display_uid      ,
    size_t                          _i_signal_uid       ,
    Model::sDisplayedSignalAtt  *   _i_att              )
{
    Model::sDisplay             *   sdp     =   nullptr;
    Model::sDisplayedSignal     *   sds     =   nullptr;
    //  ............................................................................................
    sdp =   z_display__get_from_uid(_i_display_uid);
    if ( ! sdp )    return false;

    sds = z_display__get_signal(sdp, _i_signal_uid);
    if ( ! sds )    return false;

    sds->att = (*_i_att);
    return true;
}
//  ================================================================================================
void
Model::display__set_area_from_display(
    size_t                          _i_display_uid      ,
    Model::sDisplayAtt  const   *   _i_att              ,
    double                          _i_xmin             ,   double  _i_xmax ,
    double                          _i_ymin             ,   double  _i_ymax )
{
    ( const_cast< sDisplayAtt * >( _i_att ) )->i.area.set(_i_xmin, _i_xmax, _i_ymin, _i_ymax);

    // refresh pgdisplays
}

bool
Model::display__get_att_I(
    size_t                      _i_display_uid  ,
    Model::sDisplayAttI     *   _o_att)
{
    Model::sDisplay             *   sdp     =   nullptr;
    //  ............................................................................................
    sdp =   z_display__get_from_uid(_i_display_uid);
    if ( ! sdp )    return false;

    *( _o_att ) = sdp->att->i;
    return true;
}

bool
Model::display__set_att_I(
    size_t                      _i_display_uid  ,
    Model::sDisplayAttI     *   _i_att          )
{
    Model::sDisplay             *   sdp     =   nullptr;
    //  ............................................................................................
    sdp =   z_display__get_from_uid(_i_display_uid);
    if ( ! sdp )    return false;

    sdp->att->i = *( _i_att );
    return true;
}
/*
void
Model::display__set_area(
    size_t                          _i_display_uid      ,
    Model::sDisplayAtt  const   *   _i_att              ,
    double                          _i_xmin             ,   double  _i_xmax ,
    double                          _i_ymin             ,   double  _i_ymax )
{
}
*/
//  ################################################################################################
//  Other
//  ################################################################################################
Model::Model()
{
    a_csignals.set  (&a_signals);
    a_cdisplays.set (&a_displays);

}




}
}



