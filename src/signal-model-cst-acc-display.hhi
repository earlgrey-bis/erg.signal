private:

class   sConstAccessorDisplay
{
  private:
                std::vector< Model::sDisplay* >             *   a_v;
    typename    std::vector< Model::sDisplay * >::iterator      a_it;
                Model::sCDisplayAux                             a_cd;
                size_t                                          a_ix;

  public:
    void                            set(std::vector< Model::sDisplay * > * _i_v)
                                        {
                                            a_v = _i_v;
                                        }

    Model::sCDisplay           *    enum_start()
                                        {
                                            a_ix    =   0;

                                            if ( a_v->empty() )
                                                return nullptr;

                                            a_it = a_v->begin();

                                            a_cd.display    =   (*a_it)->display;
                                            a_cd.att        =   (*a_it)->att;
                                            return (Model::sCDisplayAux const *)(&a_cd);
                                        }

    Model::sCDisplay           *    enum_next()
                                        {
                                            a_it++;
                                            a_ix++;

                                            if ( a_it == a_v->end() )
                                                return nullptr;

                                            a_cd.display    =   (*a_it)->display;
                                            a_cd.att        =   (*a_it)->att;
                                            return (Model::sCDisplayAux const *)(&a_cd);
                                        }

    size_t                          enum_idx()
                                        {
                                            return a_ix;
                                        }
};

public:

sCDisplay                       *   display__enum_start()
                                        {
                                            return a_cdisplays.enum_start();
                                        }

sCDisplay                       *   display__enum_next()
                                        {
                                            return a_cdisplays.enum_next();
                                        }

size_t                              display__enum_idx()
                                        {
                                            return a_cdisplays.enum_idx();
                                        }

