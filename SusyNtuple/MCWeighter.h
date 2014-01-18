#ifndef SusyNtuple_MCWeighter_h
#define SusyNtuple_MCWeighter_h

/*

    MCWeighter - a class to handle the normalization of Monte Carlo

    This is work in progress. The class should eventually handle the retrieval
    and application of the cross section and luminosity normalization

*/

class MCWeighter
{

  public:

    enum WeightSys
    {
      Sys_NOM = 0,
      Sys_XSEC_UP,
      Sys_XSEC_DN,
      Sys_N
    };

};


#endif
