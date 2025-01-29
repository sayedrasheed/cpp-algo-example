# cpp-algo-example
Example C++ algorithm that is integrated into TradeBot. This C++ algorithm is just a simple algortithm that uses RSI to send a buy and sell signals. The RSI buy and sell thresholds are set as runtime parameters. Depending on these thresholds and what the calculated RSI is, it will advise to BUY or SELL accordingly. NOTE: This is a simple algo just used as an example and should be not be used in practice. 

This crate uses cxx and bindgen to generate C++ Rust bindings so we can execute in Rust. Integration and execution of this C++ algoirthm can be found here:

https://github.com/sayedrasheed/tradebot-rs/blob/master/algo-service/src/cpp_algo/cpp_algo.rs

# Installation
1. Download and install [CMake](https://cmake.org/download/)
2. Download and install [Rust](https://www.rust-lang.org/tools/install). The version I use is 1.85.0
3. This C++ algo uses the C library TA-Lib so that needs to be installed. Follow these instructions [here](https://ta-lib.org/install/)

# Usage
Algorithm is executed via a generic AlgoInterface which all algorithms will implement. The header file can be found [here](https://github.com/sayedrasheed/cpp-algo-example/blob/master/cpp_algo/AlgoInterface.h)

The AlgoParams struct is designed to contain runtime parameters that each Algo can have. These parameters are set in TradeBot via a yaml file then passed into this Algo via the AlgoParams struct. See how that is done here:

https://github.com/sayedrasheed/tradebot-rs/blob/master/algo-service/src/cpp_algo/cpp_algo.rs#L176

There are 3 execution entry points into the algo via the AlgoInterface.
1. [tick_update](https://github.com/sayedrasheed/cpp-algo-example/blob/master/cpp_algo/AlgoInterface.h#L64)
2. [candle_update](https://github.com/sayedrasheed/cpp-algo-example/blob/master/cpp_algo/AlgoInterface.h#L67)
3. [historical_data_update](https://github.com/sayedrasheed/cpp-algo-example/blob/master/cpp_algo/AlgoInterface.h#L70)

Each one of these functions will have their own input and output that needs to be defined by the algo. If your algo doesn't use any of the functions then you can stub them out.

These functions will then be called by TradeBot any time the node receives the corressponding message. See here for the CppAlgo candle_update execution:

https://github.com/sayedrasheed/tradebot-rs/blob/master/algo-service/src/cpp_algo/cpp_algo.rs#L236

See video below for an example of this algorithm running in TradeBot:

