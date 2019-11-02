#ifndef __ERG_SIGNAL_DATA_HH__
#define __ERG_SIGNAL_DATA_HH__

#include    "signal-types.hh"

namespace erg
{
namespace signal
{


class   IData
{

  protected:
    static  size_t          s_uid;
            size_t          a_uid;
            char const  *   d_name;

  protected:
    size_t              a_card;

    signal::AreaDbl     a_area;

  public:
            char const *    name()  const   { return d_name;        }
            size_t          uid()   const   { return a_uid;         }
            size_t          crd()   const   { return a_card;        }

            double          xmin()  const   { return a_area.a_xmin; }
            double          xmax()  const   { return a_area.a_xmax; }
            double          ymin()  const   { return a_area.a_ymin; }
            double          ymax()  const   { return a_area.a_ymax; }
            signal::AreaDbl area()  const   { return a_area;        }

    virtual double  x(size_t _i_ix) const                                                   =   0;
    virtual double  y(size_t _i_ix) const                                                   =   0;

  public:
    virtual bool    isDI()          const                                                   =   0;
    virtual bool    isDF()          const                                                   =   0;

  protected:
    virtual ~IData()
                {
                    if ( d_name ) g_free( (gpointer)d_name );
                }

};

class   DataFull : public IData
{
  private:
    double  *   a_x;
    double  *   a_y;

  public:
    int     const       c() {   return a_card;  }
    double  const   *   x() {   return a_x;     }
    double  const   *   y() {   return a_y;     }

    double              x(size_t _i_ix)     { return a_x[_i_ix];    }
    double              y(size_t _i_ix)     { return a_y[_i_ix];    }

  public:
    virtual bool    isDI()          const   { return false; }
    virtual bool    isDF()          const   { return true;  }

  public:
    DataFull()
    {
        d_name  =   nullptr;
        a_card  =   0;
        a_x     =   NULL;
        a_y     =   NULL;
    }
    DataFull(size_t _i_uid, size_t _i_card, double* _i_x, double* _i_y)
    {
        a_card  =   _i_card;
        a_x     =   _i_x;
        a_y     =   _i_y;
    }
    virtual ~DataFull()
    {
    }
};

class   DataInterval : public IData
{
  private:
    double  const   *   a_y;
  public:
    int                 c()     {   return a_card;  }
    double  const   *   y()     {   return a_y;     }

    double              x(size_t _i_ix) const   { return xmin() + ( xmax() - xmin() ) * (double)_i_ix / ( (double)a_card - 1.0f );   }
    double              y(size_t _i_ix) const   { return a_y[_i_ix];    }

  public:
    virtual bool    isDI()              const   { return true;  }
    virtual bool    isDF()              const   { return false; }

  public:
    static  DataInterval    *   New(char const* _i_name, size_t _i_card, double _i_xmin, double _i_xmax, double const * _i_y)
    {
        s_uid++;
        return new DataInterval(s_uid, _i_name, _i_card, _i_xmin, _i_xmax, _i_y);
    }
  private:
    DataInterval()
    {
        d_name          =   nullptr;
        a_card          =   0;
        a_area.a_xmin   =   0.0f;
        a_area.a_xmax   =   0.0f;
        a_area.a_ymin   =   0.0f;
        a_area.a_ymax   =   0.0f;
        a_area.refresh();
        a_y             =   NULL;
    }
    DataInterval(size_t _i_uid, char const* _i_name, size_t _i_card, double _i_xmin, double _i_xmax, double const * _i_y)
    {
        a_uid           =   _i_uid;
        d_name          =   g_strdup(_i_name);
        a_card          =   _i_card;
        a_area.a_xmin   =   _i_xmin;
        a_area.a_xmax   =   _i_xmax;
        a_y             =   _i_y;

        a_area.a_ymin   = DBL_MAX;
        a_area.a_ymax   = DBL_MIN;
        for ( size_t i = 0 ; i != _i_card ; i++ )
        {
            if ( a_y[i] > a_area.a_ymax )   a_area.a_ymax = a_y[i];
            if ( a_y[i] < a_area.a_ymin )   a_area.a_ymin = a_y[i];
        }
        a_area.refresh();

    }
  public:
    virtual ~DataInterval()
    {
    }
};

}
}

#endif
