#include "manualtests.h"

manualTests::manualTests()
{

}

void manualTests::timeToFlyUnitTest() {
    Player p1(1, false);
    Player p2(2, false);
    //case 1: the player's pieces assigned to phase 2 has decrementented to 3. (flight mode reached)
    p1.setPhase2Pieces(3); //int should be 3 or less
    p1.timetoFly();
    cout << "Testing for player pieces in phase 2 <= 3..." << endl;
    if (p1.getFlight() == true)
        cout << "Result: Player " << p1.getPlayerID() << " has entered Flight Mode!" << endl;
    else
        cout << "Result: False, player " << p1.getPlayerID() << " not in flight mode." << endl;

    cout << "Expected result: Player 1 has entered Flight Mode!" << endl << endl;

    //case 2: the player has more than 3 pieces assigned to phase 2. (not in flight mode)
    p2.setPhase2Pieces(4); //int should be 4 or greater
    p2.timetoFly();
    cout << "Testing for player pieces in phase 2 > 3..." << endl;
    if (p2.getFlight() == false)
        cout << "Result: False, player " << p2.getPlayerID() << " not in flight mode." << endl;
    else
        cout << "Result: Player " << p2.getPlayerID() << " has entered Flight Mode!" << endl;

    cout << "Expected result: False, player " << p2.getPlayerID() << " not in flight mode." << endl << endl;
}

void manualTests::subtractPieceUnitTest() { //Also test addPiece function
    GameStates gs1 = phase1;
    GameStates gs2 = phase2;
    Player p1(1, false);

    p1.subtractPiece(gs1);
    cout << "Testing new game (phase 1), opposing player takes a piece." << endl;
    cout << "Result: "<< p1.getPhase1Pieces() << endl;
    cout << "Expected Result: 8" << endl << endl;

    //adding pieces to phase2 to test other gameState
    //Phase 2 pieces = 3.
    p1.addPiece();
    p1.addPiece();
    p1.addPiece();

    p1.subtractPiece(gs2);
    cout << "Testing subtracting pieces while in phase 2 of game." << endl;
    cout << "Result: " << p1.getPhase2Pieces() << endl;
    cout << "Expected Result: 2" << endl << endl;
}

void manualTests::formMillTest() {
    Board gameboard;

    Player player1(1, false);
    Player player2(2, false);
    gameboard.addPlayers(player1);
    gameboard.addPlayers(player2);
    gameboard.setState(phase1);
    gameboard.setCurrPlayerVar(1);

    gameboard.doMovePhase1('a', 1, gameboard.getCurrentPlayer());
    gameboard.doMovePhase1('d', 1, gameboard.getCurrentPlayer());
    gameboard.doMovePhase1('g', 1, gameboard.getCurrentPlayer());
    gameboard.checkAndChangeState(gameboard.getPlayers().at(0), gameboard.getPlayers().at(1));

    if (gameboard.getState() == mill)
        cout << "Result: " << gameboard.getCurrentPlayer().getPlayerID() << " has made a Mill! Select a piece to remove: " << endl;
    else
        cout << "Result: Test Failed" << endl;

    cout << "Expected Result: " << gameboard.getCurrentPlayer().getPlayerID() << " has made a Mill! Select a piece to remove: " << endl;
    system("pause");
}

void manualTests::moveValidityTest() {
    Board gameboard;
    Player player1(1, false);
    gameboard.addPlayers(player1);
    gameboard.setState(phase1);
    gameboard.setCurrPlayerVar(1);
    //A valid move
    if (!gameboard.phase1Check('a', 1))
        cout << "Result: move invalid, test failed" << endl;
    else
        cout << "Result: Valid move" << endl;
    cout << "Expected Result: Valid move" << endl;

    //An invalid move that is out of bounds
    if (!gameboard.phase1Check('z', 9))
        cout << "Result: move invalid" << endl;
    else
        cout << "Result: Valid move" << endl;
    cout << "Expected Result: move invalid" << endl;

    //Invalid move, space is occupied
    gameboard.doMovePhase1('a', 1, gameboard.getCurrentPlayer());
    if (!gameboard.phase1Check('a', 1))
        cout << "Result: move invalid" << endl;
    else
        cout << "Result: Valid move" << endl;
    cout << "Expected Result: move invalid" << endl;
    system("pause");
}

void manualTests::FlightMovementTest() {
    Board gameboard;
    Player player1(1, false);
    gameboard.addPlayers(player1);
    gameboard.setState(phase1);
    gameboard.setCurrPlayerVar(1);
    gameboard.doMovePhase1('a', 1, gameboard.getCurrentPlayer());
    gameboard.doMovePhase1('d', 1, gameboard.getCurrentPlayer());
    //Testing a valid move
    gameboard.setState(phase2);
    if (gameboard.isValidLocation('d', 1, 'g', 1, gameboard.getCurrentPlayer()) == false)
        cout << "Result: Invalid move" << endl;
    else
        cout << "Result: Valid Move" << endl;
    cout << "Expected Result: Valid Move" << endl;

    //Test for moving to an occupied space.
    if (gameboard.isValidLocation('d', 1, 'a', 1, gameboard.getCurrentPlayer()) == false)
        cout << "Result: Invalid move" << endl;
    else
        cout << "Result: Valid Move" << endl;
    cout << "Expected Result: Invalid move" << endl;
    system("pause");
}

void manualTests::doAllTests()
{
    timeToFlyUnitTest();
    subtractPieceUnitTest();
    formMillTest();
    moveValidityTest();
    FlightMovementTest();

}
