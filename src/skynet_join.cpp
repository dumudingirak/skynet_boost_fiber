
//          Copyright Oliver Kowalke 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// based on https://github.com/atemerev/skynet from Alexander Temerev 

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include <boost/fiber/all.hpp>
#include <boost/predef.h>
#include "result.hpp"

using allocator_type = boost::fibers::fixedsize_stack;
using channel_type = boost::fibers::buffered_channel< std::uint64_t >;
using clock_type = std::chrono::steady_clock;
using duration_type = clock_type::duration;
using time_point_type = clock_type::time_point;

// microbenchmark
void skynet( allocator_type & salloc, channel_type & c, std::size_t num, std::size_t size, std::size_t div) {
    if ( 1 == size) {
        c.push( num);
    } else {
        channel_type rc{ 16 };
        std::vector< boost::fibers::fiber > fibers;
        for ( std::size_t i = 0; i < div; ++i) {
            auto sub_num = num + i * size / div;
            fibers.emplace_back( boost::fibers::launch::dispatch,
                                 std::allocator_arg, salloc,
                                 skynet,
                                 std::ref( salloc), std::ref( rc), sub_num, size / div, div);
        }
        for ( auto & f: fibers) {
            f.join();
        }
        std::uint64_t sum{ 0 };
        for ( std::size_t i = 0; i < div; ++i) {
            sum += rc.value_pop();
        }
        c.push( sum);
    }
}

auto do_skynet_benchmark(std::size_t iterations, const size_t size, const size_t div) -> fiber_benchmark::result_t{
    std::vector<std::size_t> results{};
    for(int i = 0; i < iterations; i++){
        allocator_type salloc{ 3*allocator_type::traits_type::page_size()};
        std::uint64_t result{ 0 };
        channel_type rc{ 2 };
        time_point_type start{ clock_type::now() };
        skynet( salloc, rc, 0, size, div);
        result = rc.value_pop();
        if ( 499999500000 != result) {
            throw std::runtime_error("invalid result");
        }
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - start);
        results.emplace_back(duration.count());
        std::cout << "Iteration " << i << " finished after " << duration.count() << "ns" << std::endl;
    }
    return fiber_benchmark::result_t::generate_result(results);
}


int main() {
    try {
        const std::size_t size{ 1000000 };
        const std::size_t div{ 10 };
        auto result = do_skynet_benchmark(20, size, div);
        std::cout << "============================================\n";
        std::cout << "skynet results:\n"
                << "median: " << result._median / 1'000'000 << "ns\n"
                << "avg: " << result._avg / 1'000'000 << "ns\n"
                << "min: " << result._min / 1'000'000 << "ns\n"
                << "max: " << result._max / 1'000'000 << "ns\n";
        std::cout << "============================================\n";
        return EXIT_SUCCESS;
    } catch ( std::exception const& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unhandled exception" << std::endl;
    }
	return EXIT_FAILURE;
}
