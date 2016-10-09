#include <iostream>
#include <tuple>
#include <array>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <memory>
using namespace std;

struct crownNode {
	crownNode() : level(0), pathLen(0), pos(make_pair(0, 0)), dangerlevel(0), prev_dir(make_pair(0, 0)), moveVal(0), crown(true){}
	short level; //Depth of the node (tree-like )
	short pathLen; // Length of path for each branching till leaf/end. earliest branching has highest values
	pair<short, short> pos;
	pair<short, short> capture;
	static short player;
	short dangerlevel;
	pair<short, short> prev_dir; // previous direction we're coming from. Initialize to the origin
	vector<crownNode> north_west;
	vector<crownNode> north_east;
	vector<crownNode> south_west;
	vector<crownNode> south_east;
	int moveVal;
	bool crown;

};

struct Node {
	Node() : level(0), pathLen(0), pos(make_pair(0, 0)), dangerlevel(0), moveVal(0), crown(false), crownMember(new crownNode()){}
	short level; //Depth of the node in a tree-like setting
	short pathLen; // Length of path for each branching till leaf/end. earliest branching has highest values
	pair<short, short> pos;
	pair<short, short> capture;
	static short player;
	Node *north_west;
	Node *north_east;
	Node *south_west;
	Node *south_east;
	crownNode* crownMember;
	short dangerlevel;
	int moveVal;
	bool crown;
};

// Define addition of pairs
template <class T1, class T2>
void operator+= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
	lhs.first += rhs.first, lhs.second += rhs.second;
}

// Define Subtraction of pairs
template <class T1, class T2>
void operator-= (pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
	lhs.first -= rhs.first, lhs.second -= rhs.second;
}

class dame
{
public:
	dame(short, short);
	~dame();

	int getRows(string player);
	void printRows(string player);
	void setPlayers(string, string);
	void setSymbol(string, pair<short, short>, vector<string>&myboard = board);
	short getPlayer(string);
	short switchPlayer(string);
	short dame::switchPlayerId(short);
	void reset();
	pair<short, short> getStepNeighbors(pair<short, short>, pair<short, short>, short);
	list<pair<short, short>> getLongestTrail(pair<short, short>, pair<short, short>, vector<string>&myboard = board);
	list<pair<short,short>> getline(pair<short,short>);
	int *gettrail(int);
	int oneStepNeigh(int);

	template <class T> T *getPossibleCrownMoves(T*, vector<string>&myboard = board);
	template <class T> T *getPossiblePawnMoves(T*, vector<string>&myboard = board);
	Node *getPossibleMoves(Node*, vector<string>&myboard = board);

	template <class T> void move(T*, pair<short,short>, short);

	void capture(int,int,int);
	void printGrid();
	void printBoard(vector<string>&myboard=board);
	static vector<string> board;// = vector<string>();

	void updateBoard(list<pair<short, short>>, string, short, vector<string>&myboard = board);
	template <class T> short dangerLevel(T*, vector<string>&myboard = board);
	string getSymbol(pair<short, short>, vector<string>&myboard = board);
	short height;
	short width;
	string firstSymbol, secondSymbol, none;// symbols to assign players

private:
	string player1;
	string player2;
	short PLAYER1, PLAYER2, EMPTY, CURRENT_PLAYER;

	list<pair<short, short> > LEGAL_GRID; // Legitimate grid locations
	map<tuple<short, pair<short,short>>, bool>  Crowns; // To contain the crowns
	map<string, short> playerKey;
	map<short, string> symbols;
};
