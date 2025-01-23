use cxx::{type_id, ExternType};

// Include the bindings
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

unsafe impl ExternType for root::cpp_algo::AlgoInterface {
    type Id = type_id!("cpp_algo::AlgoInterface");
    type Kind = cxx::kind::Trivial;
}

unsafe impl ExternType for root::cpp_algo::AlgoParams {
    type Id = type_id!("cpp_algo::AlgoParams");
    type Kind = cxx::kind::Trivial;
}

unsafe impl ExternType for root::cpp_algo::CandleOutput {
    type Id = type_id!("cpp_algo::CandleOutput");
    type Kind = cxx::kind::Trivial;
}

unsafe impl ExternType for root::cpp_algo::CandleInput {
    type Id = type_id!("cpp_algo::CandleInput");
    type Kind = cxx::kind::Trivial;
}

unsafe impl ExternType for root::cpp_algo::HistoricalDataOutput {
    type Id = type_id!("cpp_algo::HistoricalDataOutput");
    type Kind = cxx::kind::Trivial;
}

unsafe impl ExternType for root::cpp_algo::HistoricalDataInput {
    type Id = type_id!("cpp_algo::HistoricalDataInput");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge(namespace = "cpp_algo")]
mod bridge {
    unsafe extern "C++" {
        include!("cpp-algo-example/cpp_algo/AlgoInterface.h");

        type AlgoInterface = super::root::cpp_algo::AlgoInterface;
        type AlgoParams = super::root::cpp_algo::AlgoParams;
        type CandleOutput = super::root::cpp_algo::CandleOutput;
        type CandleInput = super::root::cpp_algo::CandleInput;
        type HistoricalDataOutput = super::root::cpp_algo::HistoricalDataOutput;
        type HistoricalDataInput = super::root::cpp_algo::HistoricalDataInput;

        fn candle_update(
            self: Pin<&mut AlgoInterface>,
            candle: &CandleInput,
            candle_output: &mut CandleOutput,
        );

        fn historical_data_update(
            self: Pin<&mut AlgoInterface>,
            candle: &HistoricalDataInput,
            candle_output: &mut HistoricalDataOutput,
        );
        fn new_algo_interface(params: &AlgoParams) -> UniquePtr<AlgoInterface>;
        fn new_historical_data_input() -> UniquePtr<HistoricalDataInput>;
    }

    impl UniquePtr<AlgoInterface> {}
    impl UniquePtr<HistoricalDataInput> {}
}

unsafe impl Send for bridge::AlgoInterface {}

pub use bridge::{
    new_algo_interface, new_historical_data_input, AlgoInterface, AlgoParams, CandleInput,
    CandleOutput, HistoricalDataInput, HistoricalDataOutput,
};
pub use root as ffi;
