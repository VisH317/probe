#include "test.hpp"

int main() {
    Test::layerConstructor();
    Test::layerUUID();
    Test::layerDiffUUID();
    Test::diffLayerDiffUUID();
    Test::layerOutOfBoundsTest();

    Test::networkConstructor();
    Test::networkForward();
    Test::networkValid();
    Test::networkDimensionMismatch();
}