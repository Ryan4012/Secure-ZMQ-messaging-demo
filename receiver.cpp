#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include "crypto_utils.hpp"

using json = nlohmann::json;

int main() {

    // Setup REP socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    // Receive message
    zmq::message_t request;
    socket.recv(request);

    // Parse JSON
    std::string data(static_cast<char*>(request.data()), request.size());
    json payload = json::parse(data);

    // Extract fields
    long timestamp = payload["timestamp"];
    std::string message = payload["message"];
    std::string signature = payload["signature"];

    // Rebuild signed string
    std::string signed_data = std::to_string(timestamp) + "|" + message;

    // Verfiy signature
    std::string b64_signature = payload["signature"];

    // Decode base64 back to raw bytes
    std::string raw_signature = base64_decode(b64_signature);

    bool valid = verify_signature(signed_data, raw_signature, "../public.pem");

    // Output results
    if (valid)
        std::cout << "Valid message: " << message << "\n";
    else
        std::cout << "Invalid signature!\n";

    // Reply
    socket.send(zmq::buffer("OK"), zmq::send_flags::none);
}