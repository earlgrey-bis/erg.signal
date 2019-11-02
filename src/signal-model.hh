#ifndef __ERG_SIGNAL_MODEL_HH__
#define __ERG_SIGNAL_MODEL_HH__
                                                                                                    /*
====================================================================================================
Const accessor classes
====================================================================================================

1) Goal
-------
We want the Model class to hold and modify datas, but export these datas as non-modifiable datas.

2) Solutions
------------

2.1) private / public
.....................
Good for scalar types, or little objects. As soon as the datas get numerous, necessity to
- wrap every field in a specific method
 or
- copy big amounts of data on the stack as return values ( big object typically )

2.2) false const
................

pointers can be modified, and after we return a cont object so all is locked
do not typedef the struct as const, else nothing can be done

                                                                                                    */

#include    <string>

#include    "signal-types.hh"
//#include    "signal-data.hh"
#include    "signal-vpstorage.hh"

namespace   erg
{
namespace   signal
{

class   Display;
class   IData;

class Model
{

  public:
    static  size_t  const   sInvalidDisplayId = 999;

    //  ............................................................................................
    //  signals
  public:
    typedef struct _sSignalAttI
    {
    } sSignalAttI;
    typedef struct _sSignalAttV
    {
    } sSignalAttV;
    typedef struct _sSignalAttD
    {
        std::string                 fname;
        std::string                 sname;
    } sSignalAttD;
    typedef struct _sSignalAtt
    {
        sSignalAttI     i;
        sSignalAttV     v;
        sSignalAttD     d;
    } sSignalAtt;

  private:
    typedef struct _sSignal
    {
        signal::IData           *   idata;
        Model::sSignalAtt       *   att;
    } sSignal;
    typedef struct _sCSignalAux
    {
        signal::IData       const   *   idata;
        Model::sSignalAtt   const   *   att;
    } sCSignalAux;

  public:
    typedef sCSignalAux const   sCSignal;

  private:
    #include    "signal-model-cst-acc-signal.hhi"

  private:
    typedef std::vector< Model::sSignal *  >    tVSgn;
            std::vector< Model::sSignal *  >    a_signals;
            Model::sConstAccessorSignal         a_csignals;
    //  ............................................................................................
  public:
    void                    signal__add          (
                                signal::IData       *   _i_idata    ,
                                Model::sSignalAtt   *   _i_att      );
    //  ............................................................................................
  private:
    sSignal             *   z_signal__get_from_uid(size_t _i_signal_uid);
  public:
    sCSignal    const   *   signal__get_from_uid(size_t _i_signal_uid);
    //  ............................................................................................
    //  displays
  public:
    typedef struct _sDisplayedSignalAtt
    {
        signal::sDrawStyle  style;
        size_t              ix1;
        size_t              ix2;
    } sDisplayedSignalAtt;
    typedef struct _sDisplayedSignal
    {
        size_t              uid;
        sDisplayedSignalAtt att;
    } sDisplayedSignal;
    typedef struct _sDisplayAttI
    {
        signal::AreaDbl     area;
        signal::sSztPoint   rulers;
        signal::sDblPoint   axis;
    } sDisplayAttI;
    typedef struct _sDisplayAttV
    {
        signal::sColor      bgcolor;
        display::sDraw      draw;
    } sDisplayAttV;
    typedef struct _sDisplayAttD
    {
        std::string     name;
    } sDisplayAttD;
    typedef struct _sDisplayAtt
    {
        sDisplayAttI    i;
        sDisplayAttV    v;
        sDisplayAttD    d;
        std::vector< Model::sDisplayedSignal >  signals;
    } sDisplayAtt;

  private:
    typedef struct _sDisplay
    {
        signal::Display                 *   display;
        Model::sDisplayAtt              *   att;
    } sDisplay;
    typedef struct _sCDisplayAux
    {
        signal::Display                 *   display;
        Model::sDisplayAtt      const   *   att;
    } sCDisplayAux;

  public:
    typedef sCDisplayAux const  sCDisplay;

  private:
    #include    "signal-model-cst-acc-display.hhi"

  private:
    typedef std::vector     < Model::sDisplay * >                   tVDsp;
            std::vector     < Model::sDisplay * >                   a_displays;
            Model::sConstAccessorDisplay                            a_cdisplays;
    //  ............................................................................................
  public:
    void                        display__add(
                                    signal::Display     *   _i_display          ,
                                    Model::sDisplayAtt  *   _i_allocated_att    );
    //  ............................................................................................
  private:
    sDisplayedSignal        *   z_display__get_signal   (sDisplay   *   _i_sdp ,    size_t _i_signal_uid);
  public:
    sDisplayedSignal const  *   display__get_signal     (sCDisplay  *   _i_sdp ,    size_t _i_signal_uid);
    //  ............................................................................................
  private:
    sDisplay                *   z_display__get_from_uid (size_t  _i_display_uid);
  public:
    sCDisplay               *   display__get_from_uid   (size_t _i_display_uid);
    //  ............................................................................................
  public:
    bool                        display__add_signal         (size_t _i_display_uid, size_t _i_signal_uid);
    bool                        display__del_signal         (size_t _i_display_uid, size_t _i_signal_uid);
    bool                        display__get_signal_att     (size_t _i_display_uid, size_t _i_signal_uid, Model::sDisplayedSignalAtt* _o_att);
    bool                        display__set_signal_att     (size_t _i_display_uid, size_t _i_signal_uid, Model::sDisplayedSignalAtt* _i_att);
    //  ............................................................................................
  public:
    void                        display__set_area_from_display  (size_t _i_display_uid, sDisplayAtt const * _i_att, double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax);
    //void                        display__set_area               (size_t _i_display_uid, double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax);
    bool                        display__get_att_I          (size_t _i_display_uid, Model::sDisplayAttI* _o_att);
    bool                        display__get_att_V          (size_t _i_display_uid, Model::sDisplayAttV* _o_att);
    bool                        display__get_att_D          (size_t _i_display_uid, Model::sDisplayAttD* _o_att);
    bool                        display__set_att_I          (size_t _i_display_uid, Model::sDisplayAttI* _i_att);
    bool                        display__set_att_V          (size_t _i_display_uid, Model::sDisplayAttV* _i_att);
    bool                        display__set_att_D          (size_t _i_display_uid, Model::sDisplayAttD* _i_att);
    //  ............................................................................................
  public:
    Model();
};

}
}

#endif
