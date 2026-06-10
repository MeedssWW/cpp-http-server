# Multithreaded HTTP Server in C++ 🌐

A lightweight, high-performance multithreaded HTTP server written from scratch in C++17. 
Built without heavy frameworks or external dependencies to demonstrate a deep understanding of POSIX/Winsock networking, concurrency, and thread pooling.

## 🚀 Features

- **Custom Thread Pool:** Efficiently handles multiple concurrent client connections without the heavy overhead of spawning a new thread per request.
- **Cross-Platform Networking:** Uses a platform-agnostic socket abstraction layer (Winsock2 on Windows, POSIX on Linux/macOS).
- **HTTP Parsing & Routing:** Custom request parser and response builder. Supports static file serving (with MIME types) and dynamic JSON endpoints.
- **Zero Dependencies:** Written purely in standard C++17 with raw OS sockets.
- **Developer Dashboard:** Includes a minimalistic real-time status dashboard served directly by the C++ backend.

## 🛠️ Build Instructions

### Requirements
- A modern C++17 compiler (GCC, Clang, or MSVC)
- CMake (version 3.16+)
- Ninja (optional, but recommended)

### Build Steps

```bash
# 1. Clone the repository
git clone https://github.com/MeedssWW/cpp-http-server.git
cd cpp-http-server

# 2. Configure the project with CMake
cmake -B build -G Ninja

# 3. Build the executable
cmake --build build
```

## 💻 Usage

Run the compiled executable. Note: on Windows, the executable will be `http_server.exe`.

```bash
# Default (port 8080, threads = CPU cores)
./build/http_server

# Specify custom port and number of worker threads
./build/http_server -p 3000 -t 8
```

### Endpoints
Once the server is running, you can open your browser or use `curl`:

- `http://localhost:8080/` — Real-time developer dashboard (serves `index.html` from `www/`)
- `http://localhost:8080/api/status` — Returns server uptime, active threads, and request count in JSON
- `http://localhost:8080/api/info` — Returns server platform information in JSON

## 🧠 Architecture Overview

1. **Accept Loop:** The main thread binds to a TCP socket and enters an infinite `accept()` loop waiting for clients.
2. **Task Queue:** When a client connects, the socket descriptor is safely moved into a thread-safe task queue.
3. **Thread Pool:** Worker threads sleep on a `std::condition_variable`. When a new connection enters the queue, a worker wakes up, dequeues the socket, and handles the HTTP exchange.
4. **Router:** The worker parses the HTTP request string, routes it to the appropriate handler (static files or JSON API), and sends back the HTTP response.
