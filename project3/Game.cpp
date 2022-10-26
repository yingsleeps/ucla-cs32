#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    struct Ship
    {
        int m_length;
        char m_symbol;
        string m_name;
    };
    vector<Ship> shipDeck;
    int m_rows;
    int m_cols;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : m_rows(nRows), m_cols(nCols)
{}

int GameImpl::rows() const
{
    return m_rows;  // done!
}

int GameImpl::cols() const
{
    return m_cols;  // done!
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)  // done!!
{
    Ship boat = {length, symbol, name};  // initialize the new ship
    shipDeck.push_back(boat);  // add the ship to the ship vector
    return true;
}

int GameImpl::nShips() const
{
    return shipDeck.size();  // done!
}

int GameImpl::shipLength(int shipId) const  // done!
{
    return shipDeck.at(shipId).m_length;
}

char GameImpl::shipSymbol(int shipId) const  // done!
{
    return shipDeck.at(shipId).m_symbol;
}

string GameImpl::shipName(int shipId) const  // done!
{
    return shipDeck.at(shipId).m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)  // done! (i hope)
{
    bool hit, destroy, valid;
    int id = -1;
    
    if (!p1->placeShips(b1) || !p2->placeShips(b2))
    {
        return nullptr;
    }
    // keeps running until a player has won, ie no more ships on their opponents board
    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        // player 1's turn
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        // displays the board
        if (p1->isHuman())
        {
            b2.display(true);
        }
        else
        {
            b2.display(false);
        }
        Point pos1 = p1->recommendAttack();
        // if the shot was not valid
        if (!b2.attack(pos1, hit, destroy, id))
        {
            valid = false;
            cout << p1->name() << " wasted a shot at (" << pos1.r << "," << pos1.c << ")." << endl;
            p1->recordAttackResult(pos1, valid, hit, destroy, id);
        }
        else
        {
            // if a ship was destroyed
            if (hit && destroy)
            {
                valid = true;
                cout << p1->name() << " attacked (" << pos1.r << "," << pos1.c << ") and destroyed the " << p2->game().shipName(id) << ", resulting in:" << endl;
                p1->recordAttackResult(pos1, valid, hit, destroy, id);
                // displays the board
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            // if a ship was hit but not destroyed
            else if (hit)
            {
                valid = true;
                cout << p1->name() << " attacked (" << pos1.r << "," << pos1.c << ") and hit something, resulting in:" << endl;
                p1->recordAttackResult(pos1, valid, hit, destroy, id);
                // displays the board
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            // if a shot was valid but no ship was hit
            else
            {
                valid = true;
                cout << p1->name() << " attacked (" << pos1.r << "," << pos1.c << ") and missed, resulting in:" << endl;
                p1->recordAttackResult(pos1, valid, hit, destroy, id);
                // displays the board
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            
        }
        // if no ships left on board 2, player 1 wins
        if (b2.allShipsDestroyed())
        {
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
        if (shouldPause)
        {
            waitForEnter();
        }
        // player 2's turn
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        // displays the board
        if (p2->isHuman())
        {
            b1.display(true);
        }
        else
        {
            b1.display(false);
        }
        Point pos2 = p2->recommendAttack();
        // if the shot was not valid
        if (!b1.attack(pos2, hit, destroy, id))
        {
            valid = false;
            cout << p2->name() << " wasted a shot at (" << pos2.r << "," << pos2.c << ")." << endl;
            p2->recordAttackResult(pos2, valid, hit, destroy, id);
        }
        else
        {
            // if a ship was destroyed
            if (hit && destroy)
            {
                valid = true;
                cout << p2->name() << " attacked (" << pos2.r << "," << pos2.c << ") and destroyed the " << p1->game().shipName(id) << ", resulting in:" << endl;
                p2->recordAttackResult(pos2, valid, hit, destroy, id);
                // displays the board
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            // if a ship was hit but not destroyed
            else if (hit)
            {
                valid = true;
                cout << p2->name() << " attacked (" << pos2.r << "," << pos2.c << ") and hit something, resulting in:" << endl;
                p2->recordAttackResult(pos2, valid, hit, destroy, id);
                // displays the board
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            // if a shot was valid but no ship was hit
            else
            {
                valid = true;
                cout << p2->name() << " attacked (" << pos2.r << "," << pos2.c << ") and missed, resulting in:" << endl;
                p2->recordAttackResult(pos2, valid, hit, destroy, id);
                // displays the board
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
        }
        // if no ships left on board 1, player 2 wins
        if (b1.allShipsDestroyed())
        {
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
        if (shouldPause)
        {
            waitForEnter();
        }
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

