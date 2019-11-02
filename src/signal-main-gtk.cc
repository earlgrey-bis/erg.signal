#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-main.hh"
#include    "signal-data.hh"

namespace erg
{
namespace signal
{


//  ################################################################################################
//  class PgMain
//  ################################################################################################
Main::PgMain::PgMain()
{
    gtkreate();
}
Main::PgMain::PgMain(
    std::vector< signal::IData* >       &   _i_datas    ,
    std::vector< signal::sDrawStyle* >  &   _i_styles   )
{
    gtkreate();
}

void
Main::PgMain::gtkreate()
{
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);
}
//  ################################################################################################
//  class PgDisplays
//  ################################################################################################
Main::PgDisplays::PgDisplays()
{
    gtkreate();
}
void
Main::PgDisplays::gtkreate()
{
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);
}
//  ################################################################################################
//  class PgDisplay
//  ################################################################################################
void
Main::PgDisplay::GtkEvent_toggled__checkbutton_draw(
    GtkToggleButton *   _i_togglebutton ,
    gpointer            _i_data         )
{
    PgDisplay::sWidgetCallbackData  *   wcd =   (PgDisplay::sWidgetCallbackData*)_i_data;

    PgDisplay                       *   pgs =   (PgDisplay*)wcd->instance;
    size_t              ix  =   wcd->signal_index;

    //if (

}


Main::PgDisplay::PgDisplay()
{
    gtkreate();
}

Main::PgDisplay::PgDisplay(
    std::vector< signal::IData* >       &   _i_datas    ,
    std::vector< signal::sDrawStyle* >  &   _i_styles   )
{
    sWidgets01              wgts01;
    signal::IData       *   idata;
    signal::sDrawStyle  *   style;
    char                    s1  [32];
    //  ............................................................................................
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);

      d_frame_1                         =   gtk_frame_new("General");

        d_table_1                       =   gtk_table_new(_i_datas.size(), 14, FALSE);

        d_adjs_style_ep                 =   gtk_adjustment_new(1.0, 0.5, 2.0, 0.25, 0.25, 0.0);

        for ( size_t i = 0 ; i != _i_datas.size() ; i++ )
        {
            idata   = _i_datas.at(i);
            style   = _i_styles.at(i);
            sprintf(s1, "%02lu", i);

            wgts01.d_labl_index             =   gtk_label_new(s1);

            wgts01.d_chkb_draw              =   gtk_check_button_new_with_label(idata->name());
            ////gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(wgts01.d_chkb_draw), style->draw );

            wgts01.d_vsep_1                 =   gtk_vseparator_new();

            wgts01.d_labl_nsamples          =   gtk_label_new("512");//_i_signal_samples_card);
            wgts01.d_adjs_xmin              =   gtk_adjustment_new(0.0, 0.0, 511.0, 1.0, 10.0, 0.0);
            wgts01.d_spib_xmin              =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_xmin), 0.0, 0);
            wgts01.d_adjs_xmax              =   gtk_adjustment_new(511.0, 0.0, 511.0, 1.0, 10.0, 0.0);
            wgts01.d_spib_xmax              =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_xmax), 0.0, 0);

            wgts01.d_vsep_2                 =   gtk_vseparator_new();

            wgts01.d_cbxt_samples_select    =   gtk_combo_box_text_new();
            wgts01.d_adjs_sample_ix1        =   gtk_adjustment_new(0.0, 0.0, 511.0, 1.0, 10.0, 0.0);
            wgts01.d_spib_sample_ix1        =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_sample_ix1), 0.0, 0);
            wgts01.d_adjs_sample_ix2        =   gtk_adjustment_new(511.0, 0.0, 511.0, 1.0, 10.0, 0.0);
            wgts01.d_spib_sample_ix2        =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_sample_ix2), 0.0, 0);

            wgts01.d_vsep_3                 =   gtk_vseparator_new();

            wgts01.d_cbxt_style             =   gtk_combo_box_text_new();
            wgts01.d_spib_style_ep          =   gtk_spin_button_new(GTK_ADJUSTMENT(d_adjs_style_ep), 0.0, 2);
            wgts01.d_colb_style_color       =   gtk_color_button_new();

            a_widgets01.push_back(wgts01);
        }

      d_frame_2                         =   gtk_frame_new("Others");
    //  ............................................................................................
    g_object_set                ( d_table_1, "border-width", 5, NULL);                              // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_1), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_1), 5 );
    //  ............................................................................................
    for ( size_t i = 0 ; i != _i_datas.size() ; i++ )
    {
        gint    x       =  -1;
                wgts01  = a_widgets01.at(i);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_index             , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_chkb_draw              , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_vsep_1                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_nsamples          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_xmin              , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_xmax              , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_vsep_2                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_cbxt_samples_select    , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_sample_ix1        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_sample_ix2        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_vsep_3                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_cbxt_style             , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_style_ep          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_colb_style_color       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
    }

    gtk_container_add   ( GTK_CONTAINER(d_frame_1)  , d_table_1);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_1, FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_2,  TRUE,  TRUE, 0);
    //  ............................................................................................
    for ( size_t i = 0 ; i != _i_datas.size() ; i++ )
    {
        wgts01                          = a_widgets01.at(i);
        sWidgetCallbackData     *   cbd = ERGC_NEW0(sWidgetCallbackData);

        cbd->instance       = (gpointer)this;
        cbd->signal_index   = i;

        g_signal_connect( wgts01.d_chkb_draw, "toggled", G_CALLBACK(GtkEvent_toggled__checkbutton_draw), (gpointer)cbd );
    }
}

void
Main::PgDisplay::gtkreate()
{
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);
}
//  ################################################################################################
//  class Main
//  ################################################################################################

//  ================================================================================================
//  Model
//  ================================================================================================
//void
//Main::data_add(
//    signal::IData   const   *   _i_idata)
//{
//    a_idatas.push_back(_i_idata);
//}

void
Main::data_add(
    signal::IData       const   *   _i_idata    ,
    signal::sDrawStyle          *   _i_style    )
{
    a_idatas.push_back(_i_idata);
    a_styles.push_back(_i_style);
}

void
Main::data_add(
    signal::IData       const   *   _i_idata    ,
    signal::eDrawMotif              _i_motif    ,
    guint8                          _i_r        ,
    guint8                          _i_g        ,
    guint8                          _i_b        )
{
    signal::sDrawStyle  *   style;
    GdkColor                color;
    //  ............................................................................................
    color.red       = (65535/255) * _i_r;
    color.green     = (65535/255) * _i_g;
    color.blue      = (65535/255) * _i_b;
    color.pixel = (gulong)(color.red * 65536 + color.green * 256 + color.blue);

    style = ERGC_NEW0(signal::sDrawStyle);

    style->color    = color;
    style->motif    = _i_motif;

    return data_add(_i_idata, style);
}

Main::Main()
{
    gtkreate();

    gtk_widget_show_all(d_window);
}

Main::Main(
    std::vector< signal::IData* >       &   _i_datas    ,
    std::vector< signal::sDrawStyle* >  &   _i_styles   )
{
    gtkreate();
}

void
Main::gtkreate()
{
    d_window        =   gtk_window_new(GTK_WINDOW_TOPLEVEL);

    d_nbok          =   gtk_notebook_new();

    d_nbpg_main     =   new Main::PgMain();
    d_nbpg_displays =   new Main::PgDisplays();
    d_nbpg_display  =   new Main::PgDisplay();

    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_main->wgt()       , gtk_label_new("Signals")  );
    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_displays->wgt()   , gtk_label_new("Displays")  );
    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_display->wgt()    , gtk_label_new("Display") );

    gtk_container_add(GTK_CONTAINER(d_window), d_nbok);
}





}
}



