#include "cpp_algo.h"
#include "ta-lib/ta_libc.h"

namespace cpp_algo
{
    CppAlgo::CppAlgo()
    {
        std::vector<int> candle_periods = {60, 300};
        rsi_period = 14;
        rsi_buy_threshold = 20.0;
        rsi_sell_threshold = 80.0;

        for (size_t i = 0; i < candle_periods.size(); ++i)
        {
            rsi_map[candle_periods[i]] = std::vector<RsiVal>();
            candle_map[candle_periods[i]] = std::vector<CppCandle>();
        }

        long_position = false;
    }
    CppAlgo::CppAlgo(int rsi_period, double rsi_buy_threshold, double rsi_sell_threshold, std::vector<int> &candle_periods) : rsi_period(rsi_period),
                                                                                                                              rsi_buy_threshold(rsi_buy_threshold),
                                                                                                                              rsi_sell_threshold(rsi_sell_threshold)
    {
        for (size_t i = 0; i < candle_periods.size(); ++i)
        {
            rsi_map[candle_periods[i]] = std::vector<RsiVal>();
            candle_map[candle_periods[i]] = std::vector<CppCandle>();
        }

        long_position = false;
    }

    bool CppAlgo::handle_candle(int period_s, CppCandle close)
    {
        bool advice_created = false;
        if (candle_map.find(period_s) != candle_map.end())
        {
            candle_map[period_s].push_back(close);

            std::vector<double> rsi_out(candle_map[period_s].size());
            int out_beg_idx = 0;
            int out_num_element = 0;

            std::vector<double> closes;
            for (size_t i = 0; i < candle_map[period_s].size(); ++i)
            {
                closes.push_back(candle_map[period_s][i].close);
            }

            TA_RSI(0, closes.size() - 1, closes.data(), 14, &out_beg_idx, &out_num_element, rsi_out.data());
            rsi_map[period_s].push_back(RsiVal{close.timestamp_ns, rsi_out[out_num_element - 1]});

            if (!long_position && rsi_out[out_num_element - 1] < rsi_buy_threshold)
            {
                curr_advice.timestamp_ns = close.timestamp_ns;
                curr_advice.size = 1;
                curr_advice.side = Side::BUY;
                curr_advice.price = close.close;
                curr_advice.is_valid = true;

                long_position = true;
                advice_created = true;
            }
            else if (long_position && rsi_out[out_num_element - 1] > rsi_sell_threshold)
            {
                curr_advice.timestamp_ns = close.timestamp_ns;
                curr_advice.size = 1;
                curr_advice.side = Side::SELL;
                curr_advice.price = close.close;
                curr_advice.is_valid = true;

                long_position = false;
                advice_created = true;
            }
        }

        return advice_created;
    }

    void CppAlgo::handle_historical_data(int period_s, std::vector<CppCandle> historical_closes)
    {
        if (rsi_map.find(period_s) != rsi_map.end())
        {
            std::vector<double> rsi_in;
            for (size_t i = 0; i < historical_closes.size(); ++i)
            {
                rsi_in.push_back(historical_closes[i].close);
                candle_map[period_s].push_back(historical_closes[i]);
            }

            std::vector<double> rsi_out(historical_closes.size());
            int out_beg_idx = 0;
            int out_num_element = 0;

            TA_RSI(0, rsi_in.size() - 1, rsi_in.data(), 14, &out_beg_idx, &out_num_element, rsi_out.data());
            for (int i = 0; i < out_num_element; i++)
            {
                rsi_map[period_s].push_back(RsiVal{historical_closes[out_beg_idx + i].timestamp_ns, rsi_out[i]});
            }
        }
    }

    RsiVal CppAlgo::get_rsi_val(int period_s)
    {
        RsiVal rsi_val{0, 0.0};
        if (rsi_map.find(period_s) != rsi_map.end() && rsi_map[period_s].size() > 0)
        {
            rsi_val = rsi_map[period_s][rsi_map[period_s].size() - 1];
        }

        return rsi_val;
    }

    std::vector<RsiVal> CppAlgo::get_rsi_line(int period_s)
    {
        std::vector<RsiVal> rsi_vals;

        if (rsi_map.find(period_s) != rsi_map.end())
        {
            rsi_vals = rsi_map[period_s];
        }

        return rsi_vals;
    }

    CppAdvice CppAlgo::get_advice()
    {
        return curr_advice;
    }
}