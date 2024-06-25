#ifndef kk_h
#define kk_h 1

#include <vector>

class chromosome : public std::vector<bool>
{
public:
    chromosome() : std::vector<bool>(CHROMOSOME_SIZE) {}
};

#endif // kk_h
