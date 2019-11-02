  public:
    class   PgSignals : public Main::GenericView
    {
      //    ----------------------------------------------------------------------------------------
      //    Model
      //    ----------------------------------------------------------------------------------------
      //    from Main::GenericView
      //    ----------------------------------------------------------------------------------------
      //    View
      //    ----------------------------------------------------------------------------------------
      private:
        typedef struct _sWidgets01
        {
            GtkWidget                       *       d_labl_data_index;
            GtkWidget                       *       d_labl_data_name;
            GtkWidget                       *       d_labl_data_samples_card;
            GtkWidget                       *       d_labl_data_type_card;

            GtkWidget                       *       d_cbxt_display;
        } sWidgets01;

      private:
        GtkWidget                       *   d_vbox_1;

        GtkWidget                       *     d_frame_1;
        GtkWidget                       *       d_table_1;
        std::vector< sWidgets01 >                 a_widgets01;

        GtkWidget                       *     d_frame_2;

      public:
        GtkWidget   *   wgt()   { return d_vbox_1; }

        void            refresh();

      private:
        void    gtkreate();
      //    ----------------------------------------------------------------------------------------
      //    Control
      //    ----------------------------------------------------------------------------------------
      //    from Main::GenericView
      //    ----------------------------------------------------------------------------------------
      //    ()~()
      //    ----------------------------------------------------------------------------------------
      public:
                PgSignals(Main::Model* _i_model, gpointer _i_control);
        virtual ~PgSignals() {}

    };
