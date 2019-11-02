                                                                                                    /*
id name card data_type display
                                                                                                    */

  public:
    class   PgSignals : public signal::MemberModel, public signal::MemberControl
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
            gpointer                                a_uid;

            GtkWidget                       *       d_labl_data_index;
            GtkWidget                       *       d_labl_data_fname;
            GtkWidget                       *       d_entr_data_sname;
            GtkWidget                       *       d_labl_data_samples_card;
            GtkWidget                       *       d_labl_data_type_card;

            GtkWidget                       *       d_labl_data_area;
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
                PgSignals(signal::Model* _i_model, signal::Control* _i_control);
        virtual ~PgSignals() {}

    };
