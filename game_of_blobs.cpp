#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "game_of_blobs.h"
#include "ascii_art.h"


// Vector to hold all instances of blobs
//typedef std::vector<blob> vblob;

//int InitGameLoop();
//grid_space minimize_distance(grid_space position_blob1, 
//        const grid_space position_blob2);
//int calc_distance(grid_space position1, grid_space position2);
//vblob InitializeGridRandom(int width, int height, int num_blobs);

// Methods to codify blob behaviors
// Closest_Blob
//int main(int argc, char **argv) {
//    // Test distance calculator
//    grid_space cell1 = {7, 2};
//    grid_space cell2 = {6, -4};
//    int dist = calc_distance(cell1, cell2);
//    std::cout << "The distance between grid (" << std::get<0>(cell1) << ", " <<
//       std::get<1>(cell1) << ") and (" << std::get<0>(cell2) << ", " << std::get<1>(cell2) <<
//      ") is " << dist << "\n"; 
//
//    InitGameLoop();
//    return 0;
//}

int InitGameLoop() {

    // flags:
    // scared_blobs
    // aggressive_blobs

    int width;
    int height;
    int num_blobs = 5;

    // Initialize blobs 
    vblob blobVector;
    blobVector = InitializeGridRandom(BOARD_WIDTH, BOARD_HEIGHT, num_blobs);

    // Initialize game grid
    GameGrid blobGameGrid;
    blobGameGrid.initializeGrid();
    blobGameGrid.currentState = &blobVector;
    blobGameGrid.paintInitialGrid();

    // Print out the state of the beginning grid

    // Initialize game
    blob currentBlob;
    int currBlobIter = 0;

    int testIter = 0;
    bool thereCanOnlyBeOne = false;
    while(!thereCanOnlyBeOne) {

            // Let's see if we've caught the other blob...
            if (currentBlob.position == foodBlob.position) {

                if (currentBlob.mass > foodBlob.mass) {
                    //Consume food blob and add its mass to the current blob
                    currentBlob.mass += foodBlob.mass;

                    // Flip consumed blob's alive bool
                    currentBlob.is_alive = false;
                }

                else if (currentBlob.mass == foodBlob.mass) {
                    // 50% chance for either blob to emerge victorious
                    int coinFlip = std::rand() % 2;
                    // if coinFlip is 0, the current blob consumes the
                    // target blob
                    if (coinFlip == 0) {
                        currentBlob.mass += foodBlob.mass;

                        // Flip is_alive flag for consumed blob
                        foodBlob.is_alive = false;

                    }
                    // Otherwise, in a twist of fate, the target blob consumes
                    // the current blob
                    else {
                        foodBlob.mass += currentBlob.mass;

                        // Flip is_alive flag for consumed blob
                        currentBlob.is_alive = false;
                        break;
                    }
                }

                // If the target blob has a larger mass than the current blob
                // This situation should not occur...but should still be guarded against
                else {
                    foodBlob.mass += currentBlob.mass;

                    // Flip is_alive flag for consumed blob
                    currentBlob.is_alive = false;
                    // End movement loop (a consumed blob can't make additional
                    // moves!)
                    break;
                }
            }

            // Check to see if the game has been won (i.e., there is only one blob left)
            if (blobVector.size() == 1) {
                thereCanOnlyBeOne = true;
            }
            // If the game has not been won, it's the next blob's move
            else {
                // Add one to currBlobIter
                currBlobIter += 1;
                // Reinitialize currBlobIter as zero if we are at the end of the
                // blob vector
                if (currBlobIter == blobVector.size()) {
                    currBlobIter = 0;
                }
            }

            // Draw current state of board
            
        }

        // FOR TESTING PURPOSES ONLY
        testIter += 1;
        std::cout << "on iteration: " << testIter << "\n";
        if (testIter == 1000) {
            std::cout << "Game timed out. \n";
            return 1;
        }
    }
    std::cout << "Game won. \n";
    return 0;
}

void proceed_to_nearest_smaller_blob(vblob &blobVector, int *current_blob_index) {

        // Retrieve the blob whose turn it is
        blob currentBlob = blobVector[*current_blob_index];

        // Do calculations for every individual move for 
        // blob
        for (int j = 0; j < currentBlob.speed; j++) {

            // If the blob is dead, move onto next blob
            if (!currentBlob.is_alive) {
                continue;
            }

            // Find the distance of all other blobs and determine the
            // best target for the current blob (i.e., the closest blob
            // that is smaller)
            int foodBlobIter;
            blob foodBlob; 
            int min_dist;
            for (int i = 0; i < blobVector.size(); i++) {
//                std::cout << "looking at the other blobs loop\n";
                // If the blob is dead, pass
                if (!blobVector[i].is_alive) {
                    continue;
                }
                
                // pass if the other blob is larger than the
                // one whose turn it is
                if (blobVector[i].mass > currentBlob.mass) {
                    continue;
                }

                // Pass over blob whose turn it is
                if (i == *current_blob_index) {
                    continue;
                }

                // Calculate distance
                int dist = calc_distance(currentBlob.position, blobVector[i].position);

                // If the dist is less than the one stored previously, replace it. 
                // Otherwise, discard
                if ((i == 0) ||( dist < min_dist)) {
                    min_dist = dist;
                    foodBlob = blobVector[i];
                    foodBlobIter = i;
                }


            }
            // Now that we have the closest smaller blob, analyze all possible
            // movement patterns and choose move that minimizes distance
            currentBlob.position = minimize_distance(currentBlob.position,
                                                     foodBlob.position);
        }
}


grid_space minimize_distance(grid_space position_blob1, const grid_space position_blob2) {
    // Given the position of blob1 and blob2, find grid space for blob1 to move to that 
    // minimizes distance between blob1 and 2
    
    // There are only a limited number of movements available to a blob: 
    // horizontal, vertical, and diagonal. We can treat each instance of
    // the blob moving 1 space as a seperate motion, and break down the
    // move set to minimize distance between blobs that way
    int dist;
    int min_dist;
    grid_space eval_position;
    grid_space position_of_min_distance;
    std::vector<int> ones ={-1, 1};

    // Horizontal movements
    for (int j = 0; j < 2; j++) {
        eval_position = {std::get<0>(position_blob1),
                         std::get<1>(position_blob1) + ones[j]};

        // Check to see if this move would place the blob out of bounds. If
        // it would, we don't need to evaluate it
        if ((std::get<1>(eval_position) > BOARD_WIDTH) || (std::get<1>(eval_position) < BOARD_WIDTH)) {
            continue;
        }

        dist = calc_distance(position_of_min_distance, position_blob2);

        if (j == 0 || dist < min_dist) {
            min_dist = dist;
            position_of_min_distance = eval_position;
        }
    }

    // Vertical movements
    for (int j = 0; j < 2; j++) {
        eval_position = {std::get<0>(position_blob1) + ones[j],
                         std::get<1>(position_blob1)};
        dist = calc_distance(eval_position, position_blob2);

        if (dist < min_dist) {
            min_dist = dist;
            position_of_min_distance = eval_position;
        }
    }

    // Diagonal Movements
    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
            eval_position = {std::get<0>(position_blob1) + ones[j],
                             std::get<1>(position_blob1) + ones[k]};
            dist = calc_distance(eval_position, position_blob2);

            if (dist < min_dist) {
                min_dist = dist;
                position_of_min_distance = eval_position;
            }
        }
    }
    // Assign new position to blob1
    position_blob1 = position_of_min_distance;

    return position_blob1;
}
        
int calc_distance(grid_space position1, grid_space position2) {

    int x1 = std::get<1>(position1);
    int y1 = std::get<0>(position1);

    int x2 = std::get<1>(position2);
    int y2 = std::get<0>(position2);

    int dist;

    // If blobs are in the same row
    if (y1 == y2) {
        dist = std::abs(x1 - x2);
    }

    // If blobs are in the same column
    else if (x1 == x2) {
        dist = std::abs(y1 - y2);
    }

    else {
        // If on diagonal from blob
        if (std::abs(x1 - x2) == std::abs(y1 - y2)) {
            dist = std::abs(x1 - x2);
        }

        // If not on direct diagonal from blob, but also not in same row or
        // column
        else {
            // Find the distance diagonally that would put blobs in same row
            // or column
            int diag_dist = std::min(std::abs(x1 - x2), std::abs(y1 - y2));            
            // Remaining distance wil be a straight line from the end of the 
            // diagonal to the second blob
            // NOTE: one of the absolute values below should be zero
            dist = std::abs(x1 - x2) + std::abs(y1 - y2) - diag_dist;
        }
    }
        
    return dist;
}

// Initialize Grid with random placements of blobs
vblob InitializeGridRandom(int width, int height, int num_blobs) {

    // Initialize instances of blobs and position them in random
    // points of grid space
    int x;
    int y;
    blob initBlob;
    vblob allBlobs;
    for (int i = 0; i < num_blobs; i++) {
        initBlob.mass = 1;
        initBlob.speed = 1;

        bool usedPosition = true;
        while(usedPosition) {
            // Get a random point in grid space
            //
            x = std::rand() % width;
            y = std::rand() % height;
            std::cout << "Generated x: " << x << "\n";
            std::cout << "Generated y: " << y << "\n";

            // Check to ensure that the randomly initialized position
            // isn't already occupied by another blob
            blob checkBlob;
            // If this is the first blob, just add it to the vector
            if (allBlobs.empty()){
                break;
            }
            // else, check to see if the position is already in use
            else {
                for (int i = 0; i < allBlobs.size(); i++) {
                    checkBlob = allBlobs[i];
                    if ((y == std::get<0>(checkBlob.position)) && 
                                (x == std::get<1>(checkBlob.position))) {
                        break;
                    }

                    // If we have iterated over all blobs and have not broken
                    // the for loop, then we know that no other blob has the
                    // initialized position
                    if (i == allBlobs.size() - 1) {
                        usedPosition = false;
                    }
                }
            }
        }
        
        // Assign blob randomly generated position
        initBlob.position = {y, x};

        // Assign blob a unique identifier
        std::string identifier = "Blob" + std::to_string(i);
        initBlob.identifier = identifier;

        // Add blob to vector containing all blobs
        allBlobs.push_back(initBlob);


    }
//    // Initialize grid to display on command line
//    blobGameGrid->currentState = &allBlobs;
//    blobGameGrid->initializeGrid();

    return allBlobs;
}





        



