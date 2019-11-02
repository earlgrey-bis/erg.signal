#ifndef __GTKDA_SETTINGS_HH__
#define __GTKDA_SETTINGS_HH__

#include    <gtk/gtk.h>

#include    "signal-types.hh"
#include    "signal-vpstorage.hh"

#include    <string>
#include    <vector>

namespace erg
{
namespace signal
{

class   Main
{
  private:
    signal::Model   *   d_model;
    signal::View    *   d_view;
    signal::Control *   d_control;

  private:

  public:
    bool    start();
    bool    stop();

  public:
    void    signal_add(
                signal::IData       *   _i_idata                                    ,
                std::string         &   _i_fname                                    );

};

}
}










#endif
