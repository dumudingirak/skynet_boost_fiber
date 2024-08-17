#include <algorithm>
#include <vector>

namespace fiber_benchmark {

    struct result_t
    {
        std::size_t _median;
        double _avg;
        std::size_t _min;
        std::size_t _max;

        public:
            result_t() noexcept = default;


            static auto generate_result(std::vector<std::size_t> result_vec) -> result_t{
                result_t output{};
                output._median = median(result_vec);
                output._avg = average(result_vec);
                output._min = min(result_vec);
                output._max = max(result_vec);
                return std::move(output);
            }

            static auto median(std::vector<std::size_t>& vec) noexcept -> std::size_t
            {
                if(vec.empty()) {
                    return 0.0; // Handle empty vector case
                }

                std::sort(vec.begin(), vec.end());
                const auto size = vec.size();

                if(size % 2 == 0) {
                    return (vec[size / 2 - 1] + vec[size / 2]) / 2;
                } else {
                    return vec[size / 2];
                }
            }
            static auto average(const std::vector<std::size_t>& vec) noexcept -> double
            {
                if(vec.empty()) {
                    return 0.0;
                }

                auto average = 0.0;
                std::size_t count = 0;

                for(std::size_t value : vec) {
                    count++;
                    average += (value - average) / count;
                }

                return average;
            }

            static auto min(const std::vector<std::size_t>& vec) noexcept -> std::size_t
            {
                return *std::min_element(std::begin(vec), std::end(vec));
            }
            static auto max(const std::vector<std::size_t>& vec) noexcept -> std::size_t
            {
                return *std::max_element(std::begin(vec), std::end(vec));
            }
    };
};