#ifndef GLPAINT_WINDOW_LAYER_HPP
#define GLPAINT_WINDOW_LAYER_HPP

#include "Window.hpp"

#include <Event.hpp>
#include <RingQueue.hpp>
#include <common.hpp>

namespace glpaint {

using EventQueue = RingQueue<std::unique_ptr<Event>, 128>;

struct WindowLayer {
private:
    EventQueue& event_queue_;

public:
    Window window; 

    WindowLayer(EventQueue& event_queue); 

    void deinit(); 
};

}

#endif