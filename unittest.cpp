
#include "catch.hpp"

#include "Player.h"
#include "Board.h"
#include "BoardLocation.h"
#include "Enum.h"

TEST_CASE("Player Class Tests","[player]")
{
    Board testBoard;
    Player p1(1, false);
    Player p2(2, false);
    testBoard.addPlayers(p1);
    testBoard.addPlayers(p2);
    testBoard.setCurrPlayerVar(1);
    int asciiOffset = 97;
    Player *p = &testBoard.getCurrentPlayer();

    SECTION("Initalization of player")
    {
        //Player should have 9 Phase 1 pieces, and 0 Phase 2 pieces
        REQUIRE(p->getPhase1Pieces() == 9);
        REQUIRE(p->getPhase2Pieces() == 0);
    }

    SECTION("Player's pieces after phase 1 ")
    {
        //after a Phase 1 move, the player's phase1 pieces decrement, and their phase2 move increments
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        REQUIRE(p->getPhase1Pieces() == 8);
        REQUIRE(p->getPhase2Pieces() == 1);
    }
    SECTION("Player's pieces after mill-->removal")
    {
        //after a piece is removed after a mill, the adversary has their phase 2 pieces decremented.
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());


        testBoard.togglePlayer();
        testBoard.setState(mill);
        testBoard.removePiece('a',7,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        REQUIRE(p->getPhase2Pieces() == 2);
    }

    SECTION("Player is in flight mode after 3 or less Phase 2 pieces")
    {
        //after a piece is removed after a mill, check for flight
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('d',1,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.setState(mill);
        testBoard.removePiece('d',1,testBoard.getCurrentPlayer());
        testBoard.setState(phase2);
        p->timetoFly();
        //player should be in flight mode after they are down to 3 or less pieces
        REQUIRE(p->getFlight() == true);
    }

    SECTION("Player's mill stats after a mill")
    {
        //set up
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.doMovePhase1('g',4,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        //after a mill is made, active and stable mills are updated
        REQUIRE(p->getActiveMills() == 1);
        REQUIRE(p->getStableMills() == 1);
        //after removing a piece, active mills go down, stable mills remain the same
        testBoard.removePiece('g',4,testBoard.getCurrentPlayer());
        REQUIRE(p->getActiveMills() == 0);
        REQUIRE(p->getStableMills() == 1);

    }


}


TEST_CASE("Board Class Tests", "[board]")
{
    Board testBoard;
    Player p1(1, false);
    Player p2(2, false);
    testBoard.addPlayers(p1);
    testBoard.addPlayers(p2);
    testBoard.setCurrPlayerVar(1);
    int asciiOffset = 97;

    SECTION("Toggling Players Only Happens After Valid Move, MANUALLY")
    {
        //The game starts with "Player 1"
        REQUIRE(testBoard.getCurrentPlayer().getPlayerID() == 1);
        //toggling player works
        testBoard.togglePlayer();
        REQUIRE(testBoard.getCurrentPlayer().getPlayerID() == 2);
    }

    SECTION("Phase 1 Placing Pieces")
    {
        //board is automatically in phase1 when created
        REQUIRE(testBoard.getState() == phase1);
        // now manually set it just to be sure
        testBoard.setState(phase1);
        //legal move
        REQUIRE(testBoard.isValidLocation('a',7,0,0,testBoard.getCurrentPlayer()) == true);
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        //player's phase 1 pieces are subtracted, and their phase 2 pieces are increased
        REQUIRE(testBoard.getCurrentPlayer().getPhase1Pieces() == 8);
        REQUIRE(testBoard.getCurrentPlayer().getPhase2Pieces() == 1);
        //the position now has the current player's ID.
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].getStatus() == testBoard.getCurrentPlayer().getPlayerID());
        //illegal move: spot does not exist
        REQUIRE(testBoard.isValidLocation('a',6,0,0,testBoard.getCurrentPlayer()) == false);
        //illegal move: spot is occupied
        testBoard.togglePlayer();
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        REQUIRE(testBoard.isValidLocation('a','4',0,0,testBoard.getCurrentPlayer()) == false);
        //after illegal move, their pieces are still the same
        REQUIRE(testBoard.getCurrentPlayer().getPhase1Pieces() == 8);
        REQUIRE(testBoard.getCurrentPlayer().getPhase2Pieces() == 1);
        testBoard.togglePlayer(); // switch back to player 1
    }

    SECTION("General Mills Made Check")
    {
        //the player updates their mill count
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        REQUIRE(testBoard.getCurrentPlayer().getStableMills() > 0);
        //the board recognizes a mill has been made
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        REQUIRE(testBoard.getState() == mill);
    }

    SECTION("Phase 2 General Move Selection")
    {
        //set up variables
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.doMovePhase1('b',4,testBoard.getCurrentPlayer());
        testBoard.setState(phase2);
        testBoard.togglePlayer();

        //sanity checks are valid
        REQUIRE(testBoard.phase2_1Check('a', 7)==true);
        REQUIRE(testBoard.phase2_2Check('a', 4)==true);

        //valid move
        REQUIRE(testBoard.isValidLocation('a',4,'a',7,testBoard.getCurrentPlayer())==true);
        //invalid move: location selected is empty
        REQUIRE(testBoard.phase2_1Check('a', 1)==false);
        //invalid move: location sanity check is occupied
        REQUIRE(testBoard.phase2_2Check('b', 4)==false);


        //invalid move: goal does not exist
        REQUIRE(testBoard.isValidLocation('a',6,'a',4,testBoard.getCurrentPlayer())==false);
        //invalid move: goal is already occupied
        REQUIRE(testBoard.isValidLocation('b',4,'a',4,testBoard.getCurrentPlayer())==false);
        //invalid move: goal is not adjacent
        REQUIRE(testBoard.isValidLocation('d',1,'a',4,testBoard.getCurrentPlayer())==false);

    }

    SECTION("Phase 2 Mill Detection")
    {
        //set up variables
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('d',1,testBoard.getCurrentPlayer());
        testBoard.setState(phase2);
        testBoard.doMovePhase2('d',1,'a',1,testBoard.getCurrentPlayer());
        //current player has mill, both active and stable
        REQUIRE(testBoard.getCurrentPlayer().getActiveMills() > 0);
        REQUIRE(testBoard.getCurrentPlayer().getStableMills() > 0);
        //and the board is aware as well
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        REQUIRE(testBoard.getState() == mill);
    }

    SECTION("Phase 2 / General Flight Checks")
    {
        //setup
        testBoard.setState(phase2);
        testBoard.getCurrentPlayer().setPhase2Pieces(3);
        testBoard.getCurrentPlayer().timetoFly();
        //Flight is activated when player is down to 3 or less pieces
        REQUIRE(testBoard.getCurrentPlayer().getFlight()==true);
    }

    SECTION("Phase 2 Flight Move Checks")
    {

        //setup. Player has 3 pieces, meaning the qualify for flight immediately entering Phase 2
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('d',1,testBoard.getCurrentPlayer());
        //The function to check each player's state is called at the end of Phase 1, via checkandchangestate.
        //   For test purposes it doesn't need to happen, as this will demonstrate the flight check
        testBoard.setState(phase2);
        testBoard.getCurrentPlayer().timetoFly();
        REQUIRE(testBoard.getCurrentPlayer().getFlight()==true);
        //if a player is in flight mode, they can move anywhere
        REQUIRE(testBoard.isValidLocation('d',7,'d',1,testBoard.getCurrentPlayer())==true);


    }

    SECTION("Phase Mill Removal Checks: No Mills")
    {
        //setup: we know this works from the unit test above
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.doMovePhase1('g',7,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        //sanity check
        REQUIRE(testBoard.getState() == mill);
        //Invalid: spot is empty
        REQUIRE(testBoard.removePiece('b',4,testBoard.getCurrentPlayer()) == false);
        //valid removal
        REQUIRE(testBoard.removePiece('g',7,testBoard.getCurrentPlayer()) == true);
        //check that the position you removed is now "empty"
        REQUIRE(testBoard.getPositions()['g'-asciiOffset][7-1].getStatus() == 0);
        //check that the player's active mill count goes down
        REQUIRE(testBoard.getCurrentPlayer().getActiveMills() == 0);
        //but their stable mill count is still up
        REQUIRE(testBoard.getCurrentPlayer().getStableMills() == 1);
        //the phase returns to the original phase
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        REQUIRE(testBoard.getState()==phase1);
        //and the enemy player's pieces go down
        REQUIRE(testBoard.getPlayers().at(testBoard.getCurrentPlayer().getEnemy() -1).getPhase2Pieces() == 0);
    }

    SECTION("Phase Mill Removal Checks: Mills Active, No Other Moves Possible, Only one mill")
    {

        //set up the variables
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        testBoard.getCurrentPlayer().setActiveMills(0);
        testBoard.getCurrentPlayer().setStableMills(0);
        testBoard.togglePlayer();
        testBoard.doMovePhase1('g',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('g',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('g',1,testBoard.getCurrentPlayer());
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        //sanity check
        REQUIRE(testBoard.getState() == mill);
        //removing a piece in a mill is valid when there's no other piece to remove
        REQUIRE(testBoard.removePiece('a',4,testBoard.getCurrentPlayer()) == true);
    }

    SECTION("Phase Mill Removal Checks: Mills active, One move possible, All others in Mills")
    {
        //this is gonna fail

    }




    SECTION("Game Over Checks")
    {
        testBoard.getCurrentPlayer().setPhase2Pieces(2);
        testBoard.setState(phase2);
        //game ends when either player has less than 3 pieces
        testBoard.checkAndChangeState(testBoard.getPlayers().at(0),testBoard.getPlayers().at(1) );
        REQUIRE(testBoard.getState() == gameOver);

    }



}

TEST_CASE("Board Position Class Tests", "[boardLocation")
{
    Board testBoard;
    Player p1(1, false);
    Player p2(2, false);
    testBoard.addPlayers(p1);
    testBoard.addPlayers(p2);
    testBoard.setCurrPlayerVar(1);
    int asciiOffset = 97;
    Player *p = &testBoard.getCurrentPlayer();


    SECTION("Positions' validity on initialization")
    {
        //valid position
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].isItValid() == true);
        //valid position is also set to display 0 and have status 0
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].getStatus()== 0);
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].getDisp() == '0');
        //invalid position: empty
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][6-1].isItValid() == false);
        //middle position on bard is terminating spot, used for searching
        REQUIRE(testBoard.getPositions()['d'-asciiOffset][4-1].getTerminator() == true);
    }

    SECTION("Status after Phase 1 and 2 moves")
    {
        //after a move, the status of a position is the same as the current player
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].getStatus()== 1);
        testBoard.setState(phase2);
        testBoard.doMovePhase2('a',7,'a',4,testBoard.getCurrentPlayer());
        //phase 2 check
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][4-1].getStatus()== 1);


    }

    SECTION("Status after Mill Removal")
    {
        //after a piece is removed after a mill, the position's state is set to 0.
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());


        testBoard.togglePlayer();
        testBoard.setState(mill);
        testBoard.removePiece('a',7,testBoard.getCurrentPlayer());
        testBoard.togglePlayer();
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].getStatus()== 0);
    }

    SECTION("Position status if it's part of mill")
    {
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        //piece should not be part of a mill
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].checkIfPartOfMill(testBoard.getPositions()) == false);
        testBoard.doMovePhase1('a',7,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',4,testBoard.getCurrentPlayer());
        testBoard.doMovePhase1('a',1,testBoard.getCurrentPlayer());
        //now it should be
        REQUIRE(testBoard.getPositions()['a'-asciiOffset][7-1].checkIfPartOfMill(testBoard.getPositions()) == true);


    }


}


