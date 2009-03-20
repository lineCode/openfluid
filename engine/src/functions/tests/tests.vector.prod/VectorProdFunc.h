/**
  \file VectorProdFunc.h
  \brief Header of ...
*/

#ifndef __VECTORPRODFUNC_H__
#define __VECTORPRODFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class VectorProdFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    VectorProdFunction();

    /**
      Destructor
    */
    ~VectorProdFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __VECTORPRODFUNC_H__
