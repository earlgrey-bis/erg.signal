#include    <gtk/gtk.h>

#include    <libergc-common.h>
#include    <libergc-macros-check-wrapper.h>

//#define     TKI(FORMAT, ...)    ERGC_TKI(FORMAT, __VA_ARGS__);
#define     TKI(FORMAT, ...)

#include    "signal-data.hh"
#include    "signal-view.hh"
#include    "signal-model.hh"
#include    "signal-display.hh"

namespace erg
{
namespace signal
{

View::View(signal::Model* _i_model, signal::Control* _i_control)
    :   MemberModel     ( _i_model  )   ,
        MemberControl   ( _i_control)
{
}

void
View::construct()
{
    gtkreate();
}

void
View::gtkreate()
{
    d_window        =   gtk_window_new(GTK_WINDOW_TOPLEVEL);

    d_nbok          =   gtk_notebook_new();

    d_nbpg_signals  =   new View::PgSignals     (model(), control());
    d_nbpg_displays =   new View::PgDisplays    (model(), control());
    //d_nbpg_mixer    =   new View::PgMixer       (model(), control());

    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_signals->wgt()    , gtk_label_new("Signals")  );
    gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_displays->wgt()   , gtk_label_new("Displays")  );
    //gtk_notebook_append_page(GTK_NOTEBOOK(d_nbok), d_nbpg_mixer->wgt()      , gtk_label_new("Mixer") );

    gtk_container_add(GTK_CONTAINER(d_window), d_nbok);
}

void
View::show()
{
    gtk_widget_show_all(d_window);
}

void
View::refresh()
{
    d_nbpg_displays ->refresh();
    d_nbpg_signals  ->refresh();
    //d_nbpg_mixer    ->refresh();
}

void
View::refresh_display(
    size_t      _i_display_uid          ,
    bool        _i_signals              ,
    bool        _i_axes                 ,
    bool        _i_rulers               ,
    bool        _i_selected             ,
    bool        _i_inlay_mouse_coords   )
{
    Model::sCDisplay    *   csd     =   nullptr;
    display::Redraw         rdw;
    //  ............................................................................................
    csd =   model()->display__get_from_uid(_i_display_uid);

    if ( ! csd )
        return;

    rdw.signals                 =   _i_signals;
    rdw.axes                    =   _i_axes;
    rdw.rulers                  =   _i_rulers;
    rdw.selected                =   _i_selected;
    rdw.inlays.mouse_coords     =   _i_inlay_mouse_coords;

    csd->display->refresh(rdw);
}


/*
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
*/

}
}



