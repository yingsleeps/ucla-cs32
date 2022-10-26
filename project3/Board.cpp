#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <cctype>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char board[MAXROWS][MAXCOLS];
    vector<int> placedShips;
};

BoardImpl::BoardImpl(const Game& g)  // done!
 : m_game(g)
{
    clear();
}

void BoardImpl::clear()  // done!
{
    // loops through the board and resets all the points so it's empty
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            board[i][k] = '.';
        }
    }
}

void BoardImpl::block()  // done!
{
    // loops through half of the points on the board
    for (int i = 0; i < (m_game.rows() * m_game.cols()) / 2; i++)
    {
        // finds a random point to block
        int r = randInt(m_game.rows());
        int c = randInt(m_game.cols());
        // if point is already blocked, keep on looking for a point
        while (board[r][c] == '@')
        {
            r = randInt(m_game.rows());
            c = randInt(m_game.cols());
        }
        board[r][c] = '@';
    }
}

void BoardImpl::unblock()  // done!
{
    // loop through the board and remove all the blocked points
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            if (board[i][k] == '@')
            {
                board[i][k] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)  // done (i hope)
{
    // checks if id is valid
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    // checks if starting point is on the board
    if (m_game.isValid(topOrLeft) == false)
    {
        return false;
    }
    // for horizontal ships
    if (dir == HORIZONTAL)
    {
        // checks if the ship will be partially off board
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            Point p(topOrLeft.r, i);
            if (m_game.isValid(p) == false)
            {
                return false;
            }
        }
        // checks if the ship will overlap an already placed ship or a blocked point
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            if (board[topOrLeft.r][i] != '.')
            {
                return false;
            }
        }
    }
    // for vertical ships
    if (dir == VERTICAL)
    {
        // checks if the ship will be partially off board
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            Point p(i, topOrLeft.c);
            if (m_game.isValid(p) == false)
            {
                return false;
            }
        }
        // checks if the ship will overlap an already placed ship or a blocked point
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            if (board[i][topOrLeft.c] != '.')
            {
                return false;
            }
        }
    }
    // checks if the ship has already been placed
    for (int i = 0; i < placedShips.size(); i++)
    {
        if (placedShips.at(i) == shipId)
        {
            return false;
        }
    }
    // if passes above tests, place the ship
    if (dir == HORIZONTAL)
    {
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            board[topOrLeft.r][i] = m_game.shipSymbol(shipId);
        }
    }
    else
    {
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            board[i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    // adds the shipID to the list of placed ships
    placedShips.push_back(shipId);
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)  // done!
{
    // checks if id is valid
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    // checks if the starting point is on the board
    if (m_game.isValid(topOrLeft) == false)
    {
        return false;
    }
    // for horizontal ships
    if (dir == HORIZONTAL)
    {
        // checks if the ship will be partially off board
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            Point p(topOrLeft.r, i);
            if (m_game.isValid(p) == false)
            {
                return false;
            }
        }
        // checks if the ship is at location indicated
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            if (board[topOrLeft.r][i] == '.')
            {
                return false;
            }
        }
    }
    // for vertical ships
    if (dir == VERTICAL)
    {
        // checks if the ship will be partially off board
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            Point p(i, topOrLeft.c);
            if (m_game.isValid(p) == false)
            {
                return false;
            }
        }
        // checks if the ship is at the location indicated
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            if (board[i][topOrLeft.c] == '.')
            {
                return false;
            }
        }
    }
    // to remove the ship from the placedShips vector
    for (int i = 0; i < placedShips.size(); i++)
    {
        if (placedShips.at(i) == shipId)
        {
            vector<int>::iterator pos = placedShips.begin() + i;
            placedShips.erase(pos);
        }
    }
    // unplaces the ships
    if (dir == HORIZONTAL)
    {
        for (int i = topOrLeft.c; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)
        {
            board[topOrLeft.r][i] = '.';
        }
    }
    else
    {
        for (int i = topOrLeft.r; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)
        {
            board[i][topOrLeft.c] = '.';
        }
    }
    return true;
}

void BoardImpl::display(bool shotsOnly) const  // done!!
{
    // first line
    cout << "  ";
    for (int i = 0; i < m_game.cols(); i++)
    {
        cout << i;
    }
    cout << endl;
    if (shotsOnly == true)
    {
        for (int i = 0; i < m_game.rows(); i++)
        {
            string contents;  // to record all the values you want in each row
            for (int k = 0; k < m_game.cols(); k++)
            {
                // display the shots made
                if (board[i][k] == 'X' || board[i][k] == 'o')
                {
                    contents += board[i][k];
                }
                // but not the ship symbols
                else
                {
                    contents += ".";
                }
            }
            cout << i << " " << contents << endl;
        }
    }
    // for shotsOnly being false, display ship symbols too
    else
    {
        for (int i = 0; i < m_game.rows(); i++)
        {
            string contents;
            for (int k = 0; k < m_game.cols(); k++)
            {
                contents += board[i][k];
            }
            cout << i << " " << contents << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)  // done! (i hope)
{
    shotHit = false;
    shipDestroyed = true;
    // if the point is not on the board
    if (m_game.isValid(p) == false)
    {
        return false;
    }
    // if the point has already been hit
    if (board[p.r][p.c] == 'o' || board[p.r][p.c] == 'X')
    {
        return false;
    }
    // if hitting a point with no ship
    if (board[p.r][p.c] == '.')
    {
        board[p.r][p.c] = 'o';
        return true;
    }
    // the case if a ship was hit
    else
    {
        // set shothit to true
        shotHit = true;
        char symbol = board[p.r][p.c];  // store the ship symbol
        board[p.r][p.c] = 'X';
        // check the board to see if there are still ship segments left
        for (int i = 0; i < m_game.rows(); i++)
        {
            for (int k = 0; k < m_game.cols(); k++)
            {
                if (board[i][k] == symbol)
                {
                    shipDestroyed = false;
                }
            }
        }
        // if the ship is destroyed, then set the id
        if (shipDestroyed == true)
        {
            // find the ship id by looping through vector with all the ships
            for(int i = 0; i < m_game.nShips(); i++)
            {
                if (symbol == m_game.shipSymbol(i))
                {
                    shipId = i;
                }
            }
        }
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const  // done! (i hope)
{
    // loops through the board to check if there are still ship symbols left
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            if (board[i][k] != '.' && board[i][k] != 'X' && board[i][k] != 'o')
            {
                return false;
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
