#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer
import time

hostName = "0.0.0.0"
serverPort = 7999

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/plain")
        self.end_headers()

        self.wfile.write(bytes("666666", "utf-8"))
        self.wfile.flush()
        time.sleep(1)

        self.wfile.write(bytes("333", "utf-8"))
        self.wfile.flush()
        time.sleep(1)

        self.wfile.write(bytes("22", "utf-8"))
        self.wfile.flush()
        time.sleep(1)

        self.wfile.write(bytes("1", "utf-8"))
        self.wfile.flush()

if __name__ == "__main__":
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")