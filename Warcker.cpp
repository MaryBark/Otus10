#include "Warcker.h"

error::Warcker::Warcker(const std::string &name,
                        std::shared_mutex &mutex,
                        QueueType &queue,
                        const std::atomic_bool &endFlag):
    thread([this]() { run(); }),
    name(name),
    mutex(mutex),
    queue(queue),
    endFlag(endFlag)
{}

error::Warcker::~Warcker()
{
    thread.join();
}

void error::Warcker::run()
{
    while (!endFlag)
    {
        bool hasWork { };

        if (mutex.try_lock())
        {
            if (!queue.empty())
            {
                hasWork = true;
                execCritical();
                ++blocksCounter;
                commandsCounter += queue.front().second;
                queue.pop();
            }
            mutex.unlock();
            if (hasWork) execPostCritical();
        }
    }
}



void error::WarckerStdout::execCritical()
{
    std::cout << queue.front().first;
}

void error::WarckerFile::execCritical()
{
    buf = queue.front().first;
}

void error::WarckerFile::execPostCritical() {
    std::ofstream file { generateFileName(), std::ios_base::app };
    file << buf; // FIXME It seems not thread safe and fires garbage sometimes
    file.close();
}

std::string error::WarckerFile::generateFileName()
{
    auto now
    {
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
    };

    std::stringstream path { };
    path
            << "bulk"
            << std::to_string(now)
            << '-'
            << std::setfill('0') << std::setw(5) << std::to_string(++index)
            << ".log";
    return path.str();
}

