#include <iostream>
#include <string>
#include <queue>
#include <chrono>
#include <thread>

using namespace std::string_literals;  // For string literal "s"

// Simulated network packet
template<typename PayloadType>
struct Packet {
    uint32_t id;
    std::string source;
    PayloadType payload;
    
    Packet(uint32_t i, std::string src, PayloadType p)
        : id(i), source(std::move(src)), payload(std::move(p)) {}
};

// Packet handler with priority queue
template<typename T>
class PacketHandler {
    std::queue<Packet<T>> packet_queue;
    size_t max_queue_size;
    
public:
    explicit PacketHandler(size_t queue_size) : max_queue_size(queue_size) {}
    
    bool addPacket(const Packet<T>& packet) {
        if (packet_queue.size() >= max_queue_size) {
            return false;
        }
        packet_queue.push(packet);
        return true;
    }
    
    void processPackets() {
        while (!packet_queue.empty()) {
            const auto& packet = packet_queue.front();
            std::cout << "Processing packet ID: " << packet.id 
                     << " from " << packet.source 
                     << " with payload: " << packet.payload << "\n";
            packet_queue.pop();
            
            // Simulate processing time
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    size_t queueSize() const { return packet_queue.size(); }
};

int main() {
    std::cout << "C++17 Style:\n";
    // C++17 style - Template argument deduction
    PacketHandler<std::string> textHandler(5);
    PacketHandler<double> numericHandler(5);
    
    // Create and process text packets
    textHandler.addPacket(Packet(1, "Server1"s, "Hello"s));
    textHandler.addPacket(Packet(2, "Server2"s, "World"s));
    
    // Create and process numeric packets
    numericHandler.addPacket(Packet(3, "Sensor1"s, 42.5));
    numericHandler.addPacket(Packet(4, "Sensor2"s, 37.8));
    
    std::cout << "\nC++11/14 Style:\n";
    // C++11/14 style - Explicit template arguments required
    PacketHandler<std::string> old_textHandler(5);
    PacketHandler<double> old_numericHandler(5);
    
    old_textHandler.addPacket(Packet<std::string>(5, "Server3"s, "Hello"s));
    old_numericHandler.addPacket(Packet<double>(6, "Sensor3"s, 39.1));
    
    std::cout << "\nProcessing text packets:\n";
    textHandler.processPackets();
    
    std::cout << "\nProcessing numeric packets:\n";
    numericHandler.processPackets();
    
    return 0;
}
