#include "libs/mongoose.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Log
void log_event(const char *msg) {
    // Check for null pointer
    if (msg == NULL) {
        msg = "NULL_MESSAGE";
    }

    // Construct time string
    time_t now = time(NULL);
    char buf[30] = {0};
    size_t bytes = strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Check for strftime failure (exceeding size)
    if (bytes == 0) {
        printf("[ERROR] %s\n", msg);
    } else {
        printf("[%s] %s\n", buf, msg);
    }
    fflush(stdout);
}

void log_start() {
    // Start time log
    time_t now = time(NULL);
    char buf[40] = {0};
    size_t bytes = strftime(buf, sizeof(buf), "Start time: %Y-%m-%d %H:%M:%S", localtime(&now));
    if (bytes != 0) {
        log_event(buf);
    }

    // Port log
    // Get outsite port from env
    const char *port = getenv("DISPLAY_PORT");
    if (port == NULL) {
        log_event("Server started on port 8000");
    } else {
        char msg[37];
        snprintf(msg, sizeof(msg), "Server started on outside port %s", port);
        log_event(msg);
    }

    // Author info
    log_event("Author: Szymon Kowalik");
}

// Check HTTP requests
static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_match(hm->uri, mg_str("/api/locations"), NULL)) {
            // JSON
            log_event("GET /api/locations");
            struct mg_http_serve_opts opts = {.root_dir = "static"};
            mg_http_serve_file(c, hm, "static/locations.json", &opts);
        } else if (mg_match(hm->uri, mg_str("/"), NULL)) {
            // HTML
            log_event("GET /");
            struct mg_http_serve_opts opts = {.root_dir = "static"};
            mg_http_serve_file(c, hm, "static/index.html", &opts);
        } else {
            // OTHER
            log_event("404 Not Found");
            mg_http_reply(c, 404, "Content-Type: text/plain\r\n", "404 Not Found\n");
        }
    }
}

int healthcheck(int port) {
    int sock;
    struct sockaddr_in server;

    // Create socker for testing connection to server
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 1; // Cannot create socket
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Try to connect to our own server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        close(sock);
        return 1; // Connection failed
    }

    close(sock);
    return 0; // Connection succeeded
}

int main(int argc, char *argv[]) {
    // Check if healthcheck is executed
    if (argc == 2 && strcmp(argv[1], "--check") == 0) {
        return healthcheck(8000);
    }

    // Run server
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    log_start();
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}