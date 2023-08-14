#include "test.hpp"

int main(int argc, char **argv) {
    Test::layerConstructor();
    Test::layerUUID();
    Test::layerDiffUUID();
    Test::diffLayerDiffUUID();
    Test::layerOutOfBoundsTest();

    Test::networkConstructor();
    Test::networkForward();
    Test::networkValid();
    Test::networkDimensionMismatch();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}