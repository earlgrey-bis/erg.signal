#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-display.hh"
#include    "signal-data.hh"
//#include    "signal-main.hh"

namespace erg
{
namespace signal
{

void
Display::z_draw()
{
    gint        ax      =   0;
    gint        ay      =   0;
    //  ............................................................................................
    printf("z_draw():");

    ax      =   wgt()->allocation.width;
    ay      =   wgt()->allocation.height;
    //  ............................................................................................
    //  main context
    if ( d_ctx )
    {
        cairo_destroy(d_ctx);
    }
    d_ctx   =   gdk_cairo_create( a_gdk_window );
    //  ............................................................................................
    //  draw on surfaces
    if ( attv()->draw.signals )
    {
        if  (   ( a_redraw.flag ) && ( a_redraw.signals ) )
        {
            if ( d_ctx_1 )
            {
                cairo_surface_destroy   (d_crs_1);
                cairo_destroy           (d_ctx_1);
            }
            d_crs_1 =   gdk_window_create_similar_surface( a_gdk_window, CAIRO_CONTENT_COLOR_ALPHA, ax, ay );
            d_ctx_1 =   cairo_create( d_crs_1 );
            z_draw_signals(d_ctx_1);
            printf("sgns ");
        }
    }
    if ( attv()->draw.axes )
    {
        if  (   ( a_redraw.flag )                       &&
                ( a_redraw.axes || a_redraw.selected )  )
        {
            if ( d_ctx_2 )
            {
                cairo_surface_destroy   (d_crs_2);
                cairo_destroy           (d_ctx_2);
            }
            d_crs_2 =   gdk_window_create_similar_surface( a_gdk_window, CAIRO_CONTENT_COLOR_ALPHA, ax, ay );
            d_ctx_2 =   cairo_create( d_crs_2 );
            z_draw_axes         (d_ctx_2);
            z_draw_select_marks (d_ctx_2);
            printf("axes ");
        }
    }
    if ( attv()->draw.inlays.mouse_coords )
    {
        if  (   ( a_redraw.flag )                       &&
                ( a_redraw.inlays.mouse_coords )        )
        {
            if ( d_ctx_3 )
            {
                cairo_surface_destroy   (d_crs_3);
                cairo_destroy           (d_ctx_3);
            }
            d_crs_3 =   gdk_window_create_similar_surface( a_gdk_window, CAIRO_CONTENT_COLOR_ALPHA, ax, ay );
            d_ctx_3 =   cairo_create( d_crs_3 );
            z_draw_mouse_coords(d_ctx_3, a_mouse.motion.pos.x, a_mouse.motion.pos.y);
            printf("mous ");
        }
    }
    printf("\n");
    //  ............................................................................................
    //  merge
    cairo_set_operator( d_ctx, CAIRO_OPERATOR_OVER );

    if ( d_ctx_1 )
    {
        cairo_set_source_surface( d_ctx, d_crs_1, 0.0, 0.0 );
        cairo_paint             ( d_ctx );
    }
    if ( d_ctx_2 )
    {
        cairo_set_source_surface( d_ctx, d_crs_2, 0.0, 0.0 );
        cairo_paint             ( d_ctx );
    }
    if ( d_ctx_3 )
    {
        cairo_set_source_surface( d_ctx, d_crs_3, 0.0, 0.0 );
        cairo_paint             ( d_ctx );
    }
    //  ............................................................................................
    //  exit

    //  erase redraw directives
    a_redraw.set(false);
}
//  ================================================================================================
void
Display::z_draw_signal(cairo_t* _i_cr, IData const * _i_idata, sDrawStyle const * _i_style, size_t _i_sample_ix1, size_t _i_sample_ix2)
{
    gint            ax  =   wgt()->allocation.width;
    gint            ay  =   wgt()->allocation.height;

    double          pxd     =   0.0;
    double          pyd     =   0.0;
    double          nxd     =   -1.0;
    double          nyd     =   -1.0;

    double          x       =   0.0f;
    double          y       =   0.0f;
    double          mep     =   0.0f;
    double          jep     =   0.0f;
    bool            bCross  =   false;
    bool            bPoint  =   false;
    bool            bCircle =   false;
    bool            bDisk   =   false;
    double          cln     =   0.0f;
    //  ............................................................................................
    ERGC_RTV__T( _i_sample_ix1 > _i_idata->crd() );
    ERGC_RTV__T( _i_sample_ix2 > _i_idata->crd() );
    ERGC_RTV__T( ax == 0 );
    ERGC_RTV__T( ay == 0 );
    //  ............................................................................................
    mep     =   _i_style->motif_ep;
    jep     =   _i_style->join_ep;

    switch ( _i_style->motif )
    {
    case    eCross01:   cln = 1.0;  bCross = true ; break;
    case    eCross02:   cln = 2.0;  bCross = true ; break;
    case    eCross03:   cln = 3.0;  bCross = true ; break;
    case    eCross04:   cln = 4.0;  bCross = true ; break;
    default         :   cln = 2.0;  bCross = true ; break;
    }
    //  ............................................................................................
    cairo_set_source_rgb(_i_cr,
        (double)_i_style->color.red     / 65535.0f  ,
        (double)_i_style->color.green   / 65535.0f  ,
        (double)_i_style->color.blue    / 65535.0f  );
    //  ............................................................................................
    for ( size_t ix = _i_sample_ix1 ; ix <= _i_sample_ix2 ; ix++ )
    {

    x = _i_idata->x(ix);
    y = _i_idata->y(ix);

    pxd =   nxd;
    pyd =   nyd;
    nxd =   -1.0;
    nyd =   -1.0;
    //  ............................................................................................
    //  motif
    if  (   ( x >= atti()->area.a_xmin ) && ( x <= atti()->area.a_xmax )  &&
            ( y >= atti()->area.a_ymin ) && ( y <= atti()->area.a_ymax )  )
    {

        z_coords__area_to_widget_xy( &nxd, &nyd, (double)ax, (double)ay, x, y);
        cairo_set_line_width(_i_cr, mep);

        if ( bCross )
        {
            cairo_move_to(_i_cr, nxd - cln  , nyd           );
            cairo_line_to(_i_cr, nxd + cln  , nyd           );
            cairo_move_to(_i_cr, nxd        , nyd - cln     );
            cairo_line_to(_i_cr, nxd        , nyd + cln     );
        }
        //if ( bPoint )
        //{
        //}
    }
    //  ............................................................................................
    //  join
    if ( _i_style->join )
    {
    if ( ( pxd != -1.0 ) && ( nxd != -1.0 ) )
    {
        cairo_set_line_width(_i_cr, jep);

        cairo_move_to(_i_cr, pxd, pyd);
        cairo_line_to(_i_cr, nxd, nyd);
    }
    }

    }

    cairo_stroke(_i_cr);
}
void
Display::z_draw_signals(cairo_t* _i_ctx)
{
    signal::Model::sCSignal *   ssn =   nullptr;
    //  ............................................................................................
    //printf("z_draw_signals():need to recode\n");

    for ( size_t i = 0 ; i != att()->signals.size() ; i++ )
    {
        printf("z_draw_signals(%lu)\n", i);

        ssn = model()->signal__get_from_uid( att()->signals.at(i).uid );

        z_draw_signal(
            _i_ctx                              ,
            ssn->idata                          ,
            &( att()->signals.at(i).att.style ) ,
            att()->signals.at(i).att.ix1        ,
            att()->signals.at(i).att.ix2        );
    }

}
//  ================================================================================================
void
Display::z_draw_select_marks(cairo_t*   _i_cr)
{
    gint            ax  =   wgt()->allocation.width;
    gint            ay  =   wgt()->allocation.height;
    double          x1, x2, y1, y2;
    //  ............................................................................................
    cairo_set_source_rgb(_i_cr,
        (double)80.0   / 255.0f ,
        (double)80.0   / 255.0f ,
        (double)80.0   / 255.0f );
    cairo_set_line_width(_i_cr, 0.25);

    cairo_set_dash(_i_cr, a_dash_selmarks, 2, 0.0);

    if ( a_select_mark_v.v1_set )
    {
        if  ( ( a_select_mark_v.v1 >= atti()->area.a_xmin ) && ( a_select_mark_v.v1 <= atti()->area.a_xmax ) )
        {
            z_coords__area_to_widget_x( &x1,(double)ax, a_select_mark_v.v1);

            cairo_move_to(_i_cr, x1, 0.0);
            cairo_line_to(_i_cr, x1, ay);
        }
    }
    if ( a_select_mark_v.v2_set )
    {
        if  ( ( a_select_mark_v.v2 >= atti()->area.a_xmin ) && ( a_select_mark_v.v2 <= atti()->area.a_xmax ) )
        {
            z_coords__area_to_widget_x( &x2,(double)ax, a_select_mark_v.v2);

            cairo_move_to(_i_cr, x2, 0.0);
            cairo_line_to(_i_cr, x2, ay);
        }
    }
    if ( a_select_mark_h.v1_set )
    {
        if  ( ( a_select_mark_h.v1 >= atti()->area.a_ymin ) && ( a_select_mark_h.v1 <= atti()->area.a_ymax ) )
        {
            z_coords__area_to_widget_y( &y1,(double)ay, a_select_mark_h.v1);

            cairo_move_to(_i_cr, 0.0, y1);
            cairo_line_to(_i_cr, ax , y1);
        }
    }
    if ( a_select_mark_h.v2_set )
    {
        if  ( ( a_select_mark_h.v2 >= atti()->area.a_ymin ) && ( a_select_mark_h.v2 <= atti()->area.a_ymax ) )
        {
            z_coords__area_to_widget_y( &y2,(double)ay, a_select_mark_h.v2);

            cairo_move_to(_i_cr, 0.0, y2);
            cairo_line_to(_i_cr, ax , y2);
        }
    }
    cairo_stroke(_i_cr);
}
//  ================================================================================================
void
Display::z_draw_axes(cairo_t* _i_ctx)
{
    gint    ax = wgt()->allocation.width;
    gint    ay = wgt()->allocation.height;

    double  y0d, x0d;

    cairo_select_font_face(_i_ctx, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(_i_ctx, 10);

    if (( atti()->axis.x >= atti()->area.a_ymin ) && ( atti()->axis.y <= atti()->area.a_ymax ))
    {

    y0d = ( atti()->axis.x - atti()->area.a_ymin ) / ( atti()->area.a_deltay ) * (double)ay;

    cairo_move_to(_i_ctx, 0 , ay - y0d);
    cairo_line_to(_i_ctx, ax, ay - y0d);

    cairo_set_source_rgb(_i_ctx, 0.0, 0.0, 0.0);
    cairo_set_line_width(_i_ctx, 0.5);
    cairo_stroke(_i_ctx);

    for ( gint i = 1 ; i <= (gint)atti()->rulers.x ; i++ )
    {
        char                    text    [128];
        double                  xp, xr;
        cairo_text_extents_t    te;

        //xp  = 0.0f          + ( (double)(ax * i)            ) / (double)a_rivc.x;
        //xr  = atti()->area.a_xmin   + ( atti()->area.a_deltax * (double)i   ) / (double)a_rivc.x;
        xp  = 0.0f          + ( (double)(ax * i)            ) / (double)( atti()->rulers.x + 1 );
        xr  = atti()->area.a_xmin   + ( atti()->area.a_deltax * (double)i   ) / (double)( atti()->rulers.x + 1 );

        cairo_set_source_rgb    (_i_ctx, 0.0, 0.0, 0.0);
        cairo_set_line_width    (_i_ctx, 1.0);

        cairo_move_to           (_i_ctx, xp, (double)ay - y0d - 4);
        cairo_line_to           (_i_ctx, xp, (double)ay - y0d + 4);
        cairo_stroke            (_i_ctx);

        sprintf(text ,"%+7.4lf", xr);
        cairo_text_extents      (_i_ctx, text, &te);
        cairo_move_to           (_i_ctx, xp - te.width / 2, (double)ay - y0d + te.height + 5);
        cairo_set_source_rgb    (_i_ctx, 0.0, 0.0, 0.0);
        cairo_show_text         (_i_ctx, text);
    }

    }
    if (( atti()->axis.y >= atti()->area.a_xmin ) && ( atti()->axis.y <= atti()->area.a_xmax ))
    {

    x0d = ( atti()->axis.y - atti()->area.a_xmin ) / ( atti()->area.a_deltax ) * (double)ax;

    cairo_set_source_rgb(_i_ctx, 0.0, 0.0, 0.0);
    cairo_set_line_width(_i_ctx, 0.5);

    cairo_move_to(_i_ctx, x0d, 0 );
    cairo_line_to(_i_ctx, x0d, ay);
    cairo_stroke(_i_ctx);

    for ( gint i = 1 ; i <= (gint)atti()->rulers.y ; i++ )
    {
        char                    text    [128];
        double                  yp, yr;
        cairo_text_extents_t    te;

        yp  = 0.0f          + ( (double)(ay * i)            ) / (double)( atti()->rulers.y + 1);
        yr  = atti()->area.a_ymin   + ( atti()->area.a_deltay * (double)i   ) / (double)( atti()->rulers.y + 1);

        cairo_set_source_rgb    (_i_ctx, 0.0, 0.0, 0.0);
        cairo_set_line_width    (_i_ctx, 1.0);

        cairo_move_to           (_i_ctx, x0d - 4, (double)ay - yp);
        cairo_line_to           (_i_ctx, x0d + 4, (double)ay - yp);
        cairo_stroke            (_i_ctx);

        sprintf(text ,"%+7.4lf", yr);
        cairo_text_extents      (_i_ctx, text, &te);
        cairo_move_to           (_i_ctx, x0d + 8, (double)ay - yp + te.height / 2 );
        cairo_set_source_rgb    (_i_ctx, 0.0, 0.0, 0.0);
        cairo_show_text         (_i_ctx, text);
    }

    }
}
//  ================================================================================================
void
Display::z_draw_mouse_coords(cairo_t* _i_ctx, gint _i_mouse_x, gint _i_mouse_y)
{
    static  char                    text    [128]   =   "";
            cairo_text_extents_t    te;
            gint                    ax  = wgt()->allocation.width;
            gint                    ay  = wgt()->allocation.height;
            double                  x,y;

    z_coords__widget_to_area_xy(&x, &y, (double)ax, (double)ay, (double)_i_mouse_x, (double)_i_mouse_y);

    cairo_select_font_face  (_i_ctx, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size     (_i_ctx, 10);

    sprintf                 (text, "%+7.4lf %+7.4lf", x, y);

    cairo_text_extents      (_i_ctx, text, &te);
    cairo_set_source_rgb    (_i_ctx, 0.0, 0.9, 0.9);
    cairo_rectangle         (_i_ctx, 5.0 + te.x_bearing, 10.0 + te.y_bearing, te.width, te.height);
    cairo_stroke_preserve   (_i_ctx);
    cairo_fill              (_i_ctx);

    cairo_set_source_rgb    (_i_ctx, 0.0, 0.0, 0.0);
    cairo_move_to           (_i_ctx, 5.0, 10.0);
    cairo_show_text         (_i_ctx, text);
}

}
}



