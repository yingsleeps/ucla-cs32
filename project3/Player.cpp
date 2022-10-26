#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g)  // done!
{}

bool HumanPlayer::isHuman() const  // done!
{
    return true;
}

bool HumanPlayer::placeShips(Board& b)  // done!
{
    char d;
    int r, c;
    Direction dir = HORIZONTAL;

    cout << name() << " " << "must place " << game().nShips() << " " << "ships." << endl;
    b.clear();
    // loops until every ship has been placed
    for (int i = 0; i < game().nShips(); i++)
    {
        b.display(false);
        bool dvalid = false;
        // continues to prompt the user until a valid direction is entered
        while (dvalid == false)
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin >> d;
            cin.ignore(10000, '\n');
            switch(d)
            {
                case 'h':
                    dir = HORIZONTAL;
                    dvalid = true;
                    break;
                case 'v':
                    dir = VERTICAL;
                    dvalid = true;
                    break;
                default:
                    cout << "Direction must be h or v." << endl;
            }
        }
        
        bool pvalid = false;
        // continues to prompt the user until a valid point is entered
        while (pvalid == false)
        {
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            if (getLineWithTwoIntegers(r, c) == false)
            {
                cout << "You must enter two integers." << endl;
                break;
            }
            if (b.placeShip(Point(r,c), i, dir) == false)
            {
                cout << "The ship can not be placed there." << endl;
            }
            else
            {
                b.placeShip(Point(r,c), i, dir);
                pvalid = true;
            }
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()  // done!
{
    int r = -1;
    int c = -1;
    bool pvalid = false;
    // continues to prompt the user until a valid point is entered
    while (pvalid == false)
    {
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        if (getLineWithTwoIntegers(r, c) == false)
        {
            cout << "You must enter two integers." << endl;
        }
        else
        {
            pvalid = true;
        }
    }
    return Point(r,c);
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)  // done!
{}

void HumanPlayer::recordAttackByOpponent(Point p)  // done!
{}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    bool recursivePlace(Board &b, int shipID);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    int m_state;
    vector<Point> m_attackedPositions;  // stores positions that have been attacked
    vector<Point> m_valid;  // stores pool of positions to attack in state 2
    bool isIn(vector<Point> a, Point x);  // checks if point has already been attacked
    void validPos(Point p);  // generates pool of positions to attack in state 2
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), m_state(1)  // done!
{}

bool MediocrePlayer::recursivePlace(Board &b, int shipID)  // done!
{
    // base case, if you reach the last ship you must place
    if (shipID == game().nShips())
    {
        return true;
    }
    // loop through the board
    for (int i = 0; i < game().rows(); i++)
    {
        for (int k = 0; k < game().cols(); k++)
        {
            // try placing the ship horizontally or vertically
            if (b.placeShip(Point(i,k), shipID, HORIZONTAL) || b.placeShip(Point(i,k), shipID, VERTICAL))
            {
                // if possible, try placing the next ship
                if (recursivePlace(b, shipID + 1))
                {
                    return true;
                }
                // if next ship cannot be placed, unplace the first ship and move on to next point
                else
                {
                    b.unplaceShip(Point(i,k), shipID, HORIZONTAL);
                    b.unplaceShip(Point(i,k), shipID, VERTICAL);
                }
            }
        }
    }
    return false;
}

bool MediocrePlayer::placeShips(Board &b)  // done!
{
    // will return false after 50 iterations
    for (int i = 1; i <= 50; i++)
    {
        // first block off half the grid
        b.block();
        // if ships are placed return true after unblocking
        if (recursivePlace(b, 0))
        {
            b.unblock();
            return true;
        }
        // if not, unblock and try again
        b.unblock();
    }
    return false;
}

Point MediocrePlayer::recommendAttack()  // done!
{
    Point p;
    bool beenAttacked = true;
    // if the player is in state 1
    if (m_state == 1)
    {
        // loops until a point that hasn't been attacked yet is found
        while (beenAttacked == true)
        {
            p = game().randomPoint();
            // checks if generated point has already been attacked
            if (isIn(m_attackedPositions, p))
            {
                continue;
            }
            else
            {
                beenAttacked = false;
            }
        }
        return p;
    }
    // if the player is in state 2
    else
    {
        // loops until a point that hasn't been attacked yet is found
        while (beenAttacked == true)
        {
            // picks a random point from the allowed pool of points
            int random = randInt(m_valid.size());
            vector<Point>::iterator pos = m_valid.begin();
            p = m_valid[random];
            // if point has already been attacked, remove from pool and find new point
            if (isIn(m_attackedPositions, p))
            {
                pos = pos + random;
                m_valid.erase(pos);
                continue;
            }
            // if not, just remove from pool and return that point
            else
            {
                beenAttacked = false;
                pos = pos + random;
                m_valid.erase(pos);
            }
        }
        return p;
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)  // done!
{
    // adds attacked point to the list
    m_attackedPositions.push_back(p);
    // if in state 1
    if (m_state == 1)
    {
        // if hit a ship but didn't destroy, change to state 2 and generate pool of points
        if (shotHit && !shipDestroyed)
        {
            m_state = 2;
            validPos(p);
            if (m_valid.empty())
            {
                m_state = 1;
            }
        }
    }
    // if in state 2
    else
    {
        // if pool is empty, just switch back to state 1
        if (m_valid.empty())
        {
            m_state = 1;
        }
        // if ship is destroyed, switch back to state 1 and reset the pool of points
        if (shotHit && shipDestroyed)
        {
            m_state = 1;
            m_valid.clear();
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p)  // done!
{}

bool MediocrePlayer::isIn(vector<Point> a, Point x)  // done!
{
    // loops through the vector passed and checks if point passed is in it
    for (int i = 0; i < a.size(); i++)
    {
        if (x.r == a[i].r && x.c == a[i].c)
        {
            return true;
        }
    }
    return false;
}

void MediocrePlayer::validPos(Point p)  // done!!
{
    // generates a pool of at most 16 points
    for (int i = 1; i <= 4; i++)
    {
        Point pL(p.r, p.c - i);
        Point pR(p.r, p.c + i);
        Point pU(p.r - i, p.c);
        Point pD(p.r + i, p.c);
        // checks if each point is valid before adding it to the pool
        if (game().isValid(pL))
        {
            m_valid.push_back(pL);
        }
        if (game().isValid(pR))
        {
            m_valid.push_back(pR);
        }
        if (game().isValid(pU))
        {
            m_valid.push_back(pU);
        }
        if (game().isValid(pD))
        {
            m_valid.push_back(pD);
        }
    }
}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
    int m_state;  // keeps track of hunt/target mode
    int m_probability[MAXROWS][MAXCOLS];  // grid for cell probabilities
    vector<Point> m_badPoints;  // stores points we don't want to place ships onto
    vector<Point> m_attackedPositions;  // stores all the points that have already been hit
    stack<Point> m_valid;  // stores the crosshair of the 4 adjectent points to a shot
    void calculateProbability();  // calibrates the probabilites depending on shots made so far
    Point highestProb();  // returns the highest probability from the grid
    bool isIn(vector<Point> a, Point x);  // checks if the point has already been hit
    void validPos(Point p);  // generates a pool of valid points to shoot during target mode
    
};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), m_state(1), m_probability{}
{}


bool GoodPlayer::placeShips(Board &b)
{
    int counter = 0;
    while (counter != game().nShips())
    {
        for (int s = 0; s <= game().nShips(); s++)
        {
            bool placed = false;
            while (placed == false)
            {
                Point p = game().randomPoint();
                if (isIn(m_badPoints, p))
                {
                    break;
                }
                if (b.placeShip(p, s, HORIZONTAL) || b.placeShip(p, s, VERTICAL))
                {
                    placed = true;
                    counter++;
                    for (int i = 0; i < 2; i++)
                    {
                        m_badPoints.push_back(Point(p.r + i, p.c - i));
                        m_badPoints.push_back(Point(p.r - i, p.c + i));
                    }
                }
            }
        }
    }
    return true;
}

Point GoodPlayer::recommendAttack()  // done!!
{
    bool beenAttacked = true;
    Point p;
    // bot is in state 1, ie hunt mode
    if (m_state == 1)
    {
        // find the probabilties of all the cells and return the highest
        calculateProbability();
        p = highestProb();
    }
    // bot is in state 2, ie target mode
    else
    {
        // loop to look for a point that hasn't been hit yet
        while (beenAttacked && !m_valid.empty())
        {
            p = m_valid.top();
            // if already hit, remove from pool and try again
            if (isIn(m_attackedPositions, p))
            {
                m_valid.pop();
                continue;
            }
            // if not, just remove from pool and return
            else
            {
                beenAttacked = false;
                m_valid.pop();
            }
        }
    }
    return p;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)  // done!!
{
    // adds attacked position to list of already attacked
    m_attackedPositions.push_back(p);
    // bot is currently in state 1, ie hunt mode
    if (m_state == 1)
    {
        // if it hit a ship, transition to state 2, ie target mode, and generate a crosshair of values to hit
        if (shotHit && !shipDestroyed)
        {
            m_state = 2;
            validPos(p);
        }
    }
    // bot is currently in state 2, ie target mode
    else
    {
        // if it hit a ship but didn't sink, add the new crosshair values
        if (shotHit)
        {
            validPos(p);
        }
        // if there's no more values to check, change back to stage 1
        if (m_valid.empty())
        {
            m_state = 1;
        }
        // if it hits a ship and sinks it, change back to state 1 and remove all the crosshair values we didn't check
        else if (shotHit && shipDestroyed)
        {
            m_state = 1;
            while (!m_valid.empty())
            {
                m_valid.pop();
            }
        }
    }
}

void GoodPlayer::recordAttackByOpponent(Point p)  // done!
{}

void GoodPlayer::validPos(Point p)  // done!!
{
    // the four adjacent points
    Point pL(p.r, p.c - 1);
    Point pR(p.r, p.c + 1);
    Point pU(p.r - 1, p.c);
    Point pD(p.r + 1, p.c);
    // checks if each point is valid before adding it to the pool
    if (game().isValid(pL))
    {
        m_valid.push(pL);
    }
    if (game().isValid(pR))
    {
        m_valid.push(pR);
    }
    if (game().isValid(pU))
    {
        m_valid.push(pU);
    }
    if (game().isValid(pD))
    {
        m_valid.push(pD);
    }
}

void GoodPlayer::calculateProbability()  // done!
{
    // variable declarations
    int ship = 0;
    bool valid = true;
    // loops through every ship in the game
    while (ship < game().nShips())
    {
        // placing the ships horizontally
        for (int r = 0; r < game().rows(); r++)
        {
            for (int c = 0; (c + game().shipLength(ship)) <= game().cols(); c++)
            {
                // tries to place the ship horizontally
                for (int i = 0; i < game().shipLength(ship); i++)
                {
                    if (isIn(m_attackedPositions, Point(r, c + i)))
                    {
                        valid = false;
                    }
                }
                // if it works, add 1 to the probability value of that cell
                if (valid)
                {
                    for (int i = 0; i < game().shipLength(ship); i++)
                    {
                        if (!isIn(m_attackedPositions, Point(r, c + i)))
                        {
                            m_probability[r][c + i]++;
                        }
                    }
                }
                valid = true;
            }
        }
        // placing the ships vertically
        for (int c = 0; c < game().cols(); c++)
        {
            for (int r = 0; (r + game().shipLength(ship)) <= game().rows(); r++)
            {
                // tries to place the ships vertically
                for (int i = 0; i < game().shipLength(ship); i++)
                {
                    if (isIn(m_attackedPositions, Point(r + i, c)))
                    {
                        valid = false;
                    }
                }
                // if it works, add 1 to the probability value of that cell
                if (valid)
                {
                    for (int i = 0; i < game().shipLength(ship); i++)
                    {
                        if (!isIn(m_attackedPositions, Point(r + i, c)))
                        {
                            m_probability[r + i][c]++;
                        }
                    }
                }
                valid = true;
            }
        }
        ship++;
    }
}

Point GoodPlayer::highestProb()  // done!
{
    int highest = 0;
    int r = 0;
    int c = 0;
    // loops through the probability board to find the highest probability
    for (int i = 0; i < game().rows(); i++)
    {
        for (int k = 0; k < game().cols(); k++)
        {
            if (m_probability[i][k] > highest)
            {
                // if the point has not been attacked, set is as the highest so far
                if (!isIn(m_attackedPositions, Point(i, k)))
                {
                    highest = m_probability[i][k];
                    r = i;
                    c = k;
                }
            }
        }
    }
    Point p(r, c);
    return p;
}

bool GoodPlayer::isIn(vector<Point> a, Point x)  // done!
{
    // loops through the vector passed and checks if point passed is in it
    for (int i = 0; i < a.size(); i++)
    {
        if (x.r == a[i].r && x.c == a[i].c)
        {
            return true;
        }
    }
    return false;
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}

