#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include "crypto_utils.hpp"

using json = nlohmann::json;

int main() {

    // Context = ZeroMQ runtime
    //REQ socket = request sender
    // Connects to receiver
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    // Builds Timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // Message contents
    std::string message_text = "Hello from C++, This is a test message!";
    std::string to_sign = std::to_string(timestamp) + "|" + message_text;

    // Sign message
    std::string signature = sign_message(to_sign, "../private.pem");

    // Encode raw signature to base64 string
    std::string b64_signature = base64_encode(
        reinterpret_cast<const unsigned char*>(signature.data()), signature.size()
    );

    // Build JSON payload
    json payload = {
        {"timestamp", timestamp},
        {"message", message_text},
        {"signature", b64_signature}
    };


    // Send JSON
    std::string serialized = payload.dump();
    zmq::message_t msg(serialized.size());
    memcpy(msg.data(), serialized.data(), serialized.size());
    socket.send(msg, zmq::send_flags::none);

    // Receive acknowledgment
    zmq::message_t reply;
    socket.recv(reply);
    std::cout << "Receiver replied\n";
}