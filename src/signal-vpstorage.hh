#ifndef __ERG_SIGNAL_VPSTORAGE_HH__
#define __ERG_SIGNAL_VPSTORAGE_HH__

#include    <stddef.h>

#include    <vector>

namespace erg
{
namespace signal
{

template < typename T , typename U > class ConstVP
{
  private:
                std::vector< T * >              *   a_v;
    typename    std::vector< T * >::iterator        a_it;
                size_t                              a_ix;

  public:
    void            set(std::vector< T * > * _i_v)
                        {
                            a_v = _i_v;
                        }

    U   const   *   enum_start()
                        {
                            a_ix    =   0;

                            if ( a_v->empty() )
                                return nullptr;

                            a_it = a_v->begin();
                            return (U const *)(*a_it);
                        }
    U   const   *   enum_next()
                        {
                            a_it++;
                            a_ix++;

                            if ( a_it == a_v->end() )
                                return nullptr;

                            return (U const *)(*a_it);
                        }
    size_t          enum_idx()
                        {
                            return a_ix;
                        }
};

}
}

#endif
