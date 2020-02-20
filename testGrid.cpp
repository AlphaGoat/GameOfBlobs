#include <iostream>
#include <locale>

#include "ascii_art.h"
#include "game_of_blobs.h"

int main() {
    /* Test drawing capabilities of game grid
     */
    setlocale(LC_CTYPE, "");

    // Initialize randomized blob vectors
    int num_blobs = 4;

    vblob bVector = InitializeGridRandom(BOARD_WIDTH, BOARD_HEIGHT, num_blobs);

    // Determine the byte size of a normal char vs one specified with a utf-8
    // encoding
//    char test_char = u8" f ";
//    char unicode_char = u8"\u2622"

    // Test printing out unicode characters
    std::cout << "Encircled four: " <<  "\u2463" << "\n";
    std::cout << "Black circle one: " << "\u278a" << "\n";

    const char* blackCircleNums[10] = {u8"\u278a", u8"\u278b", u8"\u278c", 
                                       u8"\u278d", u8"\u278e", u8"\u278f", 
                                       u8"\u2781", u8"\u2782", u8"\u2783", 
                                       u8"\u2784"};
    for (int i = 0; i < 10; i++) {
//        std::cout << "element " << i << ": " << blackCircleNums[i] << "\n";
        std::printf("element %i: %s\n", i, blackCircleNums[i]);
    }

    // Initialize game grid
    GameGrid grid;
    grid.initializeGrid();
    grid.currentState = &bVector;
    grid.paintInitialGrid();

    //Draw grid
    grid.drawGrid();

}

