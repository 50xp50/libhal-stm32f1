// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cinttypes>

#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-stm32f1/adc.hpp>
#include <libhal-stm32f1/clock.hpp>
#include <libhal-stm32f1/constants.hpp>
#include <libhal-stm32f1/uart.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>

void application()
{
  auto cpu_frequency = hal::stm32f1::frequency(hal::stm32f1::peripheral::cpu);
  hal::cortex_m::dwt_counter steady_clock(cpu_frequency);
  hal::stm32f1::adc adc8(hal::channel<8>);
  hal::stm32f1::uart uart1(hal::port<1>, hal::buffer<128>);
  hal::print(uart1, "[stm32f1] Starting ADC demo...\n");

  while (true) {
    using namespace std::chrono_literals;

    // Read ADC value
    auto percent8 = adc8.read();
    // Get current uptime
    auto uptime = steady_clock.uptime();

    hal::print<128>(uart1,
                    "%" PRId32 "%%: %" PRIu32 "ns\n",
                    static_cast<std::int32_t>(percent8 * 100),
                    static_cast<std::uint32_t>(uptime));
    hal::delay(steady_clock, 100ms);
  }
}
