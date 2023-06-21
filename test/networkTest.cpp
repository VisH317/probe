#include "test.hpp"

int Test::networkConstructor() {
    std::vector<Layer> v{Layer(100, 50), Layer(50, 1)};
    Network n(v);
    return 0;
}

int Test::networkForward() {
    std::vector<Layer> v{Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(100);
    torch::Tensor out;
    out = n.forward(input);
    return 0;
}

int Test::networkValid() {
    std::vector<Layer> v{Layer(100, 50), Layer(50, 1)};
    Network n(v);
    bool one = n.checkValid();

    std::vector<Layer> v{Layer(100, 50), Layer(51, 1)};
    Network n2(v);
    
    bool two = n2.checkValid();

    if(one && !two) return 0;
    return 1;
}

int Test::networkDimensionMismatch() {
    std::vector<Layer> v{Layer(100, 50), Layer(50, 1)};
    Network n(v);
    torch::Tensor input = torch::ones(110);
    torch::Tensor out;
    
    try {
        out = n.forward(input);
    } catch(...) {
        return 0;
    }

    return 1;
}

