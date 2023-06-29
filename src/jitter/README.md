## Energy and optimization system

Considerations:
 * Want it to speed up when we know jittering one input causes a desired increase: increase a parameter
 * Want to do it based on the efficacy of that neuron on other weights (do not disrupt)
 * Want it to slow down as we get closer to the objective
 * want to have a configurable hyperparameter to determine maximum magniude of variation


Boltzmann dist - will increase with higher energy, but will be dampened by the temperature, not necessary

Implementation:
 * calculate an alpha parameter to boost a scalar on each time towards the desired direction, and alter based on the different conditions of the parameter
 * Pieces:
   * alpha parameter/temperature - determines magnitude/standard deviation of sampling random variation
   * multiply the magnitude by the weight of the connection
   * shift distribution mean in direction of favorable direction, otherwise shift in opposite
   * store distribution parameters for later passes