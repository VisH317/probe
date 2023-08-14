#ifndef __TEST__
#define __TEST__

#include <torch-probe/layer.hpp>
#include <torch-probe/network.hpp>
#include <gtest/gtest.h>
#include <stdexcept>
#include "layerTest.cpp"
#include "networkTest.cpp"

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