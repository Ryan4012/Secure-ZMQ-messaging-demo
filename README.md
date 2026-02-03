# Secure-ZMQ-messaging-demo

## Images
<p align="center"> 
  Initial Command in Terminal #1 <br>
  <img src="./images/Secure ZMQ Messaging Demo 01.png" alt="" width="500" />
  <br>
  Initial Command in Terminal #2 <br>
  <img src="./images/Secure ZMQ Messaging Demo 02.png" alt="" width="500" />
  <br>
  Successful Ouptut in Terminal #1 <br>
 <img src="./images/Secure ZMQ Messaging Demo 03.png" alt="" width="500" />
  <br>
</p>

## Overview
This project demonstrates secure message exchange over ZeroMQ (ZMQ) sockets using RSA digital signatures for message authenticity. A sender signs messages with a private key, encodes the signature in base64, and transmits JSON payloads. The receiver verifies message signatures using the corresponding public key, ensuring secure communication over ZMQ sockets.

## Motivation & Goals
- Learn how to combine cryptographic signatures with messaging middleware
- Understand ZeroMQ socket programming patterns (REQ/REP) in C++
- Implement secure message validation to prevent tampering or spoofing
- Explore base64 encoding/decoding of binary signature data for JSON transport

## Technology Stack
- C++17
- ZeroMQ (libzmq)
- OpenSSL
- CMake build system
- nlohmann/json for JSON handling

## Architecture Overview
- Sender (sender.cpp) signs messages, packages them into JSON with timestamps and signature
- Receiver (receiver.cpp) unpacks JSON, verifies signature, and confirms message integrity
- Uses ZeroMQ REQ (request) and REP (reply) socket types for communication
- Cryptographic functions implemented in shared crypto_utils.cpp/.hpp
- Base64 encoding/decoding used to safely transmit signatures in JSON format

## Services & Functionality
| Component    | Purpose                            | Deployment        |
|--------------|----------------------------------|-------------------|
| Sender       | Signs and sends JSON messages     | Executable        |
| Receiver     | Receives and verifies messages    | Executable        |
| Crypto Utils | Signing, verification, base64 ops | Shared library     |


## Security Considerations
- Uses RSA 2048 digital signatures with SHA-256 digest
- Base64 encoding ensures signature integrity during JSON serialization
- Verification rejects messages with invalid or tampered signatures
- Keys securely loaded from PEM files with proper error handling
- Time-stamped messages allow for potential replay protection extensions

## Setup & Deployment
- Install dependencies: OpenSSL, ZeroMQ, and nlohmann/json
- Build with CMake
- Provide correct paths to private/public PEM keys in code
- Run receiver executable first, then sender to test communication

## What I Learned
- ZeroMQ socket patterns and message flow
- Combining cryptography with messaging systems
- JSON serialization/deserialization in C++
- Base64 encoding and decoding of binary data
- Writing modular, reusable cryptographic utilities

## Future Improvements & Roadmap
- Add replay attack protection using timestamps
- Implement asynchronous message handling with ZMQ
- Add support for encrypted messaging alongside signing
- Improve error reporting and logging
- Package crypto utilities as a standalone library



