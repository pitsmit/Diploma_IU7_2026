        std::jthread loopThread([&] {
            try {
                loop.run();
            }
            catch (const std::exception& e) {
                std::cerr << "loop thread: "
                        << e.what()
                        << std::endl;
            }
        });
    #ifdef BUILD_HTTP_SERVER
        std::jthread httpThread([&] {
            try {
                http.start();
            }
            catch (const std::exception& e) {
                std::cerr << "http thread: "
                        << e.what()
                        << std::endl;
            }
        });
    #endif
        for (;;) {
            std::this_thread::sleep_for(
                std::chrono::seconds(10));
        }
    }
};
int main() { App().run(); }