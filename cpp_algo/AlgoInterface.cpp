#include "AlgoInterface.h"

namespace cpp_algo
{
    static CppAlgo cpp_algo_obj;

    AlgoInterface::AlgoInterface(AlgoParams const &parameters)
    {
        std::vector<int> candle_periods;
        for (int i = 0; i < NUM_CANDLE_PERIODS; ++i)
        {
            candle_periods.push_back(parameters.candle_periods[i]);
        }

        // Create CppAlgo with init parameters
        cpp_algo_obj = CppAlgo(parameters.rsi_period, parameters.rsi_buy_threshold, parameters.rsi_sell_threshold, candle_periods);
    }

    void AlgoInterface::tick_update(const TickInput &tick, TickOutput &tick_output)
    {
        // stub out as cpp algo example doesnt act on tick data
    }

    void AlgoInterface::candle_update(const CandleInput &candle, CandleOutput &candle_output)
    {
        // CPP Algo candle handler
        bool advice_created = cpp_algo_obj.handle_candle(candle.period_s, CppCandle{candle.timestamp_ns, candle.close});

        // Set candle output so rust node can read
        if (advice_created)
        {
            candle_output.curr_advice = cpp_algo_obj.get_advice();
        }
        candle_output.rsi = cpp_algo_obj.get_rsi_val(candle.period_s);
    }
    void AlgoInterface::historical_data_update(const HistoricalDataInput &hd, HistoricalDataOutput &hd_output)
    {
        std::vector<CppCandle> candles;

        for (int i = 0; i < hd.num_candles; ++i)
        {
            candles.push_back(CppCandle{hd.historical_candles[i].timestamp_ns, hd.historical_candles[i].close});
        }

        // CPP algo historical data handler
        cpp_algo_obj.handle_historical_data(hd.period_s, candles);

        // Set historical data output so rust node can read
        std::vector<RsiVal> rsi = cpp_algo_obj.get_rsi_line(hd.period_s);

        hd_output.num_rsi = rsi.size();
        hd_output.period_s = hd.period_s;

        for (int i = 0; i < rsi.size(); ++i)
        {
            hd_output.historical_rsi[i].timestamp_ns = rsi[i].timestamp_ns;
            hd_output.historical_rsi[i].value = rsi[i].value;
        }
    }

    std::unique_ptr<AlgoInterface> new_algo_interface(AlgoParams const &params)
    {
        return std::make_unique<AlgoInterface>(params);
    }

    std::unique_ptr<HistoricalDataInput> new_historical_data_input()
    {
        return std::make_unique<HistoricalDataInput>();
    }

}

/*================================================================================================================*/