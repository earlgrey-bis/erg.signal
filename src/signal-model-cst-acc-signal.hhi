private:


class   sConstAccessorSignal
{
  private:
                std::vector< Model::sSignal * >             *   a_v;
    typename    std::vector< Model::sSignal * >::iterator       a_it;
                Model::_sCSignalAux                             a_cs;
                size_t                                          a_ix;

  public:
    void                            set(std::vector< Model::sSignal * > * _i_v)
                                        {
                                            a_v = _i_v;
                                        }

    Model::sCSignal             *   enum_start()
                                        {
                                            a_ix    =   0;

                                            if ( a_v->empty() )
                                                return nullptr;

                                            a_it = a_v->begin();

                                            a_cs.idata  =   (*a_it)->idata;
                                            a_cs.att    =   (*a_it)->att;
                                            return (Model::_sCSignalAux const *)(&a_cs);
                                        }

    Model::sCSignal             *   enum_next()
                                        {
                                            a_it++;
                                            a_ix++;

                                            if ( a_it == a_v->end() )
                                                return nullptr;

                                            a_cs.idata  =   (*a_it)->idata;
                                            a_cs.att    =   (*a_it)->att;
                                            return (Model::_sCSignalAux const *)(&a_cs);
                                        }

    size_t                          enum_idx()
                                        {
                                            return a_ix;
                                        }
};

public:

sCSignal                        *   signal__enum_start()
                                        {
                                            return a_csignals.enum_start();
                                        }

sCSignal                        *   signal__enum_next()
                                        {
                                            return a_csignals.enum_next();
                                        }

size_t                              signal__enum_idx()
                                        {
                                            return a_csignals.enum_idx();
                                        }

