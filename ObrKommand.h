#ifndef OBRKOMMAND_H
#define OBRKOMMAND_H

#pragma once

#include <atomic>
#include <shared_mutex>
#include <queue>

#include "Warcker.h"

namespace error {
  class IObrKommand {
  public:
    virtual ~IObrKommand() { };
    virtual void print(std::string const &, unsigned) = 0;
    virtual void setMainStatistics(std::string_view) = 0;
  };


  class ObrKommand: public IObrKommand {
  public:
    ObrKommand();

    ~ObrKommand() override;

    void print(std::string const &block, unsigned blockSize) override;

    void setMainStatistics(std::string_view mainStatistics) override;

  private:
    std::atomic_bool done { };
    std::vector<std::unique_ptr<Warcker>> workers { };
    Warcker::QueueType stdoutInputQueue { };
    Warcker::QueueType fileInputQueue { };
    std::shared_mutex stdstreamMutex { };
    std::shared_mutex fileMutex { };
    std::string mainStatistics { };
  };
}


#endif // OBRKOMMAND_H
