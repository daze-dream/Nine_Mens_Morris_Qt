#ifndef MANUALTESTS_H
#define MANUALTESTS_H

#include "Player.h"
#include "Board.h"
#include "BoardLocation.h"
#include "Enum.h"
#include <iostream>

using namespace std;

class manualTests
{
public:
    manualTests();
    //Time to fly - unit test
    //Given an ongoing game in the second phase, when a player is reduced to three pieces, then the text “Player X has entered Flight Mode!” is displayed.
    void timeToFlyUnitTest();

    //Capturing Pieces Unit test.
    //Given an ongoing game in the first phase, when a player's piece is captured, then their active pieces in phase 1 decrements by 1.
    //Given an ongoing game in the second phase, when a player's piece is captured, then their active pieces in phase 2 decrements by 1.
    void subtractPieceUnitTest();

    //Forming mill test
    /*Given an ongoing game in the first phase, when a player makes a mill
     by forming 3 pieces on a connecting line, then the textbox should
     display “Player X has made a mill. Select a piece to capture.”
    */
    void formMillTest();
    //Valid/Invalid move test
    //Given an ongoing game in the first phase, when a player makes a legal move (empty spot), the piece is drawn on that spot
    //Given an ongoing game in the first phase, when a player makes an illegal move
    //(non-empty spot), the piece is returned to their stack of pieces and they can redo their turn
    void moveValidityTest();

    //Phase 2 movement test
    void FlightMovementTest();

    //does alll the current available tests.
    void doAllTests();



};

#endif // MANUALTESTS_H
