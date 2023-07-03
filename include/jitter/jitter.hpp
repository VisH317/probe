#ifndef __JITTER_H__
#define __JITTER_H__

// needs to include methods: main process, holding objective + objective loss updates with log, queue for final distribution + weight update operations(with call to metrics for updating), log of operations
// aux methods: task distribution, objective loss calculation, objective loss updates, objective loss logging, final distribution + weight update operation with queue + log, stop

// objective and metrics need to be refactored to have a background queue-based process to take in each objective
// objective only should be updated along with dist

class Jitter {

};

#endif
