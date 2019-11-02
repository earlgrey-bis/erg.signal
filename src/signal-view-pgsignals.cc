#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-data.hh"
#include    "signal-view.hh"
#include    "signal-control.hh"
#include    "signal-display.hh"

namespace erg
{
namespace signal
{

View::PgSignals::PgSignals(
    signal::Model   *   _i_model    ,
    signal::Control *   _i_control  )
    :   MemberModel     ( _i_model  )   ,
        MemberControl   ( _i_control)
{
    gtkreate();
    refresh();
}

void
View::PgSignals::gtkreate()
{
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

void
View::PgSignals::refresh()
{
    GList                       *   children;
    GList                       *   child;

    Model::sCSignal             *   ssn     =   nullptr;
    signal::IData       const   *   idata   =   nullptr;
    Model::sSignalAtt   const   *   att     =   nullptr;

    PgSignals::sWidgets01           wgts01;
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
    for ( ssn = mod()->signal__enum_start() ; ssn != nullptr ; ssn = mod()->signal__enum_next() )
    {
        idata   =   ssn->idata;
        att     =   ssn->att;

        sprintf(s1, "%02lu", idata->uid());
        wgts01.d_labl_data_index        =   gtk_label_new(s1);
        wgts01.d_labl_data_fname        =   gtk_label_new(att->d.fname.c_str());
        wgts01.d_entr_data_sname        =   gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(wgts01.d_entr_data_sname), att->d.sname.c_str());

        sprintf(s1, "%02lu", idata->crd());
        wgts01.d_labl_data_samples_card =   gtk_label_new(s1);
        wgts01.d_labl_data_type_card    =   gtk_label_new( idata->isDI() ? "Interval" : "Full" );

        //wgts01.d_cbxt_display           =   gtk_combo_box_text_new();

        //gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts01.d_cbxt_display), "none");

        //VPSTORAGE_ENUM_START( &mod()->a_displays, dpr )
        //{
        //    sprintf(s1, "%02lu - %s", 1 + mod()->a_displays.enum_idx(), "noname");
        //    gtk_combo_box_text_append_text  ( GTK_COMBO_BOX_TEXT(wgts01.d_cbxt_display), s1);
        //    VPSTORAGE_ENUM_NEXT( &mod()->a_displays, dpr );
        //}

        //if ( idv->display_id != Model::sInvalidDisplayId )
        //{
        //    gtk_combo_box_set_active    ( GTK_COMBO_BOX(wgts01.d_cbxt_display), idv->display_id);
        //}
        //else
        //{
        //    gtk_combo_box_set_active    ( GTK_COMBO_BOX(wgts01.d_cbxt_display), 0);
        //}

        //  ........................................................................................
        //  connect signals of new widgets
        //Main::ViewCallbackData  *   vcd =   new Main::ViewCallbackData(a_control, (gpointer)idv, 0);
        //g_signal_connect( wgts01.d_cbxt_display, "changed", G_CALLBACK(Main::GtkEvent__pgsignals__changed__comboboxtext_display), (gpointer)vcd );


        a_widgets01.push_back(wgts01);
    }
    //  ............................................................................................
    //  pack widgets
    for ( size_t i = 0 ; i != a_widgets01.size() ; i++ )
    {
        gint    x       =  -1;
                wgts01  = a_widgets01.at(i);

        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_index        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_fname        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_entr_data_sname        , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);
        x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_labl_data_samples_card , (guint)x, (guint)(x+1), i, i + 1, GTK_SHRINK   , GTK_SHRINK    , 0, 0);

        //x+=1 ; gtk_table_attach( GTK_TABLE(d_table_1), wgts01.d_cbxt_display           , (guint)x, (guint)(x+1), i, i + 1, GTK_FILL     , GTK_SHRINK    , 0, 0);
    }

    gtk_widget_show_all(d_table_1);
}


}
}



