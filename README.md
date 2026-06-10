# C++ HTTP Server

C++ HTTP Server is a small multithreaded web server built from scratch. It is built as a portfolio-oriented backend project demonstrating core concepts: C++17, CMake, raw OS sockets, concurrency, and custom thread pooling.

## Features

- Multithreaded request handling via a custom thread pool.
- HTTP parsing and routing implemented from scratch.
- Static file serving with automatic MIME type detection.
- JSON API endpoints for server status and metrics.
- Cross-platform socket abstraction layer (Winsock2 for Windows, POSIX for Linux/macOS).
- Zero external dependencies.

## Tech Stack

- C++17
- CMake
- Raw OS Sockets

## API

### Status Check
`GET /api/status`
Returns JSON containing server uptime, total requests, and thread statistics.

### Server Info
`GET /api/info`
Returns JSON containing server version, language, and platform details.

### Static Files
`GET /`
Serves the `index.html` developer dashboard and static assets from the `www/` directory.

## Build

```bash
git clone https://github.com/MeedssWW/cpp-http-server.git
cd cpp-http-server
cmake -B build
cmake --build build
```

## Run

```bash
# Default (port 8080, threads = CPU cores)
./build/http_server

# Custom port and threads
./build/http_server -p 3000 -t 8
```
