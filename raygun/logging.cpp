// The MIT License (MIT)
//
// Copyright (c) 2019-2021 The Raygun Authors.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "raygun/logging.hpp"

#include "raygun/config.hpp"
#include "raygun/info.hpp"

namespace raygun {

std::shared_ptr<spdlog::logger> g_logger;

// Static initialization of logging module.
struct LoggerSetup {
    LoggerSetup()
    {
        if(g_logger) {
            // Logger already initialized.
            return;
        }

        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        g_logger = std::make_shared<spdlog::logger>(RAYGUN_NAME, consoleSink);

        g_logger->set_pattern("%T.%e %^%L%$ [%n] %v");

#ifdef NDEBUG
        g_logger->flush_on(spdlog::level::warn);
#else
        g_logger->set_level(spdlog::level::debug);
        g_logger->flush_on(spdlog::level::debug);
#endif

        try {
            const auto logFile = configDirectory() / fs::path{RAYGUN_NAME ".log"};
            fs::remove(logFile);

            const auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.string());
            g_logger->sinks().push_back(fileSink);
        }
        catch(const fs::filesystem_error& e) {
            g_logger->warn("Unable to use log file: {}", e.what());
        }

        g_logger->info("Running on " RAYGUN_NAME " " RAYGUN_VERSION);
    }
} loggerSetup;

} // namespace raygun
