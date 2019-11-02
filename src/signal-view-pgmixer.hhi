class   PgMixer : public signal::MemberModel, public signal::MemberControl
{
  private:
    typedef struct _sWidgets01
    {

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
    GtkWidget                       *       d_hbox_1;
    GtkWidget                       *           d_trvw_displays;
    GtkListStore                    *           d_lsto_displays;
    GtkWidget                       *           d_vsep_1;
    GtkWidget                       *           d_scrw_signals;
    GtkWidget                       *           d_vwpr_signals;
    GtkWidget                       *           d_vbox_signals;
    std::list< GtkWidget* >                     a_list_signals;

    GtkWidget                       *     d_frame_2;

  public:
    GtkWidget   *   wgt()   { return d_vbox_1; }

    void            refresh();

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
    PgMixer(signal::Model* _i_model, signal::Control* _i_control);
    virtual ~PgMixer() {}
};
