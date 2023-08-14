#include "test.hpp"

TEST(Network, Constructor) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
}


TEST(Network, Forward) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(100);
    torch::Tensor out;
    out = n.forward(input);
}


TEST(Network, CheckValid) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    bool one = n.checkValid();

    std::vector<Layer> v2 = {Layer(100, 50), Layer(51, 1)};
    Network n2(v2);
    
    bool two = n2.checkValid();

    EXPECT_EQ(one && !two, true);
}


TEST(Network, DimensionAssertion) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(110);
    torch::Tensor out;

    try {
        out = n.forward(input);
    } catch(...) {
        FAIL() << "Network forward failed";
    }
}


