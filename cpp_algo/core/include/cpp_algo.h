#ifndef CPP_ALGO_H
#define CPP_ALGO_H

#include <cstdint>
#include <vector>
#include <unordered_map>

namespace cpp_algo
{
    enum Side
    {
        BUY,
        SELL
    };

    struct CppCandle
    {
        uint64_t timestamp_ns;
        double close;
    };

    struct RsiVal
    {
        uint64_t timestamp_ns;
        double value;
    };

    struct CppAdvice
    {
        bool is_valid;
        uint64_t timestamp_ns;
        int32_t size;
        double price;
        Side side;
    };

    class CppAlgo
    {
    public:
        CppAlgo();
        CppAlgo(int32_t rsi_period, double rsi_buy_threshold, double rsi_sell_threshold, std::vector<int32_t> &candle_periods);

        bool handle_candle(int32_t period_s, CppCandle close);
        void handle_historical_data(int32_t period_s, std::vector<CppCandle> historical_closes);

        RsiVal get_rsi_val(int32_t period_s);
        std::vector<RsiVal> get_rsi_line(int32_t period_s);
        CppAdvice get_advice();

    private:
        std::unordered_map<int32_t, std::vector<RsiVal>> rsi_map;
        std::unordered_map<int32_t, std::vector<CppCandle>> candle_map;
        int32_t rsi_period;
        double rsi_buy_threshold;
        double rsi_sell_threshold;

        bool long_position;
        CppAdvice curr_advice;
    };
}

#endif