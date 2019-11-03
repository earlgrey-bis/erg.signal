#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKW(FORMAT, ...)    ERGC_TKW(FORMAT, __VA_ARGS__);
#define     TKE(FORMAT, ...)    ERGC_TKE(FORMAT, __VA_ARGS__);
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
    GtkTreeIter             iter;
    //  ............................................................................................
    a_rdbt_selected_display_uid         =   0;
    //  ............................................................................................
    d_vbox                              =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

      d_frame_10                        =   gtk_frame_new("All displays");

        d_vbox_10                       =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
          d_table_10                    =   gtk_table_new(0, 5, FALSE);
          d_pshb_add_display            =   gtk_button_new_with_label("Add display");

      d_frame_20                        =   gtk_frame_new("Others");
        d_vbox_20                       =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
          d_trvw_20                     =   gtk_tree_view_new();
          d_lstr_20                     =   gtk_list_store_new(10,
                G_TYPE_BOOLEAN                                  ,                                   // (draw chkb)
                G_TYPE_STRING   , G_TYPE_STRING , G_TYPE_STRING ,                                   // (idx) (name) (card)
                G_TYPE_STRING   , G_TYPE_STRING                 ,                                   // (ix1) (ix2)
                G_TYPE_STRING   , G_TYPE_STRING                 ,                                   // (motif) (motif ep)
                G_TYPE_BOOLEAN  , G_TYPE_STRING                 );                                  // (draw chkb)

    gtk_tree_view_set_model( GTK_TREE_VIEW(d_trvw_20), GTK_TREE_MODEL(d_lstr_20));
    //  ............................................................................................
    a_celr_20.push_back( gtk_cell_renderer_toggle_new()       );                                    // 00 (draw chkb)
        vcd = new View::CallbackData(this, (gpointer)0x1234, 0xab);
        g_signal_connect( a_celr_20.at( 0), "toggled", G_CALLBACK(GtkEvent__pgdisplays20__signal__toggled__checkbutton), vcd);
    a_celr_20.push_back( gtk_cell_renderer_text_new()         );                                    // 01 (idx)
    a_celr_20.push_back( gtk_cell_renderer_text_new()         );                                    // 02 (name)
    a_celr_20.push_back( gtk_cell_renderer_text_new()         );                                    // 03 (card)
        g_object_set( G_OBJECT(a_celr_20.at( 1)), "editable", FALSE, NULL);
        g_object_set( G_OBJECT(a_celr_20.at( 2)), "editable", FALSE, NULL);
        g_object_set( G_OBJECT(a_celr_20.at( 3)), "editable",  TRUE, NULL);

    a_celr_20.push_back( gtk_cell_renderer_spin_new()         );                                    // 04 (ix1)
    a_celr_20.push_back( gtk_cell_renderer_spin_new()         );                                    // 05 (ix2)
        d_adjs_ix1  =   gtk_adjustment_new(    0.0, 0.0, 1023.0, 1.0, 10.0, 0.0);
        d_adjs_ix2  =   gtk_adjustment_new( 1023.0, 0.0, 1023.0, 1.0, 10.0, 0.0);
        g_object_set( G_OBJECT(a_celr_20.at( 4)), "editable",  TRUE, "adjustment", d_adjs_ix1, "digits", 0, NULL);
        g_object_set( G_OBJECT(a_celr_20.at( 5)), "editable",  TRUE, "adjustment", d_adjs_ix2, "digits", 0, NULL);
        vcd = new View::CallbackData(this, nullptr, 4);
        g_signal_connect( a_celr_20.at( 4), "edited", G_CALLBACK(GtkEvent__pgdisplays20__signal__edited__spin_ix), vcd);
        vcd = new View::CallbackData(this, nullptr, 5);
        g_signal_connect( a_celr_20.at( 5), "edited", G_CALLBACK(GtkEvent__pgdisplays20__signal__edited__spin_ix), vcd);

    a_celr_20.push_back( gtk_cell_renderer_combo_new()        );                                    // 06 (motif)
        d_lstr_motif_20 = gtk_list_store_new(1, G_TYPE_STRING);
        gtk_list_store_append   ( d_lstr_motif_20, &iter);
        gtk_list_store_set      ( d_lstr_motif_20, &iter, 0, "Cross 1", -1);
        gtk_list_store_append   ( d_lstr_motif_20, &iter);
        gtk_list_store_set      ( d_lstr_motif_20, &iter, 0, "Cross 2", -1);
        gtk_list_store_append   ( d_lstr_motif_20, &iter);
        gtk_list_store_set      ( d_lstr_motif_20, &iter, 0, "Cross 3", -1);
        gtk_list_store_append   ( d_lstr_motif_20, &iter);
        gtk_list_store_set      ( d_lstr_motif_20, &iter, 0, "Cross 4", -1);
        g_object_set( G_OBJECT(a_celr_20.at( 6))    ,
            "text-column"   , 0                     ,
            "editable"      , TRUE                  ,
            "has-entry"     , FALSE                 ,
            "model"         , d_lstr_motif_20       ,
            NULL                                    );
    a_celr_20.push_back( gtk_cell_renderer_text_new()         );                                    // 07 (motif ep)
        g_object_set( G_OBJECT(a_celr_20.at( 7)), "editable",  FALSE, NULL);

    a_celr_20.push_back( gtk_cell_renderer_toggle_new()       );                                    // 08 (join)
        vcd = new View::CallbackData(this, (gpointer)0x1234, 0xab);
        g_signal_connect( a_celr_20.at( 8), "toggled", G_CALLBACK(GtkEvent__pgdisplays20__signal__toggled__checkbutton_join), vcd);
    a_celr_20.push_back( gtk_cell_renderer_text_new()         );                                    // 09 (join ep)
        g_object_set( G_OBJECT(a_celr_20.at( 9)), "editable",  FALSE, NULL);

    a_celr_20.push_back( gtk_cell_renderer_toggle_new()       );                                    // 10 (color)

    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("Draw"    , a_celr_20.at( 0), "active",  0, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("UID"     , a_celr_20.at( 1), "text"  ,  1, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("Name"    , a_celr_20.at( 2), "text"  ,  2, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("Card"    , a_celr_20.at( 3), "text"  ,  3, NULL) );

    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("ix1"     , a_celr_20.at( 4), "text"  ,  4, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("ix2"     , a_celr_20.at( 5), "text"  ,  5, NULL) );
        gtk_tree_view_column_set_resizable( a_trvc_20.at( 4), TRUE );
        gtk_tree_view_column_set_resizable( a_trvc_20.at( 5), TRUE );

    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("motif"   , a_celr_20.at( 6), "text"  ,  6, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("thick"   , a_celr_20.at( 7), "text"  ,  7, NULL) );

    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("motif"   , a_celr_20.at( 8), "active",  8, NULL) );
    a_trvc_20.push_back( gtk_tree_view_column_new_with_attributes("thick"   , a_celr_20.at( 9), "text"  ,  9, NULL) );

    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 0));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 1));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 2));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 3));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 4));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 5));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 6));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 7));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 8));
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_trvw_20), a_trvc_20.at( 9));
    //  ............................................................................................
    g_object_set                ( d_table_10, "border-width", 5, NULL);                             // avoid frame bottom too close from last line
    gtk_table_set_row_spacings  ( GTK_TABLE(d_table_10), 5 );
    gtk_table_set_col_spacings  ( GTK_TABLE(d_table_10), 5 );
    //  ............................................................................................
    gtk_box_pack_start  ( GTK_BOX(d_vbox_10)        , d_table_10            , FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_10)        , d_pshb_add_display    , FALSE, FALSE, 0);
    gtk_container_add   ( GTK_CONTAINER(d_frame_10) , d_vbox_10);

    gtk_box_pack_start  ( GTK_BOX(d_vbox_20)        , d_trvw_20             , FALSE, FALSE, 0);
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

    GtkTreeIter                             iter;

    PgDisplays::sModel20                    modl20;

    Model::sCDisplay                    *   sdp     =   nullptr;
    Model::sDisplayedSignal     const   *   sds     =   nullptr;

    Model::sCSignal                     *   ssn     =   nullptr;
    signal::IData               const   *   idata;
    Model::sSignalAtt           const   *   att     =   nullptr;

    char                                    str         [16][32];
    bool                                    b00;
    bool                                    b08;
    View::CallbackData                  *   vcd     =   nullptr;
    //  ............................................................................................
    //  delete old widgets from table
    /*
    children = gtk_container_get_children(GTK_CONTAINER(d_table_20));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );
        child = g_list_next(child);
    }
    a_widgets20.clear();
    */
    gtk_list_store_clear( d_lstr_20 );
    a_modl_20.clear();                                                                              // __ERG_TODO__ delete adjustmemts too
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

                                        sds     =   nullptr;

        signal::sDrawStyle  const   *   style   = nullptr;

        if ( sdp )
        {
            sds     =   model()->display__get_signal( sdp, idata->uid() );
            style   =   &( sds->att.style );
        }

        size_t  i       =   mod()->signal__enum_idx();
        gint    x       =  -1;
        //  ........................................................................................
        //  base widgets only : (draw chkb) (idx) (name) (card)
        if ( ! sds )
        {

        TKI("%s\n", "Refresh 20 - simple");

        sprintf(str[ 1], "%02lu", idata->uid()          );
        sprintf(str[ 2], "%02lu", idata->crd()          );
        sprintf(str[ 3], "%s"   , att->d.sname.c_str()  );

        modl20.a_signal_uid = idata->uid();

        gtk_list_store_append   ( d_lstr_20, &iter);
        gtk_list_store_set      ( d_lstr_20, &iter,
             0, FALSE       ,
             1, str[ 1]     ,
             2, str[ 2]     ,
             3, str[ 3]     ,

             4, "---"       ,
             5, "---"       ,

             6, "---"       ,
             7, "---"       ,

             8, FALSE       ,
             9, "---"       ,

            -1              );

        a_modl_20.push_back(modl20);

        }
        //  All widgets : (draw chkb) (idx) (name) (card) * (ix1) (ix2) * (motif) (motif ep) (join) (join ep) (color)
        else
        {

        TKI("%s\n", "Refresh 20 - full");

        //  ----- (draw chkb) (idx) (name) (card)
        sprintf(str[ 1], "%02lu", idata->uid()          );
        sprintf(str[ 2], "%02lu", idata->crd()          );
        sprintf(str[ 3], "%s"   , att->d.sname.c_str()  );

        modl20.a_signal_uid = idata->uid();
        //  ----- (ix1) (ix2)
        sprintf(str[ 4], "%02lu", sds->att.ix1          );
        sprintf(str[ 5], "%02lu", sds->att.ix2          );

        //  ----- (motif) (motif ep)
        sprintf(str[ 6], "%02lu", sds->att.ix2          );
        switch ( sds->att.style.motif )
        {
            case    signal::eCross01    :   sprintf(str[ 6], "Cross 01");   break;
            case    signal::eCross02    :   sprintf(str[ 6], "Cross 02");   break;
            case    signal::eCross03    :   sprintf(str[ 6], "Cross 03");   break;
            case    signal::eCross04    :   sprintf(str[ 6], "Cross 04");   break;
            default                     :   sprintf(str[ 6], "Cross 02");   break;
        }
        sprintf(str[ 7], "%03.2lf", sds->att.style.motif_ep);

        b08 = sds->att.style.join;

        gtk_list_store_append (d_lstr_20, &iter);

        gtk_list_store_set( d_lstr_20, &iter,
             0,  TRUE       ,
             1, str[ 1]     ,
             2, str[ 2]     ,
             3, str[ 3]     ,

             4, str[ 4]     ,
             5, str[ 5]     ,

             6, str[ 6]     ,
             7, str[ 7]     ,

             8, b08         ,
             9, "---"       ,

            -1              );

        a_modl_20.push_back(modl20);

        }

//  ........................................................................................
//lab_push:
        //a_widgets20.push_back(wgts20);
    }

    //gtk_widget_show_all(d_table_20);
    //*/
    //gtk_tree_view_set_model(GTK_TREE_VIEW(d_trvw_20), GTK_TREE_MODEL(d_lstr_20));

    gtk_widget_queue_draw( this->d_trvw_20 );
}
//  ************************************************************************************************
//  callbacks
//  ************************************************************************************************
bool
View::PgDisplays::callbacks20_get_signal_uid_from_tree_path_string(
    gchar   *   _i_str_path ,
    size_t  *   _o_index    )
{
    GtkTreePath                 *   trp;
    size_t                          idc;
    gint                        *   indices;
    //  ............................................................................................
    *( _o_index )   =   0;

    trp = gtk_tree_path_new_from_string (_i_str_path);
    if ( ! trp )
    {
        TKE("%s\n", "gtk_tree_path_new_from_string() failed");
        return FALSE;
    }

    indices = gtk_tree_path_get_indices(trp);                                                       // This value should not be freed.
    //gtk_tree_path_free(trp);                                                                      // deleting trp here invalidate indices !!!
    if ( ! indices )
    {
        TKE("%s\n", "gtk_tree_path_get_indices() failed");
        gtk_tree_path_free(trp);
        return FALSE;
    }

    idc             = (size_t)( indices[0] );
    *( _o_index )   = a_modl_20.at(idc).a_signal_uid;

    gtk_tree_path_free(trp);

    return TRUE;
}

void
View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton(
    GtkCellRendererToggle       *   _i_cr       ,
               gchar            *   _i_str_path ,
               gpointer             _i_data     )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;

    GtkTreeIter                     iter;
    size_t                          suid;
    gint                            b       =   0;  //  not bool, we need bytes for gtk_tree_model_get() ; stack vars may be ovewritten !
    //  ............................................................................................
    TKI("%s\n", "(cr.toggled:signal draw) toggled");
    //  ............................................................................................
    //  get iter
    if ( ! gtk_tree_model_get_iter_from_string ( GTK_TREE_MODEL(THIS->d_lstr_20)   , &iter, _i_str_path) )
    {
        TKE("%s\n", "20:stoggled:gtk_tree_model_get_iter_from_string() failed");
        return;
    }
    //  ............................................................................................
    if ( ! THIS->callbacks20_get_signal_uid_from_tree_path_string(_i_str_path, &suid) )
    {
        TKE("%s\n", "20:stoggled:Callbacks20_get_signal_uid_from_tree_path_string() failed");
        return;
    }
    //  ............................................................................................
    //  act eventually

    //  no display selected, do nothing
    if ( THIS->a_rdbt_selected_display_uid == 0 )
        return;

    gtk_tree_model_get( GTK_TREE_MODEL(THIS->d_lstr_20)   , &iter, 0, &b, -1);
    gtk_list_store_set( THIS->d_lstr_20                   , &iter, 0, !b, -1);                      // unuseful ? will be refreshed by control

    if ( ! b )
    {
        THIS->control()->display__add_signal(THIS->a_rdbt_selected_display_uid, suid);
    }
    else
    {
        THIS->control()->display__del_signal(THIS->a_rdbt_selected_display_uid, suid);
    }
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
View::PgDisplays::GtkEvent__pgdisplays20__signal__edited__spin_ix(
    GtkCellRendererText     *   _i_cr       ,
    gchar                   *   _i_str_path ,
    gchar                   *   _i_new_text ,
    gpointer                    _i_data     )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;
    size_t                          ixcol   =   vcd->a_szt1;

    Model::sCDisplay            *   sdp     =   nullptr;

    size_t                          suid;
    double                          v       =   0.0;
    //  ............................................................................................
    TKI("%s\n", "(cr.edited:ix1) edited");
    //  ............................................................................................
    if ( ! THIS->callbacks20_get_signal_uid_from_tree_path_string(_i_str_path, &suid) )
    {
        TKE("%s\n", "20:ix1:Callbacks20_get_signal_uid_from_tree_path_string() failed");
        return;
    }
    //  ............................................................................................
    //  act eventually

    //  no display selected, do nothing
    if ( THIS->a_rdbt_selected_display_uid == 0 )
        return;

    //  cant get display !
    sdp = THIS->model()->display__get_from_uid(THIS->a_rdbt_selected_display_uid);
    if ( ! sdp )
        return;

    //  signal not drawed by display, do nothing
    if ( THIS->model()->display__get_signal(sdp, suid) == nullptr )
        return;

    if ( ixcol == 4 )
    {
        v = gtk_adjustment_get_value(THIS->d_adjs_ix1);
        THIS->control()->display__chg_signal_ix1( THIS->a_rdbt_selected_display_uid, suid, static_cast< size_t >( v ) );
    }
    if ( ixcol == 5 )
    {
        v = gtk_adjustment_get_value(THIS->d_adjs_ix2);
        THIS->control()->display__chg_signal_ix2( THIS->a_rdbt_selected_display_uid, suid, static_cast< size_t >( v ) );
    }
}

void
View::PgDisplays::GtkEvent__pgdisplays20__signal__toggled__checkbutton_join(
    GtkCellRendererToggle       *   _i_cr       ,
               gchar            *   _i_str_path ,
               gpointer             _i_data     )
{
    View::CallbackData          *   vcd     =   (View::CallbackData*)_i_data;
    View::PgDisplays            *   THIS    =   (View::PgDisplays*)vcd->a_instance;

    GtkTreeIter                     iter;
    size_t                          suid;
    gint                            b       =   0;  //  not bool, we need bytes for gtk_tree_model_get() ; stack vars may be ovewritten !
    //  ............................................................................................
    TKI("%s\n", "(cr.toggled:signal join) toggled");
    //  ............................................................................................
    //  get iter
    if ( ! gtk_tree_model_get_iter_from_string ( GTK_TREE_MODEL(THIS->d_lstr_20)   , &iter, _i_str_path) )
    {
        TKE("%s\n", "20:jtoggled:gtk_tree_model_get_iter_from_string() failed");
        return;
    }
    //  ............................................................................................
    if ( ! THIS->callbacks20_get_signal_uid_from_tree_path_string(_i_str_path, &suid) )
    {
        TKE("%s\n", "20:jtoggled:Callbacks20_get_signal_uid_from_tree_path_string() failed");
        return;
    }
    //  ............................................................................................
    //  act eventually

    //  no display selected, do nothing
    if ( THIS->a_rdbt_selected_display_uid == 0 )
        return;

    gtk_tree_model_get( GTK_TREE_MODEL(THIS->d_lstr_20)   , &iter, 8, &b, -1);
    //gtk_list_store_set( THIS->d_lstr_20                   , &iter, 0, !b, -1);                      // unuseful ? will be refreshed by control

    THIS->control()->display__chg_signal_join(THIS->a_rdbt_selected_display_uid, suid, !b);
}

}
}



