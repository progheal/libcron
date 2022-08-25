#pragma once

#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include "Task.h"

namespace libcron
{           
    template<typename LockType>
    class TaskQueue
    {
        public:
            const std::list<Task>& get_tasks() const
            {
                return c;
            }
            
            std::list<Task>& get_tasks()
            {
                return c;
            }
            
            size_t size() const noexcept
            {
                return c.size();
            }
            
            bool empty() const noexcept
            {
                return c.empty();
            }
            
            void push(Task& t)
            {
                c.push_back(std::move(t));
            }
            
            void push(Task&& t)
            {
                c.push_back(std::move(t));
            }
            
            void push(std::list<Task>& tasks_to_insert)
            {
                c.splice(c.end(), tasks_to_insert);
            }
            
            const Task& top() const
            {
                return *(c.begin());
            }
            
            void sort()
            {
                c.sort();
            }
            
            void clear()
            {
                lock.lock();
                c.clear();
                lock.unlock();
            }
            
            void remove(Task& to_remove)
            {
                c.remove_if([&to_remove] (const Task& to_compare) {
                                    return to_remove.get_name() == to_compare;
                                    });
            }

            void remove(std::string to_remove)
            {
                lock.lock();
                c.remove_if([&to_remove] (const Task& to_compare) {
                                    return to_remove == to_compare;
                                    });
                lock.unlock();
            }
            
            void lock_queue()
            {
                /* Do not allow to manipulate the Queue */
                lock.lock();
            }
            
            void release_queue()
            {
                /* Allow Access to the Queue Manipulating-Functions */
                lock.unlock();
            }
            
        private:
            LockType lock;
            std::list<Task> c;
    };
}
