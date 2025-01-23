use std::{env, io::Write, path::PathBuf};

fn main() {
    let dst = cmake::Config::new("cpp_algo").very_verbose(true).build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-search=native=/usr/local/lib");
    println!("cargo:rustc-link-lib=static=cpp_algo");
    println!("cargo:rustc-link-lib=static=ta-lib");

    cxx_build::bridge("src/bridge.rs")
        .flag_if_supported("--std=c++14")
        .file("cpp_algo/AlgoInterface.cpp")
        .cpp(true)
        .includes(&["cpp_algo/core/include"])
        .warnings(false)
        .extra_warnings(false)
        .warnings_into_errors(false)
        .compile("cpp-algo-cxx-bridge");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=cpp_algo/AlgoInterface.h");
    println!("cargo:rerun-if-changed=cpp_algo/AlgoInterface.cpp");

    // Build rust bindings
    let bindings = bindgen::Builder::default()
        .header("cpp_algo/AlgoInterface.h")
        .clang_arg("--std=c++14")
        .clang_arg("-x")
        .clang_arg("c++")
        .clang_arg(format!("-Icpp_algo/core/include/"))
        .blocklist_function(".*")
        .allowlist_type("cpp_algo::AlgoInterface")
        .allowlist_type("cpp_algo::AlgoParams")
        .allowlist_type("cpp_algo::CandleInput")
        .allowlist_type("cpp_algo::CandleOutput")
        .allowlist_type("cpp_algo::HistoricalDataInput")
        .allowlist_type("cpp_algo::HistoricalDataOutput")
        .allowlist_type("cpp_algo::CppCandle")
        .allowlist_type("cpp_algo::CppAdvice")
        .allowlist_type("cpp_algo::Side")
        .layout_tests(false)
        .enable_cxx_namespaces()
        .generate()
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    std::fs::OpenOptions::new()
        .write(true)
        .truncate(false)
        .create(true)
        .open(out_path.join("bindings.rs"))
        .expect("Couldn't open bindings file!")
        .write(bindings.to_string().as_bytes())
        .expect("Couldn't write bindings!");
}
