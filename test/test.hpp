#ifndef __TEST__
#define __TEST__

#include "layer.hpp"
#include "network.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

// im gonna organize the test suite better later but for now its just a namespace

namespace Test {
    // layer tests
    void layerConstructor();
    void layerUUID();
    void layerDiffUUID();
    void diffLayerDiffUUID();
    void layerOutOfBoundsTest();

    // network
    void networkConstructor();
    void networkForward();
    void networkValid();
    void networkDimensionMismatch();
}


#endif