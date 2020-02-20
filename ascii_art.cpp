// Utilities for drawing out game space for GameOfBlobs
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <wchar.h>

#include "ascii_art.h"

//functions to return 
//const std::string emptyGridSpace = "0xc90xcd0xbb0x0a0xba0x000xba0x0a0xc80xcd0xbc";
//const std::string downArrow = "0x2190";

// hex conversions
//std::string string_to_hex(const std::string& in);
//std::string hex_to_string(const std::string& in);

// Unicode Arrows
const char* left_arrow = u8"\u2190";
const char* up_arrow = u8"\u2191";
const char* right_arrow = u8"\u2191";
const char* down_arrow = u8"\u2193";
const char* northwest_arrow = u8"\u2196";
const char* northeast_arrow = u8"\u2197";
const char* southeast_arrow = u8"\u2198";
const char* southwest_arrow = u8"\u2199";

// unicode symbols
const char* rad_symbol = u8"\u2622";
const char* plus_sign = u8"\u2795";
const char* horizontal_bar = u8"\u2015";
const char* vertical_bar = u8"\ufe31";
const char* white_space = u8"\u0020";
const char* line_feed = u8"\u000a";

const char* whiteCircleNums[10] = {u8"\u2460", u8"\u2461", u8"\u2462", 
                                   u8"\u2463", u8"\u2464", u8"\u2465", 
                                   u8"\u2466", u8"\u2467", u8"\u2468", 
                                   u8"\u2469"};

const char* blackCircleNums[10] = {u8"\u278a", u8"\u278b", u8"\u278c", 
                                   u8"\u278d", u8"\u278e", u8"\u278f", 
                                   u8"\u2781", u8"\u2782", u8"\u2783", 
                                   u8"\u2784"};


//int main(){
//
////    std::string hexString;
//    std::string convString;
////    std::string s = "Hello World!";
////    std::cout << "original string: " << s << '\n';
////    hexString = string_to_hex(s);
////    std::cout << "hex format: " << hexString << '\n';
//    //convString = hex_to_string(downArrow);
//    //std::cout << "back to string: " << convString << '\n';
//
//    // Unicode testing
//    std::cout << "left arrow: " << left_arrow << '\n';
//    std::cout << "up arrow: " << up_arrow << '\n';
//    std::cout << "right arrow: " << right_arrow << '\n';
//    std::cout << "down arrow: " << down_arrow << '\n';
//
//}


void GameGrid::initializeGrid() {

    // Initialize character array to hold grid
    // Notice that it is initialized with 3x values
    // than height and width. This is to include the characters
    // representing connectors between different nodes of the graph
    //Note: additional column is to contain newline characters
    std::cout << "compiling grid of x_dim: " << x_dim << " and y_dim: " << y_dim << "\n";

    for (int i = 0; i < y_dim; i++) {
        for (int j = 0; j < x_dim; j++) {
            // j is 0, i.e. the first column of the row, add a newline
            // character  
            if (j == 0) {
                initialGrid[i][0] = line_feed;
            }

            // Draw nodes of graph
            else if ((i % 4 == 0) && (j % 4 == 1)){
                initialGrid[i][j] = plus_sign;
            }

            // Draw horizontal node connectors
            else if ((i % 4 == 0) && (j % 4 != 1)){
                initialGrid[i][j] = horizontal_bar;
            }

            // Draw vertical node connectors
            else if ((i % 4 != 0) && (j % 4 == 1)){
                initialGrid[i][j] = vertical_bar;
            }

            // If none of the above, place an empty space in array
            else {
                initialGrid[i][j] = white_space;
            }

        }
    }

}

void GameGrid::paintInitialGrid(){
    /*
     * Draws state of game grid at the beginning of the game, with
     * randomly initialized start positions
     */
    currentGrid = initialGrid;

    for (int i = 0; i < currentState->size(); i++) {

        // Paint positions of blobs on board
        std::tuple <int, int> curr_position = (*currentState)[i].position;
        int x = (4 * std::get<1>(curr_position)) + 1;
        int y = (4 * std::get<0>(curr_position));
        int mass = (*currentState)[i].mass;

        std::cout << "printing blob of mass: " << mass << " to grid.\n";

        currentGrid[y][x] = blackCircleNums[mass - 1];
    }
}

void GameGrid::drawGrid() {
    /*
     * Print out game grid to command line
     */
    int r;
    //setlocale(LC_CTYPE, "");
    std::locale::global(std::locale("en_US.utf8"));
    std::cout.imbue(std::locale()); 
    for (int y = 0; y < currentGrid.size(); y++) {
       for (int x = 0; x < currentGrid[0].size(); x++) {
//           std::cout << currentGrid[y][x];
           std::cout << currentGrid[y][x];            
       }
    }
    std::printf("\n");
} 



void GameGrid::updateGrid(){
    /* 
     * Function to depict the grid after a blob finishes its move
     */

    // :param blob_index: int of position of blob making move in blob vector
    
    // Grab previous and current positions of blob
//    prev_position = (*previousState)[blob_index].position;
//    curr_position = (*currentState)[blob_index].position;

    // Grab previous and current masses of blob
//    prev_mass = (*previousState)[blob_index].mass;
//    curr_mass = (*previousState)[blob_index].mass;

    // Reinitialize grid to its initial state
    currentGrid = initialGrid;

    // Plot out moves
    for (int i = 0; i < currentState->size(); i++) {
        // If the blob is dead, move on
        if (!(*currentState)[i].is_alive) {
            continue;
        }

        // Paint previous position of blob
        std::tuple <int,int> prev_position = previousState[i].position;
        std::tuple <int, int> curr_position = (*currentState)[i].position;

        int prev_x = (4 * std::get<1>(prev_position)) + 1;
        int prev_y = (4 * std::get<0>(prev_position));

        int curr_x = (4 * std::get<1>(curr_position)) + 1;
        int curr_y = (4 * std::get<0>(curr_position));

        // Masses of the blob
        int prev_mass = previousState[i].mass;
        int curr_mass = (*currentState)[i].mass;

        // Paint previous position
        currentGrid[prev_y][prev_x] = whiteCircleNums[prev_mass - 1];

        // Paint current position
        currentGrid[curr_y][curr_x] = blackCircleNums[prev_mass - 1];;

        // Draw arrows pointing direction of movement for blobs
        // movement right:
        if ((prev_y == curr_y) && (prev_x < curr_x)) {
            for (int i = 1; i <= 3 ; i++) {
                currentGrid[curr_y][curr_x - i] = *right_arrow;
            }
        }

        // movement left:
        else if ((prev_y == curr_y) && (prev_x > curr_x)) {
            for (int i = 1; i <=3; i++) {
                currentGrid[curr_y][curr_x + i] = *left_arrow;
            }
        }

        // Movement up:
        else if ((prev_y < curr_y) && (prev_x == curr_x)) {
            for (int i = 1; i<=3; i++) {
                currentGrid[curr_y - i][curr_x] = *up_arrow;
            }
        }

        // Movement down:
        else if ((prev_y > curr_y) && (prev_x == curr_x)) {
            for (int i = 1; i<=3; i++) {
                currentGrid[curr_y + i][curr_x] = *down_arrow;
            }
        }

        // Diagonal north east
        else if ((prev_y < curr_y) && (prev_x < curr_x)) {
            for (int i = 1; i <= 3 ; i++) {
                currentGrid[curr_y - i][curr_x - i] = *northeast_arrow;
            }
        }

        // Diagonal north west
        else if ((prev_y < curr_y) && (prev_x > curr_x)) {
            for (int i = 1; i <= 3 ; i++) {
                currentGrid[curr_y - i][curr_x + i] = *northwest_arrow;
            }
        }

        // Diagonal south east
        else if ((prev_y > curr_y) && (prev_x < curr_x)) {
            for (int i = 1; i <= 3 ; i++) {
                currentGrid[curr_y + i][curr_x - i] = *southeast_arrow;
            }
        }

        else if ((prev_y > curr_y) && (prev_x > curr_x)) {
            for (int i = 1; i <= 3 ; i++) {
                currentGrid[curr_y + i][curr_x + i] = *southwest_arrow;
            }
        }
    }
}


//
//std::string OccupiedGridSpace() {
//}
//
//std::string PreviouslyOccupedGridSpace() {
//}
//
//std::string Edge() {
//}
//
//std::string MovementEdge() {
//}

// Following functions sourced from X-Istence on stackoverflow
// https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
std::string string_to_hex(const std::string& in) {
    std::stringstream ss;

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; in.length() > i; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
    }

    return ss.str();
}

std::string hex_to_string(const std::string& in) {
    std::string output;

    if ((in.length() % 2) != 0) {
        throw std::runtime_error("String is not valid length...");
    }

    size_t cnt = in.length() / 2;

    for (size_t i = 0; cnt > i; ++i) {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << in.substr(i * 2, 2);
        ss >> s;

        output.push_back(static_cast<unsigned char>(s));
    }

    return output;
}

//const char* unicode_to_utf8(wchar_t c) {
//    // NOT thread safe, not re-entrant
//    static unsigned char b_static[5];
//    unsigned char* b = b_static;
//
//    if (c<(1<<7)) {// 7 bit unicode encoded as plain ascii 
//        *b++ = (unsigned char) (c);
//    }
//
//    else if (c<(1<<11)) { //11 bit unicode encoded in 2 utf-8 bytes
//        *b++ = (unsigned char)(((c>>12))|0xE0);
//        *b++ = (unsigned char)(((c>>6)&0x3F)|0x80);
//        *b++ = (unsigned char)((c&0x3F)|0x80);
//    }
//
//    else if (c<(1<<21)) { // 21 bit unicode encoded in 4 utf-8 bytes
//        *b++ = (unsigned char)(((c>>18))|0xF0);
//        *b++ = (unsigned char)(((c>>12)&0x3F)|0x80);
//        *b++ = (unsigned char)(((c>>6)&0x3F)|0x80);
//        *b++ = (unsigned char)((c&0x3F)|0x80);
//    }
//    *b = '\0';
//    return b_static;
//}




