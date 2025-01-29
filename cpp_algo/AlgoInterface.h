#pragma once
#include <memory>
#include "cpp_algo.h"

namespace cpp_algo
{
    static const int NUM_CANDLE_PERIODS = 2;
    static const int MAX_HISTORICAL_DATA_POINTS = 5760; // 3 days of one minute candles is max we will need to store

    // Algo parameters entry point, fill in algo params here
    struct AlgoParams
    {
        int rsi_period;
        double rsi_buy_threshold;
        double rsi_sell_threshold;
        int candle_periods[NUM_CANDLE_PERIODS];
    };

    // Input to tick handler, fill in desired input, if any
    struct TickInput
    {
    };

    // Output from tick handler, fill in algo output from a tick message, if any
    struct TickOutput
    {
    };

    // Input to candle handler, fill in desired input, if any
    struct CandleInput
    {
        uint64_t timestamp_ns;
        uint32_t period_s;
        float close;
    };

    // Output from candle handler, fill in algo output from a candle message, if any
    struct CandleOutput
    {
        CppAdvice curr_advice;
        RsiVal rsi;
    };

    // Input to historical data handler, fill in desired input, if any
    struct HistoricalDataInput
    {
        uint32_t period_s;
        uint32_t num_candles;
        CppCandle historical_candles[MAX_HISTORICAL_DATA_POINTS];
    };

    // Output from historical data handler, fill in algo output from a historical data message, if any
    struct HistoricalDataOutput
    {
        uint32_t period_s;
        uint32_t num_rsi;
        RsiVal historical_rsi[MAX_HISTORICAL_DATA_POINTS];
    };

    struct AlgoInterface
    {
        AlgoInterface(AlgoParams const &);

        // Tick handler
        void tick_update(const TickInput &tick, TickOutput &tick_response);

        // Candle handler
        void candle_update(const CandleInput &candle, CandleOutput &tick_response);

        // Historical Data handler
        void historical_data_update(const HistoricalDataInput &hd, HistoricalDataOutput &hd_response);

        // Store any transient state in below here
        CppAlgo cpp_algo_obj;
    };

    std::unique_ptr<AlgoInterface> new_algo_interface(AlgoParams const &);
    std::unique_ptr<HistoricalDataInput> new_historical_data_input();

};
