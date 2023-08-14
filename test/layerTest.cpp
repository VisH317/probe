#include "test.hpp"

TEST(Layer, Constructor) {
    Layer l(100, 50);
    Layer l2(50, 1);
}


TEST(Layer, UUID) {
    Layer l(100, 50);
    std::tuple<std::string, torch::Tensor, torch::Tensor> info = l.getNeuron(0);
    EXPECT_EQ(std::get<1>(info).squeeze().size(0)==100 && std::get<2>(info).squeeze().size(0)==1, true);
}


TEST(Layer, DiffUUID) {
    Layer l(100, 50);
    auto info = l.getNeuron(0);
    auto info2 = l.getNeuron(1);
    EXPECT_EQ(std::strcmp(std::get<0>(info).c_str(), std::get<0>(info2).c_str())==0, false);
}


TEST(Layer, DiffLayerUUID) {
    Layer l(100, 50);
    Layer l2(50, 5);
    auto info = l.getNeuron(0);
    auto info2 = l2.getNeuron(0);
    EXPECT_EQ(std::strcmp(std::get<0>(info).c_str(), std::get<0>(info2).c_str())==0, false);
}


TEST(Layer, OutOfRangeCheck) {
    Layer l(100, 50);
    try {
        auto info = l.getNeuron(52);
    } catch (std::out_of_range const & err) {
        EXPECT_EQ(err.what(), "The neuron you selected to probe was out of range of the layer's neurons");
    } catch(...) {
        FAIL() << "Expected std::out_of_range";
    };  
}