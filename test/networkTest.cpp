#include "test.hpp"

TEST(NetworkConstructor, BasicAssertions) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
}


TEST(NetworkForward, BasicAssertions) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(100);
    torch::Tensor out;
    out = n.forward(input);
}


TEST(NetworkValid, BasicAssertions) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    bool one = n.checkValid();

    std::vector<Layer> v{Layer(100, 50), Layer(51, 1)};
    Network n2(v);
    
    bool two = n2.checkValid();

    EXPECT_EQ(one && !two, true);
}


TEST(NetworkDims, BasicAssertions) {
    std::vector<Layer> v = {Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(110);
    torch::Tensor out;

    try {
        out = n.forward(input);
    } catch() {
        FAIL() << "Network forward failed";
    }
}


