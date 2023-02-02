#include <App.hpp>
#include <spdlog/spdlog.h>

int main() {
    try {
        auto& app = glpaint::App::instance();
        app.run();
        app.deinit();
    } catch(const std::exception& e) {
        spdlog::critical(e.what());
    }
}