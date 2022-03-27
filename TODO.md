
* TODO better organization of internal & public header files.
* TODO unify containers with a template implementing all operations,
  type-free.
* TODO add histogram
* TODO add runtimeMetrics
* TODO move _ops in data/* to a constructor arg?
* TODO remove operators new & delete for o1::d_linked::node ?
* TODO Reduce memory footprint of some container objects:
  * d_linked::node: could define specialized d_linked::list, w/out size(),
    hence no need for EventHandlers.
  * d_linked::node: how to avoid ref() additional pointer, w/out requiring
    standard layout.
