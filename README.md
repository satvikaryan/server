# Web Server Project

## Overview
This project is a multi-threaded web server implemented in C with support for HTTP methods (GET, POST, etc.), basic authentication, and error handling.

## Project Structure
- `config/server.conf`: Server configuration (port, document root).
- `include/server.h`: Header file with definitions and function prototypes.
- `src/`: Source code files.
- `www/`: Document root for static files (HTML, CSS, JS).
- `logs/access.log`: Log file for incoming requests.

## Getting Started
1. Run `make` to build the server.
2. Run `./web_server` to start the server.

## Requirements
- GCC compiler
- POSIX threads

## Usage
- Configure the server in `config/server.conf`.
- Place static files in `www/` directory to serve.

