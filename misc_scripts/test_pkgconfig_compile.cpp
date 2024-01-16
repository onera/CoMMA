#include <iostream>

#include "CoMMA/CoMMA.h"
#include "CoMMA/Util.h"

int main()
{
  const comma::CoMMAIndexT idx = 42;
  const comma::CoMMAIntT itg = 42;
  const comma::CoMMAWeightT wei = 42.42;
  std::cout <<
    "Test CoMMA index  = " << idx << std::endl <<
    "Test CoMMA int    = " << itg << std::endl <<
    "Test CoMMA weight = " << wei << std::endl;
  return 0;
}
