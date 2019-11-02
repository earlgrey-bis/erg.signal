                                                                                                    /*
_GWR_TEK_01_
============
We should have pass
    std::vector< signal::Display const * >   const   *   _i_displays
as parameter, but the conversion from
    std::vector< signal::Display       * >
to
    std::vector< signal::Display const * >
is not allowed ( maybe because std::vector is a template ).
So use only the display(i) method that does the conversion.
                                                                                                    */
#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-data.hh"
#include    "signal-main.hh"
#include    "signal-display.hh"

namespace erg
{
namespace signal
{


//  ################################################################################################
//  class PgSignals
//  ################################################################################################
Main::PgSignals::PgSignals(
    Main::Model     *   _i_model    ,
    gpointer            _i_control  )
{
    a_model     =   _i_model;
    a_control   =   _i_control;

    gtkreate();
    refresh();
}

void
Main::PgSignals::refresh()
{
    GList                       *   children;
    GList                       *   child;
    PgSignals::sWidgets01           wgts01;
    signal::IData       const   *   idata;
    char                            s1  [32];
    //  ............................................................................................
    //  delete old widgets from table
    children = gtk_container_get_children(GTK_CONTAINER(d_table_1));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );

        child = g_list_next(child);
    }
    a_widgets01.clear();
    //  ............................................................................................
    //  create new widgets
    Model::sIData       *   idv = nullptr;
    Model::sDisplay     *   dpr = nullptr;
    VPSTORAGE_ENUM_START( &mod()->a_idatas, idv )
    {
        idata   = idv->idata;
        sprintf(s1, "%02lu", 1 + mod()->a_idatas.enum_idx());
        wgts01.d_labl_data_index        =   gtk_label_new(s1);
        wgts01.d_labl_data_name         =   gtk_label_new(idata->name());
        sprintf(s1, "%02lu", idata->crd());
        wgts01.d_labl_data_samples_card =   gtk_label_new(s1);
        wgts01.d_labl_data_type_card    =   gtk_label_new( idata->isDI() ? "Interval" : "Full" );

        wgts01.d_cbxt_display           =   gtk_combo_box_text_new();

        gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts01.d_cbxt_display), "none");

        VPSTORAGE_ENUM_START( &mod()->a_displays, dpr )
        {
            sprintf(s1, "%02lu - %s", 1 + mod()->a_displays.enum_idx(), "noname");
            gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts01.d_cbxt_display), s1);
            VPSTORAGE_ENUM_NEXT( &mod()->a_displays, dpr );
        }

        if ( idv->display_id != Model::sInvalidDisplayId )
        {
            gtk_combo_box_set_active    ( GTK_COMBO_BOX(wgts01.d_cbxt_display), idv->display_id);
        }
        else
        {
            gtk_combo_box_set_active    ( GTK_COMBO_BOX(wgts01.d_cbxt_display), 0);
        }

        //  ........................................................................................
        //  connect signals of new widgets
        Main::ViewCallbackData  *   vcd =   new Main::ViewCallbackData(a_control, (gpointer)idv, 0);
        g_signal_connect( wgts01.d_cbxt_display, "changed", G_CALLBACK(Main::GtkEvent__pgsignals__changed__comboboxtext_display), (gpointer)vcd );


        a_widgets01.push_back(wgts01);
        VPSTORAGE_ENUM_NEXT( &mod()->a_idatas, idv );
    }
    //  ............................................................................................
    //  pack widgets
    for ( size_t i = 0 ; i != a_widgets01.size() ; i++ )
    {
        gint    x       =  -1;
                wgts01  = a_widgets01.at(i);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_index        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_name         , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_samples_card , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_cbxt_display           , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
    }

    gtk_widget_show_all(d_table_1);
}

void
Main::PgSignals::gtkreate()
{
    sWidgets01              wgts01;
    signal::IData       *   idata;
    signal::sDrawStyle  *   style;
    char                    s1  [32];
    //  ............................................................................................
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);

      d_frame_1                         =   gtk_frame_new("Signals");

        d_table_1                       =   gtk_table_new(0, 5, FALSE);

      d_frame_2                         =   gtk_frame_new("Others");
    //  ............................................................................................
    g_object_set                ( d_table_1, "border-width", 5, NULL);                              // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_1), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_1), 5 );
    //  ............................................................................................
    gtk_container_add   ( GTK_CONTAINER(d_frame_1)  , d_table_1);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_1, FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_2,  TRUE,  TRUE, 0);
}
//  ################################################################################################
//  class PgDisplays
//  ################################################################################################
Main::PgDisplays::PgDisplays(Main::Model * _i_model, gpointer _i_control)
{
    a_model     =   _i_model;
    a_control   =   _i_control;
    gtkreate();
    refresh();
}
void
Main::PgDisplays::gtkreate()
{
    sWidgets01              wgts01;
    signal::IData       *   idata;
    signal::sDrawStyle  *   style;
    char                    s1  [32];
    //  ............................................................................................
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);

      d_frame_1                         =   gtk_frame_new("Displays");

        d_vbox_2                        =   gtk_vbox_new(FALSE, 2);
          d_table_1                     =   gtk_table_new(0, 5, FALSE);
          d_pshb_add_display            =   gtk_button_new_with_label("Add display");

      d_frame_2                         =   gtk_frame_new("Others");
    //  ............................................................................................
    g_object_set                ( d_table_1, "border-width", 5, NULL);                              // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_1), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_1), 5 );
    //  ............................................................................................
    gtk_box_pack_start  ( GTK_BOX(d_vbox_2)         , d_table_1             , FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_2)         , d_pshb_add_display    , FALSE, FALSE, 0);
    gtk_container_add   ( GTK_CONTAINER(d_frame_1)  , d_vbox_2);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_1, FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_2,  TRUE,  TRUE, 0);
    //  ............................................................................................
    Main::ViewCallbackData  *   vcd =   new Main::ViewCallbackData(a_control);
    g_signal_connect(d_pshb_add_display, "button-press-event", G_CALLBACK(Main::GtkEvent__pgdisplays__button_press_event__add_display), (gpointer)vcd);
}

void
Main::PgDisplays::refresh()
{
    GList                           *   children;
    GList                           *   child;
    PgDisplays::sWidgets01              wgts01;
    signal::Display         const   *   display;
    char                                s1  [32];
    //  ............................................................................................
    //  delete old widgets from table
    children = gtk_container_get_children(GTK_CONTAINER(d_table_1));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );

        child = g_list_next(child);
    }
    a_widgets01.clear();
    //  ............................................................................................
    //  create new widgets
    Model::sIData       *   idv = nullptr;
    Model::sDisplay     *   dpr = nullptr;
    VPSTORAGE_ENUM_START( &mod()->a_displays, dpr )
    {
        display                         =   dpr->display;

        sprintf(s1, "%02lu", 1 + mod()->a_displays.enum_idx());
        wgts01.d_labl_display_index     =   gtk_label_new(s1);
        wgts01.d_labl_display_name      =   gtk_label_new("noname");

        wgts01.d_vsep_1                 =   gtk_vseparator_new();

        wgts01.d_adjs_xmin_def          =   gtk_adjustment_new(0.0, 0.0, 2.0, 0.1, 0.1, 0.0);
        wgts01.d_spib_xmin_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_xmin_def), 0.0, 3);
        wgts01.d_adjs_xmax_def          =   gtk_adjustment_new(1.0, 0.0, 2.0, 0.1, 0.1, 0.0);
        wgts01.d_spib_xmax_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_xmax_def), 0.0, 3);
        wgts01.d_adjs_ymin_def          =   gtk_adjustment_new(-0.5, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts01.d_spib_ymin_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_ymin_def), 0.0, 3);
        wgts01.d_adjs_ymax_def          =   gtk_adjustment_new(+0.5, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts01.d_spib_ymax_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_ymax_def), 0.0, 3);

        wgts01.d_pshb_area_cur_to_def   =   gtk_button_new_with_label("<");
        wgts01.d_pshb_area_def_to_cur   =   gtk_button_new_with_label(">");

        wgts01.d_labl_area_current      =   gtk_label_new("[0.0 0.0 0.0 0.0]");

        wgts01.d_vsep_2                 =   gtk_vseparator_new();

        wgts01.d_adjs_rulers_x          =   gtk_adjustment_new(2.0, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts01.d_spib_rulers_x          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_rulers_x), 0.0, 0);
        wgts01.d_adjs_rulers_y          =   gtk_adjustment_new(2.0, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts01.d_spib_rulers_y          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_rulers_y), 0.0, 0);

        wgts01.d_adjs_axis_x            =   gtk_adjustment_new(2.0, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts01.d_spib_axis_x            =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_rulers_x), 0.0, 0);
        wgts01.d_adjs_axis_y            =   gtk_adjustment_new(2.0, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts01.d_spib_axis_y            =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts01.d_adjs_rulers_y), 0.0, 0);

        wgts01.d_colb_bgcolor           =   gtk_color_button_new();

        a_widgets01.push_back(wgts01);
        VPSTORAGE_ENUM_NEXT( &mod()->a_displays, dpr )
    }
    //  ............................................................................................
    //  pack widgets
    for ( size_t i = 0 ; i != a_widgets01.size() ; i++ )
    {
        gint    x       =  -1;
                wgts01  = a_widgets01.at(i);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_display_index      , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_display_name       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_vsep_1                  , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_xmin_def           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_xmax_def           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_ymin_def           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_ymax_def           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_pshb_area_cur_to_def    , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_pshb_area_def_to_cur    , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_area_current       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_vsep_2                  , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_rulers_x           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_rulers_y           , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_axis_x             , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_spib_axis_y             , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_colb_bgcolor            , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
    }
    //  ............................................................................................
    //  connect signals of new widgets
    //for ( size_t i = 0 ; i != mod()->a_displays_params.size() ; i++ )
    //{
    //    wgts01                          = a_widgets01.at(i);
    //    Main::sCallbackData     *   cbd = ERGC_NEW0(Main::sCallbackData);

    //    cbd->instance       =   a_control;
    //    cbd->szt1           =   i;

    //    g_signal_connect( wgts01.d_chkb_draw, "toggled", G_CALLBACK(Main::GtkEvent__pgdisplays__toggled__checkbutton_draw), (gpointer)cbd );
    //}
    //  ............................................................................................

    gtk_widget_show_all(d_table_1);
}
//  ################################################################################################
//  class PgDisplay
//  ################################################################################################
Main::PgDisplay::PgDisplay(Main::Model* _i_model, gpointer _i_control)
{
    a_model     =   _i_model;
    a_control   =   _i_control;
    gtkreate();
}
/*
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
        wgts01                          =   a_widgets01.at(i);
        Main::ViewCallbackData  *   vcd =   new Main::ViewCallbackData(a_control, i);
        g_signal_connect( wgts01.d_chkb_draw, "toggled", G_CALLBACK(GtkEvent_toggled__checkbutton_draw), (gpointer)vcd );
    }
}
*/
void
Main::PgDisplay::gtkreate()
{
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);
}


//  ################################################################################################
//  class Main
//  ################################################################################################



Main::Main()
{
    d_model = new Model();

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

    d_nbpg_displays =   new Main::PgDisplays    (model(), (gpointer)this);
    d_nbpg_signals  =   new Main::PgSignals     (model(), (gpointer)this);
    d_nbpg_display  =   new Main::PgDisplay     (model(), (gpointer)this);

    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_displays->wgt()   , gtk_label_new("Displays")  );
    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_signals->wgt()    , gtk_label_new("Signals")  );
    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_display->wgt()    , gtk_label_new("Display") );

    gtk_container_add(GTK_CONTAINER(d_window), d_nbok);
}

void
Main::view_refresh()
{
    d_nbpg_displays ->refresh();
    d_nbpg_signals  ->refresh();
    //d_nbpg_display  ->refresh();
}

gboolean
Main::GtkEvent__pgdisplays__button_press_event__add_display(
    GtkWidget   *   _i_widget       ,
    GdkEvent    *   _i_evt          ,
    gpointer        _i_data         )
{
    Main                        *   THIS    =   (Main*)( ((Main::ViewCallbackData*)_i_data)->a_instance );
    Main::Model::sDisplay       *   dp      =   ERGC_NEW0(Model::sDisplay);
    size_t                          ix      =   THIS->model()->a_displays.next_add_index();
    //  ............................................................................................
    dp->area.xmin   =   -0.01;
    dp->area.xmax   =   +0.05;
    dp->area.ymin   =   -0.05;
    dp->area.ymax   =   +0.05;
    dp->area.deltax =   0.06;
    dp->area.deltay =   0.1;

    dp->rulers.x    =   4;
    dp->rulers.y    =   2;

    dp->axis.x      =   0.0;
    dp->axis.y      =   0.0;

    dp->bgcolor.r   =   255;
    dp->bgcolor.g   =   0;
    dp->bgcolor.b   =   0;

    dp->display     =   new signal::Display(ix, 100, 300);

    //dp->display->set_area(dp->area.xmin, dp->area.xmax, dp->area.ymin, dp->area.ymax);
    //dp->display->set_axes(dp->axis.x, dp->axis.y);
    //dp->display->set_rulers_card(dp->rulers.x, dp->rulers.y);

    THIS->model()->display__add(dp);

    THIS->view_refresh();

    return FALSE;
}

void
Main::GtkEvent__pgdisplay__toggled__checkbutton_draw(
    GtkToggleButton *   _i_togglebutton ,
    gpointer            _i_data         )
{
    Main::ViewCallbackData  *   vcd =   (Main::ViewCallbackData*)_i_data;
}

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


}
}



