#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

// Request structure
struct HTTPRequest
{
    int client_socket;
    std::string request_data;
};

// Thread pool class
class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    void enqueueRequest(HTTPRequest request);
    void start();

private:
    void worker();
    std::vector<std::thread> threads;
    std::queue<HTTPRequest> request_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    std::atomic<bool> stop;
};

// Web Server class
class WebServer
{
public:
    WebServer(const std::string &address, int port, size_t thread_pool_size);
    ~WebServer();
    void start();

private:
    void handleClient(int client_socket);

    std::string address;
    int port;
    int server_socket;
    ThreadPool thread_pool;
};

// ThreadPool constructor
ThreadPool::ThreadPool(size_t num_threads) : stop(false)
{
    threads.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i)
    {
        threads.push_back(std::thread(&ThreadPool::worker, this));
    }
}

// ThreadPool destructor
ThreadPool::~ThreadPool()
{
    stop = true;
    queue_cv.notify_all();
    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

// Add a request to the queue
void ThreadPool::enqueueRequest(HTTPRequest request)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    request_queue.push(request);
    queue_cv.notify_one();
}

// Worker function
void ThreadPool::worker()
{
    while (!stop)
    {
        HTTPRequest request;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [this]
                          { return !request_queue.empty() || stop; });

            if (stop && request_queue.empty())
            {
                return;
            }

            request = request_queue.front();
            request_queue.pop();
        }

        // Process the HTTP request
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from the server!";
        send(request.client_socket, response.c_str(), response.length(), 0);
        close(request.client_socket); // Close the client socket after sending the response
    }
}

// WebServer constructor
WebServer::WebServer(const std::string &address, int port, size_t thread_pool_size)
    : address(address), port(port), thread_pool(thread_pool_size) {}

// WebServer destructor
WebServer::~WebServer()
{
    close(server_socket);
}

// Start the web server
void WebServer::start()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return;
    }

    if (listen(server_socket, 10) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        return;
    }

    std::cout << "Server listening on " << address << ":" << port << std::endl;

    // Start the server loop
    while (true)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0)
        {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        char buffer[1024];
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            HTTPRequest request = {client_socket, std::string(buffer)};
            thread_pool.enqueueRequest(request);
        }
    }
}

int main()
{
    WebServer server("127.0.0.1", 8080, 4); // Address, Port, Thread pool size
    server.start();
    return 0;
}
