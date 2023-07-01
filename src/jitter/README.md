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
   * shift distribution mean in direction of favorable direction, otherwise shift in opposite (use a beta distribution)
   * store distribution parameters for later passes
 * step 1: initialize dp (kinda?) with map of neuron uuids to distribution parameters
 * step 2: for input: sample random parameter change and measure objective change (log the change)
   * if positive: shift distribution by alpha parameter (dont go past 1 to keep bell curve)
     * alpha += 
   * move to future node:
     * calculate new distribution params:
       * alpha = previous alpha + coming alpha * weight (avg for now) * lambda (limiter param)
       * beta is same equation
     * sample new distribution, make change, log, and measure
     * continue moving through each future node if change works
 * step 3: move to next input node after change stops being positive
   * use saved distribution parameters and continue

Multithreaded design:
 * multiple threads communicating with a master process
 * master process monitors after each change to determine when to stop
   * consists of a metrics class
 * during each recursive iteration:
   * will poll the latest distribution info
   * will record an index at the time of latest network update
 * if successful - will continue on with algorithm but also send a message back to the master process to instill updates in the main network
 * master node will sort out concurrencies etc.