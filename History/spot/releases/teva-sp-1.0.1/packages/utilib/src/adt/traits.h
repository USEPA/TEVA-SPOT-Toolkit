//
// Defines a generic macro for setting traits
//

#ifndef coliny_trait_h
#define coliny_trait_h

#define SetTrait(nspace,trait,type,val)\
namespace nspace { \
template <> \
struct trait < type > { \
public: \
static const bool value = val; \
}; }

#endif
