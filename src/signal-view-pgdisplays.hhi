                                                                                                    /*
id name area_def area_cur rulers axis color
                                                                                                    */
class   PgDisplays : public signal::MemberModel, public signal::MemberControl
{
    //  --------------------------------------------------------------------------------------------
    //  Model
    //  --------------------------------------------------------------------------------------------
    //  from Main::GenericView
    //  --------------------------------------------------------------------------------------------
    //  View
    //  --------------------------------------------------------------------------------------------
  private:
    typedef struct _sWidgets10
    {
        size_t                                  a_display_uid;

        GtkWidget                       *       d_rdbt_display_uid;
        GtkWidget                       *       d_labl_display_name;

        GtkWidget                       *       d_vsep_1;

        GtkWidget                       *       d_spib_xmin_def;
        GtkObject                       *       d_adjs_xmin_def;
        GtkWidget                       *       d_spib_xmax_def;
        GtkObject                       *       d_adjs_xmax_def;
        GtkWidget                       *       d_spib_ymin_def;
        GtkObject                       *       d_adjs_ymin_def;
        GtkWidget                       *       d_spib_ymax_def;
        GtkObject                       *       d_adjs_ymax_def;

        GtkWidget                       *       d_pshb_area_cur_to_def;
        GtkWidget                       *       d_pshb_area_def_to_cur;

        GtkWidget                       *       d_labl_area_current;

        GtkWidget                       *       d_vsep_2;

        GtkWidget                       *       d_spib_rulers_x;
        GtkObject                       *       d_adjs_rulers_x;
        GtkWidget                       *       d_spib_rulers_y;
        GtkObject                       *       d_adjs_rulers_y;

        GtkWidget                       *       d_spib_axis_x;
        GtkObject                       *       d_adjs_axis_x;
        GtkWidget                       *       d_spib_axis_y;
        GtkObject                       *       d_adjs_axis_y;

        GtkWidget                       *       d_colb_bgcolor;

    } sWidgets10;

    typedef struct _sWidgets20
    {
        size_t                                  a_data_uid;
        GtkWidget                       *       d_chkb_data_draw;
        GtkWidget                       *       d_labl_data_index;
        GtkWidget                       *       d_labl_data_sname;
        GtkWidget                       *       d_labl_data_samples_card;

        GtkWidget                       *       d_vsep_1;

        GtkWidget                       *       d_spib_signal_ix1;
        GtkObject                       *       d_adjs_signal_ix1;
        GtkWidget                       *       d_spib_signal_ix2;
        GtkObject                       *       d_adjs_signal_ix2;

        GtkWidget                       *       d_cbbx_signal_motif;
        GtkWidget                       *       d_spib_signal_ep;
        GtkObject                       *       d_adjs_signal_ep;
        GtkWidget                       *       d_chkb_signal_join;
        GtkWidget                       *       d_spib_signal_join_ep;
        GtkObject                       *       d_adjs_signal_join_ep;
        GtkWidget                       *       d_colb_signal;

        GtkWidget                       *       d_labl_data_area;
    } sWidgets20;

    typedef struct _sListStore20
    {
        size_t                      a_signal_uid;

    } sListStore20;

  private:
    GtkWidget                       *   d_vbox;

    GtkWidget                       *     d_frame_10;
    GtkWidget                       *       d_vbox_10;
    GtkWidget                       *         d_table_10;
    std::vector< sWidgets10 >                 a_widgets10;
    GtkWidget                       *         d_pshb_add_display;
    size_t                              a_rdbt_selected_display_uid;

    GtkWidget                       *     d_frame_20;
    GtkWidget                       *       d_vbox_20;

    //GtkWidget                       *         d_table_20;
    //std::vector< sWidgets20 >                 a_widgets20;

    GtkWidget                       *         d_trvw_20;
    std::vector<GtkCellRenderer*>             a_celr_20;
    std::vector<GtkTreeViewColumn*>           a_trvc_20;
    GtkListStore                    *         d_lstr_20;
    GtkListStore                    *         d_lstr_motif_20;
    std::vector<sListStore20>                 a_vlst_20;

  public:
    GtkWidget   *   wgt()   { return d_vbox; }

    void            refresh();
    GtkWidget   *   rf1_create_rdbt(
                        size_t  _i_enum_idx     ,
                        size_t  _i_display_uid  );
    void            refresh_frame1();
    void            refresh_frame2();

    sWidgets10  *   widgets10__get_from_display_uid   (size_t _i_display_uid);
    //sWidgets20  *   widgets20__get_from_signal_uid    (size_t _i_signal_uid);

  private:
    void    gtkreate();
    //  --------------------------------------------------------------------------------------------
    //  Control
    //  --------------------------------------------------------------------------------------------
    //  from Main::GenericView
  private:
    static  void        GtkEvent__pgdisplays10__display__toggled__checkbutton       (GtkToggleButton    *   , gpointer);

    static  void        GtkEvent__pgdisplays20__signal__toggled__checkbutton        (GtkCellRendererToggle  *   _i_cr   , gchar *   _i_path , gpointer);
    static  void        GtkEvent__pgdisplays20__signal__changed__cbbx               (GtkComboBox            *                               , gpointer);
    static  void        GtkEvent__pgdisplays20__signal__value_changed__adj          (GtkAdjustment          *                               , gpointer);
    static  void        GtkEvent__pgdisplays20__signal__color_set__colorbutton      (GtkColorButton         *                               , gpointer);
    static  void        GtkEvent__pgdisplays20__signal__toggled__checkbutton_join   (GtkToggleButton        *                               , gpointer);
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
             PgDisplays(signal::Model* _i_model, signal::Control* _i_control);
    virtual ~PgDisplays() {}

};
