#ifndef __ERG_SIGNAL_CONTROL_HH__
#define __ERG_SIGNAL_CONTROL_HH__

#include    <gtk/gtk.h>

#include    "signal-types.hh"
#include    "signal-vpstorage.hh"

#include    "signal-model.hh"

#include    <vector>

namespace erg
{
namespace signal
{

class   IData;
class   Display;
class   Model;

class   Control : public signal::MemberModel, public signal::MemberView
{
    //  --------------------------------------------------------------------------------------------
    //  Control
    //  --------------------------------------------------------------------------------------------
  public:
    static  gboolean    GtkEvent__pgdisplays__button_press_event__add_display   (GtkWidget          *, GdkEvent*    , gpointer);

    //static  void        GtkEvent__pgdisplay__toggled__checkbutton_draw          (GtkToggleButton    *               , gpointer);

    static  gboolean    GtkEvent__pgsignals__changed__comboboxtext_display      (GtkComboBox        *               , gpointer);

    /*
  public:
    void        data_add    (signal::IData const * _i_idata, signal::sDrawStyle* _i_style);
    void        data_add    (signal::IData const * _i_idata, signal::eDrawMotif _i_motif, guint8 _i_r, guint8 _i_g, guint8 _i_b);
    //  --------------------------------------------------------------------------------------------
    //  utils
    //  --------------------------------------------------------------------------------------------
  private:
    */
    //  --------------------------------------------------------------------------------------------
    //  extern calls
    //  --------------------------------------------------------------------------------------------
    void    signal__add(
                signal::IData       *   _i_idata    ,
                Model::sSignalAtt   *   _i_att  );
    void    signal__add(
                signal::IData       *   _i_idata    ,
                std::string         &   _i_fname    );

    void    display__add(
                Model::sDisplayAtt  *   _i_att  );
    void    display__add(
                std::string         &   _i_name     ,
                signal::AreaDbl     *   _i_area     ,
                signal::sSztPoint   *   _i_rulers   ,
                signal::sDblPoint   *   _i_axis     ,
                signal::sColor      *   _i_bgcolor  ,
                display::sDraw      *   _i_what     );


    void    display__add_signal         (size_t _i_display_uid, size_t _i_signal_uid);
    void    display__del_signal         (size_t _i_display_uid, size_t _i_signal_uid);
    void    display__chg_area           (size_t _i_display_uid, double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax);
    void    display__chg_signal_motif   (size_t _i_display_uid, size_t _i_signal_uid, gint _i_ix);
    void    display__chg_signal_ix1     (size_t _i_display_uid, size_t _i_signal_uid, gdouble _i_ix1);
    void    display__chg_signal_ix2     (size_t _i_display_uid, size_t _i_signal_uid, gdouble _i_ix2);
    void    display__chg_signal_ep      (size_t _i_display_uid, size_t _i_signal_uid, gdouble _i_ep);
    void    display__chg_signal_join    (size_t _i_display_uid, size_t _i_signal_uid, bool _i_join);
    void    display__chg_signal_join_ep (size_t _i_display_uid, size_t _i_signal_uid, gdouble _i_ep);
    void    display__chg_signal_color   (size_t _i_display_uid, size_t _i_signal_uid, GdkColor* _i_color);
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
             Control()      {}
    virtual ~Control()      {}


};





}
}










#endif
