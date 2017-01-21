#include "Requeriments.h"
Requeriments::Requeriments(const Flagset& n, const Flagset& r) :needed(n), rejected(r) {}
Requeriments::Requeriments() : needed(), rejected() {}