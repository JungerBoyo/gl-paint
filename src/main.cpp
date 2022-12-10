#include <App.hpp>

int main() {
    auto& app = glpaint::App::instance();
    app.run();
    app.deinit();
}