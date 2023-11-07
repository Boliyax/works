#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <thread>
#include <array>
#include <mutex>
#include <functional>
#include <condition_variable>

class FileKeysGenerator {
public:

    std::pair<time_t, uint32_t> get_keys() {
        std::time_t t;
        {
            // Теперь уникальность названия гарантированна в многопоточном режиме.
            std::lock_guard unique_key_guard(unique_key_mutex);
            t = std::time(nullptr);
            if(t == last_request_time) {
                ++secondary_key;
            } else {
                secondary_key =  0;
            }
            last_request_time = t;
        }
        
        return std::make_pair(t, secondary_key);
    }

private:
    time_t last_request_time = 0;
    uint32_t secondary_key = 0;
    std::mutex unique_key_mutex;

} key_generator;



std::ofstream generate_output_file() {
    auto [time, code] = key_generator.get_keys();
    std::ofstream file("bulk" + std::to_string(time) + "_" + std::to_string(code) + ".log");
    file << "bulk: ";
    return file;
}

struct ParallelOutputStruct {
    std::list<std::string> commands;
    std::ofstream stream;
    std::mutex mutex;
    bool worked_out = false;

    ParallelOutputStruct(std::list<std::string>&& c, std::ofstream&& s)
        :commands(std::move(c)), stream(std::move(s)), mutex() {} 
};

struct OneThreadOutputStruct {
    std::list<std::string> commands;
    bool worked_out = false;
    OneThreadOutputStruct(std::list<std::string>&& c): commands(std::move(c)) {};
};


class Output {
    bool end = false;
    std::list<std::shared_ptr<ParallelOutputStruct>> file_output_queue;
    std::mutex file_output_queue_mutex;
    std::condition_variable file_output_condition;
    std::condition_variable console_output_condition;
    std::list<OneThreadOutputStruct> console_output_queue;
    std::mutex console_output_queue_mutex;
    std::thread console_thread;
    std::array<std::thread, 2> files_threads;
    std::list<std::string> mixed_output_list;
    std::mutex mixed_output_mutex;

public:
    void file_parallel_ouput() {
        for(;;) {
            {
                std::shared_ptr<ParallelOutputStruct> output_point;
                {
                    std::unique_lock lock(file_output_queue_mutex);
                    while(file_output_queue.empty()) {
                        if(end) break;
                        file_output_condition.wait(lock);
                    }
                    if(file_output_queue.empty() && end) break;
                    if(file_output_queue.front()->worked_out) {
                        file_output_queue.pop_front();
                        continue;
                    }
                    output_point = file_output_queue.front();
                }
                for(;;)
                {
                    std::unique_lock lock(output_point->mutex);
                    if(output_point->commands.empty()){
                        output_point->worked_out = true;
                        break;
                    }
                    auto command = std::move(output_point->commands.front());
                    output_point->commands.pop_front();
                    output_point->stream << command;
                    if(!output_point->commands.empty()) output_point->stream << ", ";
                }
            }
        }
    }
    void console_ouput() {
        for(;;) {
            {
                {
                    std::unique_lock lock(console_output_queue_mutex);
                    while(console_output_queue.empty()) {
                        if(end) break;
                        console_output_condition.wait(lock);
                    }
                    if(console_output_queue.empty() && end) break;
                    if(console_output_queue.front().worked_out) {
                        console_output_queue.pop_front();
                        continue;
                    }
                }
                std::cout << "bulk: ";
                for(;;)
                {   
                    if(console_output_queue.front().commands.empty()){
                        console_output_queue.front().worked_out = true;
                        break;
                    }
                    auto command = std::move(console_output_queue.front().commands.front());
                    console_output_queue.front().commands.pop_front();
                    std::cout << command;
                    if(!(console_output_queue.front().commands.empty())) std::cout << ", ";
                }
                std::cout << std::endl;
            }
        }
    }
public:
    Output(): console_thread(std::bind(&Output::console_ouput, this)),
        files_threads{std::thread(std::bind(&Output::file_parallel_ouput, this)), std::thread(std::bind(&Output::file_parallel_ouput, this))}{}
    void print(const std::list<std::string>& command_set) {
        if(command_set.empty()) return;
        {
            std::ofstream file = generate_output_file();
            std::list<std::string> file_command_list(command_set);
            std::unique_lock guard(file_output_queue_mutex);
            file_output_queue.push_back(std::shared_ptr<ParallelOutputStruct>(new ParallelOutputStruct(std::move(file_command_list), std::move(file))));
            file_output_condition.notify_all(); 
        }
        {
            std::list<std::string> console_command_list(command_set);
            std::unique_lock guard(console_output_queue_mutex);
            console_output_queue.push_back(OneThreadOutputStruct(std::move(console_command_list)));
            console_output_condition.notify_all();
        }
    }
    void add_static_command(const std::string& command) {
        std::lock_guard guard(mixed_output_mutex);
        mixed_output_list.push_back(command);
    }
    void mix_print(size_t s) {
        std::lock_guard guard(mixed_output_mutex);
        std::list<std::string> out;
        for(; s-- ;((!mixed_output_list.empty()) && (s > 0))) {
            out.push_back(mixed_output_list.front());
            mixed_output_list.pop_front();
        }
        print(out);
    }
    
    
    ~Output() {
        end = true;
        file_output_condition.notify_all();
        console_output_condition.notify_all();
        console_thread.join();
        files_threads[0].join();
        files_threads[1].join();
        if(files_threads[0].joinable()) {
            end = false;
        }
    }
};