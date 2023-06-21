#ifndef __TEST__
#define __TEST__

#include "layer.hpp"
#include "network.hpp"

// im gonna organize the test suite better later but for now its just a namespace

namespace Test {
    // layer tests
    int layerConstructor();
    int layerUUID();
    int layerDiffUUID();
    int diffLayerDiffUUID();

    // network
    int networkConstructor();
    int networkForward();
    int networkValid();
}


#endif