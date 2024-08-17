# skynet_boost_fiber

This repository contains enhanced versions of the benchmark files, created by the authors of the Boost::fiber library, notably Oliver Kowalke. Instead of only performing the skynet benchmark for a single instance, the benchmark is repeatedly executed, which enables the user to obtain more firmly determined values for the execution times. As for this version of the benchmark, the average and median over all measured durations is ourput, as well as the minimal and maximal duration count. 

Additionally, a CMake-File is provided for building and executing the benchmarks binaries. 

**WARNING: The "shared" benchmarks, using the NUMA-aware extension of Boost::fiber, do not work with CMake yet**
