#include "server.h"
#include "logger.h"
#include <iostream>
#include <cstdlib>
#include <thread>

int main(int argc, char* argv[]) {
    uint16_t port = 8080;
    size_t threads = std::thread::hardware_concurrency();
    std::string root = "www";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "-p" || arg == "--port") && i + 1 < argc) {
            port = static_cast<uint16_t>(std::atoi(argv[++i]));
        }
        else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            threads = std::atoi(argv[++i]);
        }
        else if ((arg == "-r" || arg == "--root") && i + 1 < argc) {
            root = argv[++i];
        }
        else if (arg == "-h" || arg == "--help") {
            std::cout << "MiniHTTP Server v1.0\n\n"
                      << "Usage: http_server [options]\n\n"
                      << "Options:\n"
                      << "  -p, --port <port>       Port (default: 8080)\n"
                      << "  -t, --threads <count>   Thread pool size (default: "
                      << std::thread::hardware_concurrency() << ")\n"
                      << "  -r, --root <path>       Static files dir (default: www)\n"
                      << "  -h, --help              Show this message\n";
            return 0;
        }
    }

    if (threads == 0) threads = 4;

    Server server(port, threads, root);
    server.start();

    return 0;
}
