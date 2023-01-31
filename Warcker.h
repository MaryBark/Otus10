#ifndef WARCKER_H
#define WARCKER_H

#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>

namespace error
{

  class Warcker {
  public:
    using QueueType = std::queue<std::pair<std::string, unsigned>>;

    Warcker(
        std::string const &name,
        std::shared_mutex &mutex,
        QueueType &queue,
        std::atomic_bool const &endFlag);

    virtual ~Warcker();

    virtual operator std::string () const {
      std::stringstream ss;
      ss
        << "Thread "
        << name
        << ": "
        << blocksCounter
        << " blocks, "
        << commandsCounter
        << " commands."
        << std::endl;
      return ss.str();
    };

  protected:
    std::thread thread;
    std::string name;
    std::shared_mutex &mutex;
    QueueType &queue;
    std::atomic_bool const &endFlag;
    unsigned blocksCounter { };
    unsigned commandsCounter { };

    void run();

    virtual void execCritical() = 0;

    virtual void execPostCritical() { }

  };

  class WarckerStdout: public Warcker
  {
    using Warcker::Warcker;
    void execCritical() override;
  };

  class WarckerFile: public Warcker
  {
    using Warcker::Warcker;

    std::string buf { };
    static inline std::atomic_uint index { };

    void execCritical() override;

    void execPostCritical() override;

    std::string generateFileName();
  };
}

#endif // WARCKER_H
