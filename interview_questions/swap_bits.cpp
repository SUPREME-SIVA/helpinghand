#include <iostream>
#include <cstdint>
#include <iomanip>

// Function to swap 1st and 4th bits of a number
uint32_t swapBits(uint32_t num) {
    // Create masks for 1st and 4th bits
    constexpr uint32_t bit1_mask = 0x01;      // 00000001
    constexpr uint32_t bit4_mask = 0x08;      // 00001000

    // Check if 1st bit is set
    bool bit1 = (num & bit1_mask) != 0;
    
    // Check if 4th bit is set
    bool bit4 = (num & bit4_mask) != 0;

    // If both bits are same, no need to swap
    if (bit1 == bit4) {
        return num;
    }

    // Toggle both bits
    num ^= bit1_mask;
    num ^= bit4_mask;

    return num;
}

int main() {
    uint32_t num = 0x12345678;  // Example number
    std::cout << "Original number: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << num << std::endl;
    
    uint32_t result = swapBits(num);
    std::cout << "After swapping 1st and 4th bits: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result << std::endl;
    
    return 0;
}
