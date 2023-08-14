#include <torch-probe/layer.hpp>
#include <torch-probe/network.hpp>
#include <gtest/gtest.h>
#include <stdexcept>
#include "layerTest.cpp"
#include "networkTest.cpp

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}