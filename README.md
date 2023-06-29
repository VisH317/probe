## TORCH-PROBE

_purpose:_ A set of tools to analyze and probe parts of neural networks and observe metrics directly within weights

**Features:**
 * Identification - wrappers around torch layers to identify and probe specific neurons
 * Jitter - analytics with parameters through a jitter algorithm to analyze effect of neurons on the network
 * OK I FIGURED IT OUT - use add_subdirectory and target_link_libraries on the added library for each subdirectory, or just append the sources directly