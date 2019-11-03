#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
//#define     TKI(FORMAT, ...)

#include    "signal-data.hh"
#include    "signal-view.hh"
#include    "signal-control.hh"
#include    "signal-display.hh"

namespace erg
{
namespace signal
{

View::PgDisplays::sWidgets10    *
View::PgDisplays::widgets10__get_from_display_uid(size_t _i_display_uid)
{
    for ( size_t i = 0 ; i != a_widgets10.size() ; i++ )
    {
        if ( a_widgets10.at(i).a_display_uid == _i_display_uid )
            return &( a_widgets10.at(i) );
    }

    return nullptr;
}
/*
View::PgDisplays::sWidgets20    *
View::PgDisplays::widgets20__get_from_signal_uid(size_t _i_signal_uid)
{
    for ( size_t i = 0 ; i != a_widgets20.size() ; i++ )
    {
        if ( a_widgets20.at(i).a_data_uid == _i_signal_uid )
            return &( a_widgets20.at(i) );
    }

    return nullptr;
}
*/

View::PgDisplays::PgDisplays(
    signal::Model   *   _i_model    ,
    signal::Control *   _i_control  )
    :   MemberModel     ( _i_model  )   ,
        MemberControl   ( _i_control)
{
    gtkreate();
    refresh();
}
void
View::PgDisplays::gtkreate()
{
    View::CallbackData  *   vcd     =   nullptr;
    //  ............................................................................................
    a_rdbt_selected_display_uid         =   0;
    //  ............................................................................................
    d_vbox                              =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

      d_frame_10                        =   gtk_frame_new("All displays");

        d_vbox_10                       =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
          d_table_10                    =   gtk_table_new(0, 5, FALSE);
          d_pshb_add_display            =   gtk_button_new_with_label("Add display");

      d_frame_20                        =   gtk_frame_new("Others");

        //d_trvw_20                       =   gtk_tree_view_new();
        //d_lsto_20                       =   gtk_list_store_new(
        d_vbox_20                       =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
          d_table_20                    =   gtk_table_new(0, 5, FALSE);
    //  ............................................................................................
    g_object_set                ( d_table_10, "border-width", 5, NULL);                             // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_10), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_10), 5 );

    g_object_set                ( d_table_20, "border-width", 5, NULL);                             // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_20), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_20), 5 );
    //  ............................................................................................
    gtk_box_pack_start  ( GTK_BOX(d_vbox_10)        , d_table_10            , FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_10)        , d_pshb_add_display    , FALSE, FALSE, 0);
    gtk_container_add   ( GTK_CONTAINER(d_frame_10) , d_vbox_10);

    gtk_box_pack_start  ( GTK_BOX(d_vbox_20)        , d_table_20            , FALSE, FALSE, 0);
    //gtk_box_pack_start  ( GTK_BOX(d_vbox_20)         , d_pshb_add_display    , FALSE, FALSE, 0);
    gtk_container_add   ( GTK_CONTAINER(d_frame_20) , d_vbox_20);

    gtk_box_pack_start  ( GTK_BOX(d_vbox)           , d_frame_10,   FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox)           , d_frame_20,    TRUE,  TRUE, 0);
    //  ............................................................................................
    vcd = new View::CallbackData(a_control);
    g_signal_connect(
        d_pshb_add_display                                                                  ,
        "button-press-event"                                                                ,
        G_CALLBACK(signal::Control::GtkEvent__pgdisplays__button_press_event__add_display)  ,
        (gpointer)vcd                                                                       );
}

void
View::PgDisplays::refresh()
{
    refresh_frame1();
    refresh_frame2();
}
//  ================================================================================================
//  frame 1
//  ================================================================================================
GtkWidget   *
View::PgDisplays::rf1_create_rdbt(
    size_t                      _i_enum_idx     ,
    size_t                      _i_display_uid  )
{
    char                        str     [32];
    GtkWidget               *   rdbt    =   nullptr;
    //  ............................................................................................
    sprintf(str, "%02lu", _i_display_uid);

    if ( _i_enum_idx == 0 )
    {
        rdbt    =   gtk_radio_button_new_with_label(NULL, str);
    }
    else
    {
        rdbt    =   gtk_radio_button_new_with_label_from_widget(
                        GTK_RADIO_BUTTON(a_widgets10.at(0).d_rdbt_display_uid)  ,
                        str                                                     );
    }

    return rdbt;
}
void
View::PgDisplays::refresh_frame1()
{
    GList                           *   children;
    GList                           *   child;

    Model::sCDisplay                *   sdp     =   nullptr;
    signal::Display     const       *   dpl     =   nullptr;
    Model::sDisplayAtt  const       *   att     =   nullptr;

    PgDisplays::sWidgets10              wgts10;
    char                                str1    [32];
    View::CallbackData              *   vcd     =   nullptr;
    //  ............................................................................................
    //  delete old widgets from table
    children = gtk_container_get_children(GTK_CONTAINER(d_table_10));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );

        child = g_list_next(child);
    }
    a_widgets10.clear();
    //  ............................................................................................
    //  create new widgets
    for ( sdp = mod()->display__enum_start() ; sdp != nullptr ; sdp = mod()->display__enum_next() )
    {
        dpl                             =   sdp->display;
        att                             =   sdp->att;

        wgts10.d_rdbt_display_uid       =   rf1_create_rdbt(
                                                mod()->display__enum_idx()  ,
                                                dpl->uid()                  );
        wgts10.a_display_uid            =   dpl->uid();

        wgts10.d_labl_display_name      =   gtk_label_new(att->d.name.c_str());

        wgts10.d_vsep_1                 =   gtk_separator_new(GTK_ORIENTATION_VERTICAL);

        wgts10.d_adjs_xmin_def          =   gtk_adjustment_new(att->i.area.a_xmin, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts10.d_spib_xmin_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_xmin_def), 0.0, 5);
        wgts10.d_adjs_xmax_def          =   gtk_adjustment_new(att->i.area.a_xmax, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts10.d_spib_xmax_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_xmax_def), 0.0, 5);
        wgts10.d_adjs_ymin_def          =   gtk_adjustment_new(att->i.area.a_ymin, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts10.d_spib_ymin_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_ymin_def), 0.0, 5);
        wgts10.d_adjs_ymax_def          =   gtk_adjustment_new(att->i.area.a_ymax, -1.0, 1.0, 0.1, 0.1, 0.0);
        wgts10.d_spib_ymax_def          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_ymax_def), 0.0, 5);

        wgts10.d_pshb_area_cur_to_def   =   gtk_button_new_with_label("<");
        wgts10.d_pshb_area_def_to_cur   =   gtk_button_new_with_label(">");

        wgts10.d_labl_area_current      =   gtk_label_new("[0.0 0.0 0.0 0.0]");

        wgts10.d_vsep_2                 =   gtk_separator_new(GTK_ORIENTATION_VERTICAL);

        wgts10.d_adjs_rulers_x          =   gtk_adjustment_new(att->i.rulers.x, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts10.d_spib_rulers_x          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_rulers_x), 0.0, 0);
        wgts10.d_adjs_rulers_y          =   gtk_adjustment_new(att->i.rulers.y, 0.0, 10.0, 1.0, 1.0, 0.0);
        wgts10.d_spib_rulers_y          =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_rulers_y), 0.0, 0);

        wgts10.d_adjs_axis_x            =   gtk_adjustment_new(att->i.axis.x, -2.0, 2.0, 0.01, 0.1, 0.0);
        wgts10.d_spib_axis_x            =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_axis_x), 0.0, 2);
        wgts10.d_adjs_axis_y            =   gtk_adjustment_new(att->i.axis.y, -2.0, 2.0, 0.01, 0.1, 0.0);
        wgts10.d_spib_axis_y            =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts10.d_adjs_axis_y), 0.0, 2);

        wgts10.d_colb_bgcolor           =   gtk_color_button_new();

        a_widgets10.push_back(wgts10);
    }
    //  ............................................................................................
    //  pack widgets
    for ( size_t i = 0 ; i != a_widgets10.size() ; i++ )
    {
        gint    x       =  -1;
                wgts10  = a_widgets10.at(i);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_rdbt_display_uid       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_labl_display_name      , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_vsep_1                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_xmin_def          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_xmax_def          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_ymin_def          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_ymax_def          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        //x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_pshb_area_cur_to_def    , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        //x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_pshb_area_def_to_cur    , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        //x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_area_current       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_vsep_2                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_rulers_x          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_rulers_y          , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_axis_x            , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_spib_axis_y            , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_10), wgts10.d_colb_bgcolor           , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
    }
    //  ............................................................................................
    //  select the good radiobutton, while signals are not connected
    if ( a_rdbt_selected_display_uid == 0 )                                                         // first pass / no display before
    {
        if ( a_widgets10.size() != 0 )                                                              // displays exist
        {
            a_rdbt_selected_display_uid =   a_widgets10.at(0).a_display_uid;                        // take the first one
        }
    }
    else                                                                                            // a display was previously selected
    {
        sWidgets10* wgts10 = widgets10__get_from_display_uid(a_rdbt_selected_display_uid);

        if ( wgts10 )                                                                               // found it in the new vector
        {
            gtk_toggle_button_set_active(                                                           // select it
                GTK_TOGGLE_BUTTON(wgts10->d_rdbt_display_uid)   ,
                TRUE                                            );
        }
        else
        {
            if ( a_widgets10.size() != 0 )
            {
                a_rdbt_selected_display_uid =   a_widgets10.at(0).a_display_uid;
            }
            else
            {
                a_rdbt_selected_display_uid =   0;
            }
        }
    }
    //  ............................................................................................
    //  now connect signals for radiobuttons
    for ( size_t i = 0 ; i != a_widgets10.size() ; i++ )
    {
        vcd = new View::CallbackData(this, nullptr, a_widgets10.at(i).a_display_uid);

        g_signal_connect(
            a_widgets10.at(i).d_rdbt_display_uid                                                ,
            "toggled"                                                                           ,
            G_CALLBACK(View::PgDisplays::GtkEvent__pgdisplays10__display__toggled__checkbutton) ,
            (gpointer)vcd                                                                       );
    }
    //  ............................................................................................

    gtk_widget_show_all(d_table_10);
}

void
View::PgDisplays::GtkEvent__pgdisplays10__display__toggled__checkbutton(
    GtkToggleButton *   _i_togglebutton ,
    gpointer            _i_data         )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    //  ............................................................................................
    printf("toggled\n");
    if ( gtk_toggle_button_get_active( _i_togglebutton ) )
    {
        THIS->a_rdbt_selected_display_uid = vcd->a_szt1;
    }
    THIS->refresh_frame2();
}
//  ================================================================================================
//  frame 2
//  ================================================================================================
void
View::PgDisplays::refresh_frame2()
{

    GList                               *   children;
    GList                               *   child;

    PgDisplays::sWidgets20                  wgts20;

    Model::sCDisplay                    *   sdp     =   nullptr;
    Model::sDisplayedSignal     const   *   sds     =   nullptr;

    Model::sCSignal                     *   ssn     =   nullptr;
    signal::IData               const   *   idata;
    Model::sSignalAtt           const   *   att     =   nullptr;

    char                                    s1          [32];
    View::CallbackData                  *   vcd     =   nullptr;
    //  ............................................................................................
    //  delete old widgets from table
    children = gtk_container_get_children(GTK_CONTAINER(d_table_20));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );

        child = g_list_next(child);
    }
    a_widgets20.clear();
    //  ............................................................................................
    //  get selected display
    if ( a_rdbt_selected_display_uid != 0 )
    {
        sdp = mod()->display__get_from_uid(a_rdbt_selected_display_uid);
    }
    //  ............................................................................................
    //  do everything here because widgets depends on displays too
    for ( ssn = mod()->signal__enum_start() ; ssn != nullptr ; ssn = mod()->signal__enum_next() )
    {
                                        idata   =   ssn->idata;
                                        att     =   ssn->att;

        signal::sDrawStyle  const   *   style   = nullptr;

        if ( sdp )
        {
            sds     =   model()->display__get_signal( sdp, idata->uid() );
            style   =   &( sds->att.style );
        }

        size_t  i       =   mod()->signal__enum_idx();
        gint    x       =  -1;

        //  ........................................................................................
        //  base widgets : checkbutton, uid, name, ix1, ix2
        wgts20.a_data_uid               =   idata->uid();

        wgts20.d_chkb_data_draw         =   gtk_check_button_new();

        sprintf(s1, "%02lu", idata->uid());
        wgts20.d_labl_data_index        =   gtk_label_new(s1);
        wgts20.d_labl_data_sname        =   gtk_label_new(att->d.sname.c_str());

        sprintf(s1, "%02lu", idata->crd());
        wgts20.d_labl_data_samples_card =   gtk_label_new(s1);

        //  pack
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_chkb_data_draw         , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_labl_data_index        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_labl_data_sname        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_labl_data_samples_card , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        //  signals
        vcd = new View::CallbackData(this, nullptr, wgts20.a_data_uid);

        //  no display
        if ( ! sdp )
        {
            gtk_widget_set_sensitive(wgts20.d_chkb_data_draw, false);
            goto lab_push;
        }

        //  signal not drawed in display ?
        if ( ! sds )
        {
            g_signal_connect(
                wgts20.d_chkb_data_draw                                                             ,
                "toggled"                                                                           ,
                G_CALLBACK(View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton)  ,
                (gpointer)vcd                                                                       );

            goto lab_push;
        }
        //  ........................................................................................
        //  drawed signal

        //  activate the checkbutton
        gtk_toggle_button_set_active(                                                               // this does send a "toggled" signal
            GTK_TOGGLE_BUTTON(wgts20.d_chkb_data_draw)  ,
            TRUE                                        );

        //  connect it only after activation for avoiding infinite loop
        g_signal_connect(
            wgts20.d_chkb_data_draw                                                             ,
            "toggled"                                                                           ,
            G_CALLBACK(View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton)  ,
            (gpointer)vcd                                                                       );

        // create additional widgets
        wgts20.d_vsep_1                 =   gtk_separator_new(GTK_ORIENTATION_VERTICAL);

        wgts20.d_adjs_signal_ix1        =   gtk_adjustment_new( 0.0, 0.0, idata->crd() - 1, 1.0, .0, 0.0);
        wgts20.d_spib_signal_ix1        =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts20.d_adjs_signal_ix1), 0.0, 0);
        wgts20.d_adjs_signal_ix2        =   gtk_adjustment_new( idata->crd() - 1, 0.0, idata->crd() - 1, 1.0, 10.0, 0.0);
        wgts20.d_spib_signal_ix2        =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts20.d_adjs_signal_ix2), 0.0, 0);

        wgts20.d_cbbx_signal_motif      =   gtk_combo_box_text_new();

        wgts20.d_adjs_signal_ep         =   gtk_adjustment_new( style->motif_ep, 0.25, 2.0, 0.25, 0.25, 0.0);
        wgts20.d_spib_signal_ep         =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts20.d_adjs_signal_ep), 0.0, 2);

        wgts20.d_chkb_signal_join       =   gtk_check_button_new();
        wgts20.d_adjs_signal_join_ep    =   gtk_adjustment_new( style->join_ep, 0.25, 2.0, 0.25, 0.25, 0.0);
        wgts20.d_spib_signal_join_ep    =   gtk_spin_button_new(GTK_ADJUSTMENT(wgts20.d_adjs_signal_join_ep), 0.0, 2);

        wgts20.d_colb_signal            =   gtk_color_button_new();

        gtk_color_button_set_color      ( GTK_COLOR_BUTTON  (wgts20.d_colb_signal       ), & sds->att.style.color );
        gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts20.d_cbbx_signal_motif ), "cross 1");
        gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts20.d_cbbx_signal_motif ), "cross 2");
        gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts20.d_cbbx_signal_motif ), "cross 3");
        gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts20.d_cbbx_signal_motif ), "cross 4");
        gtk_combo_box_set_active        ( GTK_COMBO_BOX     (wgts20.d_cbbx_signal_motif ), sds->att.style.motif);

        //  join signal dots ?
        if ( style->join )
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(wgts20.d_chkb_signal_join), TRUE );
            gtk_widget_set_sensitive(wgts20.d_spib_signal_join_ep, TRUE);
        }
        else
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(wgts20.d_chkb_signal_join), FALSE );
            gtk_widget_set_sensitive(wgts20.d_spib_signal_join_ep, FALSE);
        }

        //  pack
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_vsep_1                 , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_FILL      , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_spib_signal_ix1        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_spib_signal_ix2        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_cbbx_signal_motif      , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_spib_signal_ep         , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_chkb_signal_join       , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_spib_signal_join_ep    , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_20), wgts20.d_colb_signal            , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        //  signals
        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        g_signal_connect(wgts20.d_cbbx_signal_motif, "changed"          , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__changed__cbbx), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        vcd->a_szt3         =   1;                                                                  //  1 = ix1
        g_signal_connect(wgts20.d_adjs_signal_ix1, "value-changed"      , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__value_changed__adj), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        vcd->a_szt3         =   2;                                                                  //  2 = ix2
        g_signal_connect(wgts20.d_adjs_signal_ix2, "value-changed"      , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__value_changed__adj), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        vcd->a_szt3         =   3;                                                                  //  3 = sgn ep
        g_signal_connect(wgts20.d_adjs_signal_ep, "value-changed"       , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__value_changed__adj), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        vcd->a_szt3         =   4;                                                                  //  4 = sgn join ep
        g_signal_connect(wgts20.d_adjs_signal_join_ep, "value-changed"  , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__value_changed__adj), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        g_signal_connect(wgts20.d_chkb_signal_join, "toggled"           , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton_join), (gpointer)vcd);

        vcd = new View::CallbackData(this);
        vcd->a_szt1         =   a_rdbt_selected_display_uid;
        vcd->a_szt2         =   wgts20.a_data_uid;
        g_signal_connect(wgts20.d_colb_signal, "color-set"              , G_CALLBACK(PgDisplays::GtkEvent__pgdisplays20__signal__color_set__colorbutton), (gpointer)vcd);


        //  ........................................................................................
lab_push:
        a_widgets20.push_back(wgts20);
    }

    gtk_widget_show_all(d_table_20);

}

void
View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton(
    GtkToggleButton *   _i_togglebutton ,
    gpointer            _i_data         )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    //  ............................................................................................
    printf("checked-toggled\n");
    if ( THIS->a_rdbt_selected_display_uid == 0 )
        return;

    THIS->control()->display__add_signal(THIS->a_rdbt_selected_display_uid, vcd->a_szt1);
}
void
View::PgDisplays::GtkEvent__pgdisplays20__signal__color_set__colorbutton(
    GtkColorButton  *   _i_colorbutton  ,
    gpointer            _i_data         )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    GdkColor                        color;
    //  ............................................................................................
    printf("color-set\n");

    gtk_color_button_get_color( _i_colorbutton, &color );

    THIS->control()->display__chg_signal_color(
        vcd->a_szt1     ,
        vcd->a_szt2     ,
        &color          );
}
void
View::PgDisplays::GtkEvent__pgdisplays20__signal__changed__cbbx(
    GtkComboBox     *       _i_cbbx     ,
    gpointer                _i_data     )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    gint                            i       =   gtk_combo_box_get_active(_i_cbbx);
    //  ............................................................................................
    printf("changed (cbbx)\n");

    THIS->control()->display__chg_signal_motif(vcd->a_szt1, vcd->a_szt2, i);
}
void
View::PgDisplays::GtkEvent__pgdisplays20__signal__value_changed__adj(
    GtkAdjustment   *       _i_adj  ,
    gpointer                _i_data )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    double                          v       =   gtk_adjustment_get_value(_i_adj);
    //  ............................................................................................
    printf("value-changed (%lu)\n", vcd->a_szt3);


    switch ( vcd->a_szt3 )
    {

    case 1:                                                                                         // ix1
    THIS->control()->display__chg_signal_ix1        (vcd->a_szt1, vcd->a_szt2, v);
    break;

    case 2:                                                                                         // ix2
    THIS->control()->display__chg_signal_ix2        (vcd->a_szt1, vcd->a_szt2, v);
    break;

    case 3:                                                                                         // sgn ep
    THIS->control()->display__chg_signal_ep         (vcd->a_szt1, vcd->a_szt2, v);
    break;

    case 4:                                                                                         // sgn join ep
    THIS->control()->display__chg_signal_join_ep    (vcd->a_szt1, vcd->a_szt2, v);
    break;


    }

}

void
View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton_join(
    GtkToggleButton *   _i_togglebutton ,
    gpointer            _i_data         )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    //  ............................................................................................
    //if ( THIS->a_rdbt_selected_display_uid == 0 )
    //    return;

    THIS->control()->display__chg_signal_join(vcd->a_szt1, vcd->a_szt2, gtk_toggle_button_get_active(_i_togglebutton));
}


}
}



