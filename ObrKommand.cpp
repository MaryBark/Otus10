#include "ObrKommand.h"

error::ObrKommand::ObrKommand() {
    workers.emplace_back( new WarckerStdout(
                              "log",
                              stdstreamMutex,
                              stdoutInputQueue,
                              done));

    for (int i { 1 }; i < 3; ++i)
        workers.emplace_back(new WarckerFile(
                                 "file" + std::to_string(i),
                                 fileMutex,
                                 fileInputQueue,
                                 done));
}

error::ObrKommand::~ObrKommand() {
    while (true) {
        std::shared_lock lock { stdstreamMutex };
        if (stdoutInputQueue.empty()) break;
    }
    while (true) {
        std::shared_lock lock { fileMutex };
        if (fileInputQueue.empty()) break;
    }
    done = true;
    {
        std::unique_lock lock { stdstreamMutex };
        std::cerr << mainStatistics;
        for (auto const &worker: workers) {
            std::cerr << std::string(*worker);
        }
    }
}

void error::ObrKommand::print(const std::string &block, unsigned blockSize) {
    {
        std::unique_lock lock { stdstreamMutex };
        stdoutInputQueue.push(std::make_pair(block, blockSize));
    }
    {
        std::unique_lock lock { fileMutex };
        fileInputQueue.push(std::make_pair(block, blockSize));
    }
}

void error::ObrKommand::setMainStatistics(std::string_view mainStatistics) {
    this->mainStatistics = mainStatistics;
}
