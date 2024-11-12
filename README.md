Here's the full `README.md` content in a format you can directly copy and paste into your `README.md` file:

```markdown
# Web Server Project

This is a simple multi-threaded web server written in C. It uses **pthreads** for handling multiple client connections concurrently, supports various HTTP methods (GET, POST, PUT, DELETE), serves static files, and includes logging and basic authentication mechanisms.

## Features

- **Serving Static Files**: Serve HTML, CSS, JS, and image files to clients.
- **HTTP Method Support**: Handles GET, POST, PUT, and DELETE methods.
- **Request Logging**: Logs incoming requests to a file for debugging and monitoring purposes.
- **Content Type Handling**: Responds with appropriate content types based on requested resources.
- **Error Handling**: Implements error handling for HTTP errors like 404 (Not Found) and 500 (Internal Server Error).
- **Thread Pooling**: Uses a thread pool to efficiently manage client connections and optimize resource usage.
- **Server Configuration**: Configurable through a configuration file (e.g., setting the server's port and document root).
- **Basic Authentication**: Implements a simple username/password authentication mechanism for secure access.

## Directory Structure

```
web-server/
├── src/                  # Source code for the server
│   ├── main.c
│   ├── request_handler.c
│   ├── thread_pool.c
│   ├── logger.c
│   ├── server.c
│   └── config.c
├── obj/                  # Object files (created during compilation)
├── bin/                  # Compiled server binary
├── www/                  # Web content (HTML, CSS, JS, images)
│   ├── index.html
│   ├── about.html
│   ├── styles/
│   ├── scripts/
│   └── images/
├── config/               # Configuration files
│   └── server.conf
├── Makefile              # Build script
├── README.md             # Project documentation
└── logs/                 # Server log files
```

## Prerequisites

- **C Compiler**: A C compiler like `gcc`.
- **Pthreads**: The server uses POSIX threads, so ensure that your system supports it.
- **Make**: A tool to build the project.

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/web-server.git
   cd web-server
   ```

2. **Build the Project**:
   Run the following command to compile the server:
   ```bash
   make
   ```

3. **Configuration**:
   Edit the `config/server.conf` file to configure the server, such as setting the port number and document root (the `www/` directory). Here's an example configuration:

   ```ini
   port = 8080
   document_root = ./www
   log_file = ./logs/server.log
   ```

4. **Run the Server**:
   To start the web server:
   ```bash
   make run
   ```
   This will start the server with the configuration specified in `config/server.conf`.

## Usage

- The server listens on the port specified in the configuration file (`server.conf`).
- It serves static files (HTML, CSS, JS, images) from the `www/` directory.
- To make requests, you can use a web browser or a tool like `curl`.

  Example GET request:
  ```bash
  curl http://localhost:8080/index.html
  ```

  Example POST request (for example, submitting a form or data):
  ```bash
  curl -X POST http://localhost:8080/submit -d "name=John&message=Hello"
  ```

## Logging

The server logs all incoming requests to a log file, which is defined in the `server.conf` configuration file. The log entries are useful for debugging and monitoring.

Example log entry:
```
[INFO] 127.0.0.1 - - [2024-11-12 12:34:56] "GET /index.html HTTP/1.1" 200
```

## Error Handling

The server returns standard HTTP error codes when something goes wrong:

- **404 Not Found**: If the requested file does not exist.
- **500 Internal Server Error**: If something goes wrong on the server-side (e.g., a misconfiguration or an unexpected error).

You can customize these error pages by adding `404.html` and `500.html` to the `www/` directory.

## Thread Pooling

The server uses a **thread pool** to manage concurrent client connections efficiently. A limited number of threads are created, and incoming requests are assigned to available threads in the pool.

## Basic Authentication

The server supports basic authentication. Add the following to a protected resource URL:

```html
Authorization: Basic <base64-encoded username:password>
```

For example, to access a protected resource, you can pass the credentials using `curl` like this:

```bash
curl -u username:password http://localhost:8080/protected.html
```

## Clean Up

To clean up the project (remove object files and the compiled binary), use:

```bash
make clean
```

To completely rebuild the project, use:

```bash
make rebuild
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
