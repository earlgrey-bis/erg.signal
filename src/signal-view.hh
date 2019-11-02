#ifndef __ERG_SIGNAL_VIEW_HH__
#define __ERG_SIGNAL_VIEW_HH__

#include    <gtk/gtk.h>

#include    "signal-types.hh"
#include    "signal-vpstorage.hh"

#include    <list>
#include    <vector>

namespace erg
{
namespace signal
{

class   View : public signal::MemberModel, public signal::MemberControl
{
  public:
    class   PgSignals;
    class   PgDisplays;
    class   PgDisplay;
    //  ============================================================================================
    //  inner class "CallbackData"
    //  ============================================================================================
  public:
    class   CallbackData
    {
      public:
        gpointer    a_instance;
        gpointer    a_v1;
        size_t      a_szt1;
        size_t      a_szt2;
        size_t      a_szt3;

      public:
                CallbackData(gpointer _i_instance)
                    :   a_instance(_i_instance) ,
                        a_v1(nullptr)           ,
                        a_szt1(0)
                    {
                    }
                CallbackData(gpointer _i_instance, gpointer _i_v1, size_t _i_szt1)
                    :   a_instance  ( _i_instance   )   ,
                        a_v1        ( _i_v1         )   ,
                        a_szt1      ( _i_szt1       )
                    {
                    }
        virtual ~CallbackData()
                    {
                    }
    };
    //  ============================================================================================
    //  inner class "PgDisplays"
    //  ============================================================================================
  public:
    #include    "signal-view-pgdisplays.hhi"
    //  ============================================================================================
    //  inner class "PgSignals"
    //  ============================================================================================
    #include    "signal-view-pgsignals.hhi"
    //  ============================================================================================
    //  inner class "PgMixer"
    //  ============================================================================================
    #include    "signal-view-pgmixer.hhi"
    /*
  public:
    class   PgDisplay : public signal::GenericView
    {
      private:
        typedef struct _sWidgets01
        {
            GtkWidget                       *       d_labl_index;
            GtkWidget                       *       d_chkb_draw;

            GtkWidget                       *       d_vsep_1;

            GtkWidget                       *       d_labl_nsamples;
            GtkWidget                       *       d_spib_xmin;
            GtkObject                       *         d_adjs_xmin;
            GtkWidget                       *       d_spib_xmax;
            GtkObject                       *         d_adjs_xmax;

            GtkWidget                       *       d_vsep_2;

            GtkWidget                       *       d_cbxt_samples_select;
            GtkWidget                       *       d_spib_sample_ix1;
            GtkObject                       *         d_adjs_sample_ix1;
            GtkWidget                       *       d_spib_sample_ix2;
            GtkObject                       *         d_adjs_sample_ix2;

            GtkWidget                       *       d_vsep_3;

            GtkWidget                       *       d_cbxt_style;
            GtkWidget                       *       d_spib_style_ep;
            GtkWidget                       *       d_colb_style_color;
        } sWidgets01;
        //  ----------------------------------------------------------------------------------------
        //  Model
        //  ----------------------------------------------------------------------------------------
        //  from Main::GenericView
        //  ----------------------------------------------------------------------------------------
        //  View
        //  ----------------------------------------------------------------------------------------
      private:
        GtkWidget                       *   d_vbox_1;

        GtkWidget                       *     d_frame_1;
        GtkWidget                       *       d_table_1;
        std::vector< sWidgets01 >                 a_widgets01;
        GtkObject                       *         d_adjs_style_ep;

        GtkWidget                       *     d_frame_2;

      public:
        GtkWidget   *   wgt()   { return d_vbox_1; }

      private:
        void    gtkreate();
        //  ----------------------------------------------------------------------------------------
        //  Control
        //  ----------------------------------------------------------------------------------------
        //  from Main::GenericView
        //  ----------------------------------------------------------------------------------------
        //  ()~()
        //  ----------------------------------------------------------------------------------------
      public:
        PgDisplay(Main::Model* _i_model, gpointer _i_control);
        //PgDisplay(
        //    std::vector< signal::IData* >       &   _i_datas    ,
        //    std::vector< signal::sDrawStyle* >  &   _i_styles   );
        virtual ~PgDisplay() {}
    };
    */
    //  ============================================================================================
    //  View
    //  ============================================================================================
    //  --------------------------------------------------------------------------------------------
    //  struct
    //  --------------------------------------------------------------------------------------------
    typedef     struct  _sMouseInfo
    {
        GdkPoint    spos;
        gint        dragx;
        gint        dragy;
        GdkPoint    coords;
        bool        b1;
    } sMouseInfo;
    //  --------------------------------------------------------------------------------------------
    //  View
    //  --------------------------------------------------------------------------------------------
  private:
    GtkWidget               *   d_window;
    GtkWidget               *     d_nbok;
    PgDisplays              *       d_nbpg_displays;
    PgSignals               *       d_nbpg_signals;
    //PgMixer                 *       d_nbpg_mixer;
    //  --------------------------------------------------------------------------------------------
    void    gtkreate();
    //  --------------------------------------------------------------------------------------------
  public:
    void    refresh();
    void    refresh_display(
                size_t      _i_display_uid                      ,
                bool        _i_signals              =   true    ,
                bool        _i_axes                 =   true    ,
                bool        _i_rulers               =   true    ,
                bool        _i_selected             =   true    ,
                bool        _i_inlay_mouse_coords   =   true    );
    //  --------------------------------------------------------------------------------------------
    //  --------------------------------------------------------------------------------------------
    //  Control
    //  --------------------------------------------------------------------------------------------
  public:
    //static  void        GtkEvent__pgdisplay__toggled__checkbutton_draw          (GtkToggleButton    *               , gpointer);
    //static  gboolean    GtkEvent__pgdisplays__button_press_event__add_display   (GtkWidget          *, GdkEvent*    , gpointer);
    //static  gboolean    GtkEvent__pgsignals__changed__comboboxtext_display      (GtkComboBox        *               , gpointer);

  public:
    //void        data_add    (signal::IData const * _i_idata, signal::sDrawStyle* _i_style);
    //void        data_add    (signal::IData const * _i_idata, signal::eDrawMotif _i_motif, guint8 _i_r, guint8 _i_g, guint8 _i_b);


    //  --------------------------------------------------------------------------------------------
    //  utils
    //  --------------------------------------------------------------------------------------------
  private:
    //  --------------------------------------------------------------------------------------------
    //  draw
    //  --------------------------------------------------------------------------------------------
  public:
    void    show();

    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
     View(signal::Model* _i_model, signal::Control* _i_control);
    ~View()    {}

    void    construct();

};





}
}










#endif
