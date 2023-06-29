## Jitter Algorithm

_Description:_ An algorithm to introduce random changes into network parameters to reach a certain amount of output distortion

_Inspirations:_ Ising model, Boltzmann distribution, tree algorithms

**General Algorithm Flow:**
1. move along each input node - induce random change using boltzmann distribution
   1. distribution is based on an energy value - proportional to the amount of output change caused by the random change
2. If random change is favorable - move to next node and induce similar random change with higher energy for same direction
3. continue until objective reached


**Implementation Details:**
 * Need to link the neural network probing package
 * need to set up energy values for each neuron - update algorithm as the input energy values pass through
 * need to set up random selection + increment algorithm
 * need to set up tree search algorithm and break on objective function