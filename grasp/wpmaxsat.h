#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>

class WpMaxSAT : public GRASP {
public:
    WpMaxSAT();
    ~WpMaxSAT();

private:
    std::vector<int> rcl;
};
#endif
