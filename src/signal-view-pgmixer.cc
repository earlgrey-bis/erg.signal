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


View::PgMixer::PgMixer(
    signal::Model   *   _i_model    ,
    signal::Control *   _i_control  )
    :   MemberModel     ( _i_model  )   ,
        MemberControl   ( _i_control)
{
    gtkreate();
    refresh();
}

void
View::PgMixer::gtkreate()
{
    //View::CallbackData  *   vcd     =   nullptr;
    //  ............................................................................................
    d_vbox_1                            =   gtk_vbox_new(FALSE, 2);

      d_frame_1                         =   gtk_frame_new("Mixer");

        d_hbox_1                        =   gtk_hbox_new(FALSE, 2);
          d_lsto_displays               =   gtk_list_store_new(2, G_TYPE_UINT, G_TYPE_STRING);

          d_trvw_displays               =   gtk_tree_view_new();

          d_vsep_1                      =   gtk_vseparator_new();

          d_vwpr_signals                =   gtk_viewport_new(NULL, NULL);
          d_scrw_signals                =   gtk_scrolled_window_new(NULL, NULL);
          d_vbox_signals                =   gtk_vbox_new(FALSE, 2);

      d_frame_2                         =   gtk_frame_new("Others");
    //  ............................................................................................
    g_object_set                ( d_hbox_1, "border-width", 5, NULL);                               // avoid frame bottom too close from last line

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(d_trvw_displays),
                                               -1,
                                               "UID",
                                               gtk_cell_renderer_text_new(),
                                               "text", 0,
                                               NULL);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(d_trvw_displays),
                                               -1,
                                               "Name",
                                               gtk_cell_renderer_text_new(),
                                               "text", 1,
                                               NULL);
    gtk_tree_view_set_model(GTK_TREE_VIEW(d_trvw_displays), GTK_TREE_MODEL(d_lsto_displays));
    //  ............................................................................................
    gtk_container_add   ( GTK_CONTAINER(d_vwpr_signals), d_vbox_signals);
    gtk_container_add   ( GTK_CONTAINER(d_scrw_signals), d_vwpr_signals);


    gtk_box_pack_start  ( GTK_BOX(d_hbox_1)         , d_trvw_displays   , FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_hbox_1)         , d_vsep_1          ,  TRUE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_hbox_1)         , d_scrw_signals    ,  TRUE,  TRUE, 0);
    gtk_container_add   ( GTK_CONTAINER(d_frame_1)  , d_hbox_1);

    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_1         , FALSE, FALSE, 0);
    gtk_box_pack_start  ( GTK_BOX(d_vbox_1)         , d_frame_2         ,  TRUE,  TRUE, 0);
    //  ............................................................................................
    //vcd = new View::CallbackData(a_control);
    //g_signal_connect(
    //    d_pshb_add_display                                                                  ,
    //    "button-press-event"                                                                ,
    //    G_CALLBACK(signal::Control::GtkEvent__pgdisplays__button_press_event__add_display)  ,
    //    (gpointer)vcd                                                                       );
}

void
View::PgMixer::refresh()
{
    GList                           *   children;
    GList                           *   child;
    //sWidgets01              wgts01;
    //signal::IData       *   idata;
    //signal::sDrawStyle  *   style;
    char                    s1  [128];
    //  ............................................................................................
    //  delete old widgets from table
    gtk_list_store_clear(d_lsto_displays);
    //  ............................................................................................
    children = gtk_container_get_children(GTK_CONTAINER(d_vbox_signals));
    child = g_list_first(children) ; while ( child)
    {
        // gtk_widget_destroy will remove child from container, and invalidate all connected signals
        gtk_widget_destroy( (GtkWidget*)child->data );

        child = g_list_next(child);
    }
    a_list_signals.clear();
    //  ............................................................................................
    //  create new widgets
    {

    Model::sCDisplay    *   sdp =   nullptr;
    signal::Display     *   dpl =   nullptr;
    Model::sDisplayAtt  *   att =   nullptr;
    GtkTreeIter             itr;
    for ( sdp = mod()->display__enum_start() ; sdp != nullptr ; sdp = mod()->display__enum_next() )
    {
        dpl                             =   sdp->display;
        att                             =   sdp->att;

        gtk_list_store_append   (d_lsto_displays, &itr);
        gtk_list_store_set      (d_lsto_displays, &itr  ,
                                    0,  dpl->uid()      ,
                                    1,  att->d.name     ,   -1 );
    }

    }
    //  ............................................................................................
    {

    Model::sCSignal     *   ssg     =   nullptr;
    Model::sSignalAtt   *   att     =   nullptr;
    signal::IData const *   idata   =   nullptr;
    GtkWidget           *   chkb    =   nullptr;

    for ( ssg = mod()->signal__enum_start() ; ssg != nullptr ; ssg = mod()->signal__enum_next() )
    {
        idata   =   ssg->idata;
        att     =   ssg->att;

        sprintf(s1, "%02lu : %s", idata->uid(), att->d.sname.c_str());
        chkb = gtk_check_button_new_with_label(s1);

        gtk_box_pack_start( GTK_BOX(d_vbox_signals), chkb, FALSE, FALSE, 0);

    }

    }
    //  ............................................................................................
    //for ( size_t i = 0 ; i != _i_datas.size() ; i++ )
    //{
    //    wgts01                          =   a_widgets01.at(i);
    //    Main::ViewCallbackData  *   vcd =   new Main::ViewCallbackData(a_control, i);
    //    g_signal_connect( wgts01.d_chkb_draw, "toggled", G_CALLBACK(GtkEvent_toggled__checkbutton_draw), (gpointer)vcd );
    //}

    gtk_widget_show_all(d_vbox_1);


}



}
}



