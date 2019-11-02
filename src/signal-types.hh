#ifndef __ERG_SIGNAL_TYPES_HH__
#define __ERG_SIGNAL_TYPES_HH__

#include    <gtk/gtk.h>

namespace erg
{
namespace signal
{

//  ------------------------------------------------------------------------------------------------
class   IData;
class   Display;

class   Main;
class   Model;
class   View;
class   Control;
//  ------------------------------------------------------------------------------------------------
//  class "MemberModel"
//  ------------------------------------------------------------------------------------------------
class   MemberModel
{
    friend class signal::Main;

  protected:
    signal::Model           *   a_model;

  protected:
    inline  signal::Model   *   model       ()                          { return a_model;       }
    inline  signal::Model   *   mod         ()                          { return a_model;       }

  private:
    inline  void                set_model   (signal::Model* _i_model)   { a_model = _i_model;   }
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
             MemberModel(signal::Model* _i_model)                       { set_model(_i_model);  }
             MemberModel()  {}
    virtual ~MemberModel()  {}
};
//  ------------------------------------------------------------------------------------------------
//  class "MemberView"
//  ------------------------------------------------------------------------------------------------
class   MemberView
{
    friend class signal::Main;

  protected:
    signal::View            *   a_view;

  protected:
    inline  signal::View    *   view        ()                          { return a_view;        }

  private:
    inline  void                set_view    (signal::View* _i_view)     { a_view = _i_view;     }
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
             MemberView(signal::View* _i_view)                          { set_view(_i_view);    }
             MemberView()   {}
    virtual ~MemberView()   {}
};
//  ------------------------------------------------------------------------------------------------
//  class "MemberControl"
//  ------------------------------------------------------------------------------------------------
class   MemberControl
{
    friend class signal::Main;

  protected:
    signal::Control            *   a_control;

  protected:
    inline  signal::Control   *   control   ()                              { return a_control;     }
    inline  signal::Control   *   ctrl      ()                              { return a_control;     }

  private:
    inline  void                set_control (signal::Control* _i_control)   { a_control = _i_control;   }
    //  --------------------------------------------------------------------------------------------
    //  ()~()
    //  --------------------------------------------------------------------------------------------
  public:
             MemberControl(signal::Control* _i_control)                     { set_control(_i_control);  }
             MemberControl()    {}
    virtual ~MemberControl()    {}
};
//  ------------------------------------------------------------------------------------------------
typedef struct  _sColor
{
    unsigned char   r;
    unsigned char   g;
    unsigned char   b;
} sColor;

extern  void    Get_GdkColor(GdkColor& _o_gdkcolor, signal::sColor _i_scolor);

typedef enum    _eDrawMotif
{
    eCross01        =    0  ,
    eCross02        =    1  ,
    eCross03        =    2  ,
    eCross04        =    3  ,
    ePoint          =    4  ,
    eCircle         =    5  ,
    eDisk           =    6
} eDrawMotif;

typedef struct  _sDrawStyle
{
    GdkColor            color;
    eDrawMotif          motif;
    gdouble             motif_ep;
    bool                join;
    gdouble             join_ep;
} sDrawStyle;

extern  signal::sDrawStyle  A_draw_style_default;

extern  GdkColor            A_GdkColor_black;

typedef struct _sSztDomain
{
    size_t      ix1;
    size_t      ix2;
} sSztDomain;

typedef struct _sDblPoint
{
    double  x;
    double  y;
} sDblPoint;

typedef struct _sSztPoint
{
    size_t  x;
    size_t  y;
} sSztPoint;

class   AreaDbl
{
  public:
    double  a_xmin;
    double  a_xmax;
    double  a_ymin;
    double  a_ymax;

    double  a_deltax;
    double  a_deltay;

  public:
    void    refresh()
                {
                    a_deltax = a_xmax - a_xmin;
                    a_deltay = a_ymax - a_ymin;
                }
    void    set(double _i_xmin, double _i_xmax, double _i_ymin, double _i_ymax)
                {
                    a_xmin = _i_xmin; a_xmax = _i_xmax;
                    a_ymin = _i_ymin; a_ymax = _i_ymax;
                    refresh();
                }

  public:
             AreaDbl()  {}
    virtual ~AreaDbl()  {}
};
typedef struct _sDblArea
{
    double  xmin;
    double  xmax;
    double  ymin;
    double  ymax;

    double  deltax;
    double  deltay;
} sDblArea;

typedef     struct _sGintDim
{
    gint    w;
    gint    h;
} sGintDim;

typedef     struct _sGdkRectangle
{
    GdkPoint    pos;
    sGintDim    dim;
} sGdkRectangle;

namespace display
{

typedef     struct _sDraw
{
    bool        signals;
    bool        axes;
    bool        rulers;
    struct
    {
        bool        mouse_coords;
    } inlays;
} sDraw;

/*
typedef     struct _sRedraw
{
    bool        flag;
    bool        signals;
    bool        axes;
    bool        rulers;
    bool        selected;
    struct
    {
        bool        mouse_coords;
    } inlays;
} sRedraw;
*/
class   Redraw
{
    friend class erg::signal::Display;

  private:
    bool        flag;

  public:
    bool        signals;
    bool        axes;
    bool        rulers;
    bool        selected;
    struct
    {
        bool        mouse_coords;
    } inlays;

  private:
    void        set(bool _i_b)
                {
                    flag = signals = axes = rulers = inlays.mouse_coords = _i_b;
                }
    void        set(bool _i_flag, bool _i_b)
                {
                    flag = _i_flag;
                    signals = axes = rulers = inlays.mouse_coords = _i_b;
                }

  public:
    Redraw(bool _i_b = false)
    {
        flag    = true;
        signals = axes = rulers = inlays.mouse_coords =_i_b;
    }
    Redraw(
        bool        _i_signals              ,
        bool        _i_axes                 ,
        bool        _i_rulers               ,
        bool        _i_selected             ,
        bool        _i_inlay_mouse_coords   )
    {
        flag                    =   true;
        signals                 =   _i_signals;
        axes                    =   _i_axes;
        rulers                  =   _i_rulers;
        selected                =   _i_selected;
        inlays.mouse_coords     =   _i_inlay_mouse_coords;
    }


};

}




}
}










#endif
