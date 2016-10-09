#include "dame.h"
#include <math.h>
#include <map>
#include <tuple>
#include <list>
#include <memory>

using namespace std;

short Node::player = 2;

short crownNode::player = 2;

vector<string> dame::board = vector<string>(); // initialize the board

dame::dame(short h, short w)
{
	dame::firstSymbol = "O";
	dame::secondSymbol = "X";
	dame::none = " ";
	dame::PLAYER1 = 1;
	dame::PLAYER2 = 2;
	dame::EMPTY = 0;
	symbols[PLAYER1] = firstSymbol;
	symbols[PLAYER2] = secondSymbol;
	height = h;
	width = w;
	//board = new string[height*width];
	short gridIndex = 0;
	list<pair<short,short>>::iterator it;
	short count_sec = 0;
	//vector<string> &board = vector<string>(w*h);
	board = vector<string>(w*h);

	for (short i = 0; i < height; i++)
	{
		for (short j = 0; j < width; j++)
		{

			board[i*height + j] = " ";
			if ((i+j)%2==0)
			{

				LEGAL_GRID.push_back(make_pair(i,j));

				Crowns[make_tuple(PLAYER1, make_pair(i, j))] = false; // Initialize crowns to false for both players
				Crowns[make_tuple(PLAYER2, make_pair(i, j))] = false;

				if (gridIndex<20)
				{

					board[i*height + j] = firstSymbol;
				}
				else if (gridIndex >= (height*width)/2-20)
				{

					board[i*height + j] = secondSymbol;
					count_sec++;
				}

				gridIndex++;
			}
		}
	}
	Crowns[make_tuple(PLAYER1, make_pair(-1, -1))] = false;
	Crowns[make_tuple(PLAYER2, make_pair(-1, -1))] = false;
	board[44] = " "; board[22] = " "; board[37] = " ";
	board[51] = firstSymbol;board[53] = firstSymbol;
	board[62] = " ";
	//secondPlayers[make_pair(6, 6)] = true;
	//board[66] = firstSymbol;


}

dame::~dame()
{
	//delete [] board;
}

void dame::setPlayers(string player1, string player2)
{
	playerKey[player1] = PLAYER1;
	playerKey[player2] = PLAYER2;
}

void dame::setSymbol(string str, pair<short, short>pos, vector<string> &myboard)
{
	myboard[pos.first*dame::height + pos.second] = str;
}

short dame::getPlayer(string player)
{
	return playerKey[player];
}

short dame::switchPlayer(string player)
{
	if (playerKey[player] == PLAYER1)
	{
		CURRENT_PLAYER = PLAYER2;
		return PLAYER2;
	}
	else
	{
		CURRENT_PLAYER = PLAYER1;
		return PLAYER1;
	}
}

short dame::switchPlayerId(short playerId)
{
	if (playerId==1)
	{
		return PLAYER2;
	}
	else
	{
		return PLAYER1;
	}
}

void dame::printBoard(vector<string>&myboard)
{
	string strBoard = "";
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			strBoard += myboard[i*height + j];
			if (j==width-1)
			{
				strBoard += "\n";
			}
			else
			{
				strBoard += " | ";
			}
		}

		if (i != height-1)
		{
			strBoard += string(4 * height - 3, '-');
			strBoard += "\n";
		}
	}
	cout << strBoard << "\n";
}

void dame::updateBoard(list<pair<short, short>> mylist, string action, short player, vector<string> &myboard)
{
	list<pair<short, short>>::iterator it;
	pair<short, short> position;
	if (action == "remove")
	{
		for (it = mylist.begin(); it != mylist.end(); it++)
		{
			position = *it;
			myboard[position.first*height + position.second] = " ";
		}
	}
	else if (action == "replace")
	{
		for (it = mylist.begin(); it != mylist.end(); it++)
		{
			position = *it;
			myboard[position.first*height + position.second] = symbols[player];
		}
	}
	else
	{
		cout << "Incorrect action" << endl;
	}
};

string dame::getSymbol(pair<short, short>position, vector<string>& myboard)
{
	if (!(find(LEGAL_GRID.begin(),LEGAL_GRID.end(),position) != LEGAL_GRID.end()))
	{
		return "Invalid position";
	}
	return myboard[position.first*height + position.second];
}

list<pair<short, short>> dame::getline(pair<short, short> position)
{
	list<pair<short, short>> line;
	for (int j = 0; j < width; j++)
	{
		short y1 = j + position.first - position.second;
		short y2 = position.first + position.second - j;
		pair<short, short> tup1 = make_pair(y1, j);
		pair<short, short> tup2 = make_pair(y2, j);
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), tup1) != LEGAL_GRID.end()) && (y1 != position.first))
		{
			line.push_back(tup1);
		}
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), tup2) != LEGAL_GRID.end()) && (y1 != position.first))
		{
			line.push_back(tup2);
		}
	}

	return line;
}

list<pair<short, short>> dame::getLongestTrail(pair<short, short> position, pair<short, short> direction, vector<string> &myboard)
{
	//*Given a starting position, and direction (pair), determine the trail from 'position' till another nonempty position
	list<pair<short, short>> neighbors;
	pair<short, short> candidate;

	short countSteps = 1;

	while (true && (countSteps<9)) // 9 because the grid width and heights are both 10 (change if necessary)
	{
		short x = position.first + countSteps*direction.first;
		short y = position.second + countSteps*direction.second;
		candidate = make_pair(x, y);
		neighbors.push_back(candidate);

		if (!(find(LEGAL_GRID.begin(), LEGAL_GRID.end(), candidate) != LEGAL_GRID.end())) // if not legal grid point
		{
			neighbors.pop_back();
			candidate = make_pair(-1, -1); // any outside grid point is assigned (-1,-1)
			neighbors.push_back(candidate);
			break;
		}

		if (myboard[x*height + y] != " ") // if occupied
		{

			break;
		}

		//candidate = make_pair(x, y);
		//neighbors.push_back(candidate);
		countSteps++;
	}

	return neighbors;
}

template<class T> short dame::dangerLevel(T* x, vector<string> &myboard)
{
	short danger = 0;
	pair<short, short> position = x->pos, oneStepSE, oneStepSW, oneStepNE, oneStepNW, crown, deadlock_NW, deadlock_NE, deadlock_SW, deadlock_SE;
	pair<short, short> SE = make_pair(1, 1), SW = make_pair(1, -1), NW = make_pair(-1, -1), NE = make_pair(-1, 1);
	list<pair<short, short>> trail_NW, trail_SE, trail_NE, trail_SW;


	trail_NE = dame::getLongestTrail(position, NE, myboard), deadlock_NE = trail_NE.back(), trail_NE.pop_back();
	trail_NW = dame::getLongestTrail(position, NW, myboard), deadlock_NW = trail_NW.back(), trail_NW.pop_back();
	trail_SE = dame::getLongestTrail(position, SE, myboard), deadlock_SE = trail_SE.back(), trail_SE.pop_back();
	trail_SW = dame::getLongestTrail(position, SW, myboard), deadlock_SW = trail_SW.back(), trail_SW.pop_back();


	// Can it be captured by pawns....
	oneStepSE = dame::getStepNeighbors(position, SE, 1);
	oneStepNE = dame::getStepNeighbors(position, NE, 1);
	oneStepNW = dame::getStepNeighbors(position, NW, 1);
	oneStepSW = dame::getStepNeighbors(position, SW, 1);
	if (dame::getSymbol(oneStepSE,myboard) == symbols[dame::switchPlayerId(x->player)] && dame::getSymbol(oneStepNW,myboard)==" ")
	{
		danger++;
	}
	if (dame::getSymbol(oneStepNE,myboard) == symbols[dame::switchPlayerId(x->player)] && dame::getSymbol(oneStepSW,myboard)==" ")
	{
		danger++;
	}
	if (dame::getSymbol(oneStepNW,myboard) == symbols[dame::switchPlayerId(x->player)] && dame::getSymbol(oneStepSE,myboard)==" ")
	{
		danger++;
	}

	if (dame::getSymbol(oneStepSW,myboard) == symbols[dame::switchPlayerId(x->player)] && dame::getSymbol(oneStepNE,myboard)==" ")
	{
		danger++;
	}


	// Can it be captured by crowns....

	if (Crowns[make_tuple(dame::switchPlayerId(x->player),deadlock_NW)] && (trail_SE.size() > 1))
	{
		danger++;
	}

	if (Crowns[make_tuple(dame::switchPlayerId(x->player), deadlock_NE)] && (trail_SW.size() > 1))
	{
		danger++;
	}

	if (Crowns[make_tuple(dame::switchPlayerId(x->player), deadlock_SW)] && (trail_NE.size() > 1))
	{
		danger++;
	}

	if (Crowns[make_tuple(dame::switchPlayerId(x->player), deadlock_SE)] && (trail_NW.size() > 1))
	{
		danger++;
	}
	return danger;
}

pair<short, short> dame::getStepNeighbors(pair<short, short> position, pair<short, short> direction, short step)
{
	/*Given a starting position, and direction (pair), determine the neighbor of 'position','step' steps away*/

	pair<short, short> candidate;

	candidate = make_pair(position.first + step*direction.first, position.second + step*direction.second);
	if (find(LEGAL_GRID.begin(), LEGAL_GRID.end(), candidate) != LEGAL_GRID.end())
	{
		return candidate;
	}
	else
	{
		return make_pair(-1, -1); // Return -1 to indicate no neighbors in this 'direction' was found
	}
}


template <class T> T *dame::getPossibleCrownMoves(T *moves, vector<string> &board)
{
	/* Given a current position and player, determine possible moves */

	list<pair<short, short>>::iterator it;
	pair<short, short> move_position = moves->pos, position;

	short player = moves->player;

	pair<short, short> pairs, oneStep, twoStep;
	// Directions
	pair<short, short> SE = make_pair(1, 1), SW = make_pair(1, -1), NW = make_pair(-1, -1), NE = make_pair(-1, 1), deadlock;

	// Trails
	list<pair<short, short>> trail_NW, trail_NE, trail_SW, trail_SE;

	/* North-West direction */
	printBoard(board);
	int movesValue1 = 0, movesValue2 = 0, movesValue3 = 0, movesValue4 = 0;

	if (!(moves->prev_dir == NW)) // If we are not coming from the SE direction
	{

		trail_NW = dame::getLongestTrail(move_position, NW, board);
		deadlock = trail_NW.back();
		trail_NW.pop_back(); // remove the deadlock from the trail leaving the possible landing points

		twoStep = dame::getStepNeighbors(deadlock, NW, 1);
		if ((dame::getSymbol(deadlock,board) == symbols[dame::switchPlayerId(player)]) && (dame::getSymbol(twoStep,board) == " "))
		{
			position = deadlock;
			dame::setSymbol(none, deadlock, board);
			trail_NW = dame::getLongestTrail(position, NW,board);
			deadlock = trail_NW.back();

			trail_NW.pop_back();
			int idx = 0;
			//moves->north_west = make_unique<crownNode[]>(trail_NE.size()); //
			for (list<pair<short, short>>::iterator it = trail_NW.begin(); it != trail_NW.end(); it++)
			{
				(moves->north_west).push_back(crownNode());
				moves->north_west[idx].pos = *it;
				moves->north_west[idx].level += moves->level + 2;
				moves->north_west[idx].capture = position;

				if (Crowns[make_pair(dame::switchPlayerId(moves->player), position)]) // Assign high value if it captures a crown
					moves->north_west[idx].moveVal += 4;
				else
				{
					moves->north_west[idx].moveVal += 2;
				}
				moves->north_west[idx].prev_dir -= NW; // Find the direction we are coming from
				((moves)->north_west[idx]) = *(dame::getPossibleCrownMoves(&(moves->north_west[idx]),board));
				movesValue1 = max(movesValue1, moves->north_west[idx].moveVal);


				idx++;
			}
		}

		else // if we cannot capture any
		{
			if (trail_NW.size() > 0) // if possible internal vertices/landing points
			{
				if (moves->level == 0) // if this is the first move for this crown
				{
					//moves->north_west = make_unique<crownNode*[]>(trail_NW.size()); //
					int idx = 0;
					for (list<pair<short, short>>::iterator it = trail_NW.begin(); it != trail_NW.end(); it++)
					{
						moves->north_west.push_back(crownNode());
						moves->north_west[idx].pos = *it;
						moves->north_west[idx].level += moves->level + 2;
						moves->north_west[idx].dangerlevel = dame::dangerLevel(&moves->north_west[idx],board);
						idx++;
					}

				}
				else // If we have used this crown during the current move
				{
					moves->dangerlevel = dame::dangerLevel(moves,board);
				}
			}
		}

	}

	// North-East direction
	if (!(moves->prev_dir == NE)) // If we are not coming from the NE direction
	{

		trail_NE = dame::getLongestTrail(move_position, NE, board);
		deadlock = trail_NE.back();
		trail_NE.pop_back(); // remove the deadlock from the trail leaving the possible landing points

		twoStep = dame::getStepNeighbors(deadlock, NE, 1);
		if ((dame::getSymbol(deadlock,board) == symbols[dame::switchPlayerId(player)]) && (dame::getSymbol(twoStep,board) == " "))
		{
			position = deadlock;
			dame::setSymbol(" ", deadlock, board);
			trail_NE = dame::getLongestTrail(position, NE, board);
			deadlock = trail_NE.back();
			trail_NE.pop_back();
			int idx = 0;
			for (list<pair<short, short>>::iterator it = trail_NE.begin(); it != trail_NE.end(); it++)
			{
				moves->north_east.push_back(crownNode());
				moves->north_east[idx].pos = *it;
				moves->north_east[idx].level += moves->level + 2;
				moves->north_east[idx].capture = position;

				if (Crowns[make_pair(dame::switchPlayerId(moves->player), position)]) // Assign high value if it captures a crown
					moves->north_east[idx].moveVal += 4;
				else
				{
					moves->north_east[idx].moveVal += 2;
				}

				moves->north_east[idx].prev_dir -= NE; // Find the direction we are coming from
				moves->north_east[idx] = *dame::getPossibleCrownMoves(&moves->north_east[idx],board);
				movesValue2 = max(movesValue2, moves->north_east[idx].moveVal);
				idx++;
			}
		}

		else // if we cannot capture any
		{
			if (trail_NE.size() > 0) // if possible internal vertices/landing points
			{
				if (moves->level == 0) // if this is the first move for this crown
				{
					//moves->north_east = make_unique<crownNode*[]>(trail_NE.size()); //
					int idx = 0;
					for (list<pair<short, short>>::iterator it = trail_NE.begin(); it != trail_NE.end(); it++)
					{
						moves->north_east.push_back(crownNode());
						moves->north_east[idx].pos = *it;
						moves->north_east[idx].level += moves->level + 2;
						moves->north_east[idx].dangerlevel = dame::dangerLevel(&moves->north_east[idx],board);
						idx++;
					}

				}
				else // If we have uNEd this crown during the current move
				{
					moves->dangerlevel = dame::dangerLevel(moves,board);
				}
			}
		}

	}

	// South-East direction
	if (!(moves->prev_dir == SE)) // If we are not coming from the SE direction
	{

		trail_SE = dame::getLongestTrail(move_position, SE, board);
		deadlock = trail_SE.back();
		trail_SE.pop_back(); // remove the deadlock from the trail leaving the possible landing points

		twoStep = dame::getStepNeighbors(deadlock, SE, 1);
		if ((dame::getSymbol(deadlock,board) == symbols[dame::switchPlayerId(player)]) && (dame::getSymbol(twoStep,board) == " "))
		{
			position = deadlock;
			dame::setSymbol(" ", deadlock, board);
			trail_SE = dame::getLongestTrail(position, SE, board);
			deadlock = trail_SE.back();
			trail_SE.pop_back();
			int idx = 0;
			for (list<pair<short, short>>::iterator it = trail_SE.begin(); it != trail_SE.end(); it++)
			{
				moves->south_east.push_back(crownNode());
				moves->south_east[idx].pos = *it;
				moves->south_east[idx].level += moves->level + 2;
				moves->south_east[idx].capture = position;

				if (Crowns[make_pair(dame::switchPlayerId(moves->player), position)]) // Assign high value if it captures a crown
					moves->south_east[idx].moveVal += 4;
				else
				{
					moves->south_east[idx].moveVal += 2;
				}

				moves->south_east[idx].prev_dir -= SE; // Find the direction we are coming from
				moves->south_east[idx] = *dame::getPossibleCrownMoves(&moves->south_east[idx],board);
				movesValue2 = max(movesValue3, moves->south_east[idx].moveVal);
				idx++;
			}
		}

		else // if we cannot capture any
		{
			if (trail_SE.size() > 0) // if possible internal vertices/landing points
			{
				if (moves->level == 0) // if this is the first move for this crown
				{

					int idx = 0;
					for (list<pair<short, short>>::iterator it = trail_SE.begin(); it != trail_SE.end(); it++)
					{
						moves->south_east.push_back(crownNode());
						moves->south_east[idx].pos = *it;
						moves->south_east[idx].level += moves->level + 2;
						moves->south_east[idx].dangerlevel = dame::dangerLevel(&moves->south_east[idx],board);
						idx++;
					}

				}
				else // If we have used this crown during the current move
				{
					moves->dangerlevel = dame::dangerLevel(moves,board);
				}
			}
		}

	}


	// South-West direction
	if (!(moves->prev_dir == SW)) // If we are not coming from the SW direction
	{
		trail_SW = dame::getLongestTrail(move_position, SW, board);
		deadlock = trail_SW.back();
		trail_SW.pop_back(); // remove the deadlock from the trail leaving the possible landing points

		twoStep = dame::getStepNeighbors(deadlock, SW, 1);
		if ((dame::getSymbol(deadlock,board) == symbols[dame::switchPlayerId(player)]) && (dame::getSymbol(twoStep,board) == " "))
		{
			position = deadlock;
			dame::setSymbol(" ", deadlock, board);
			trail_SW = dame::getLongestTrail(position, SW, board);
			deadlock = trail_SW.back();
			trail_SW.pop_back();
			int idx = 0;
			for (list<pair<short, short>>::iterator it = trail_SW.begin(); it != trail_SW.end(); it++)
			{
				moves->south_west.push_back(crownNode());
				moves->south_west[idx].pos = *it;
				moves->south_west[idx].level += moves->level + 2;
				moves->south_west[idx].capture = position;

				if (Crowns[make_pair(dame::switchPlayerId(moves->player), position)]) // Assign high value if it captures a crown
					moves->south_west[idx].moveVal += 4;
				else
				{
					moves->south_west[idx].moveVal += 2;
				}

				moves->south_west[idx].prev_dir -= SW; // Find the direction we are coming from
				moves->south_west[idx] = *dame::getPossibleCrownMoves(&moves->south_west[idx],board);
				movesValue4 = max(movesValue4, moves->south_west[idx].moveVal);
				idx++;
			}
		}

		else // if we cannot capture any
		{
			if (trail_SW.size() > 0) // if possible internal vertices/landing points
			{
				if (moves->level == 0) // if this is the first move for this crown
				{
					moves->south_west.push_back(crownNode());
					int idx = 0;
					for (list<pair<short, short>>::iterator it = trail_SW.begin(); it != trail_SW.end(); it++)
					{
						moves->south_west.push_back(crownNode());
						moves->south_west[idx].pos = *it;
						moves->south_west[idx].level += moves->level + 2;
						moves->south_west[idx].dangerlevel = dame::dangerLevel(&moves->south_west[idx],board);
						idx++;
					}

				}
				else // If we have used this crown during the current move
				{
					moves->dangerlevel = dame::dangerLevel(moves,board);
				}
			}
		}

	}


	int max_moveVal = max(movesValue1, movesValue2);
	max_moveVal = max(max_moveVal, movesValue3); max_moveVal = max(max_moveVal, movesValue4);
	moves->moveVal = (moves->moveVal) + max_moveVal;
	return moves;
}

template <class T> T *dame::getPossiblePawnMoves(T *moves, vector<string> &board)
{

	/* Given a current position and player, determine possible moves */

	list<pair<short, short>>::iterator it;
	pair<short, short> position = moves->pos;

	short player = moves->player;

	pair<short, short> pairs, oneStep, twoStep;

	moves->north_west = &Node();
	moves->north_east = &Node();
	moves->south_east = &Node();
	moves->south_west = &Node();
	if (player == 1)
	{
		/* down right direction */
		oneStep = dame::getStepNeighbors(position, make_pair(1, 1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(1, 1), 2);

		if ((board[oneStep.first*height + oneStep.second] == " ") && (moves->level == 0))
		{
			moves->south_east->pos = oneStep;
			moves->south_east->level++;
			moves->south_east->pathLen++;
			moves->south_east->dangerlevel = dame::dangerLevel(moves->south_east, board);

		}
		else if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(board[oneStep.first*height + oneStep.second] == secondSymbol) &&
			(board[twoStep.first*height + twoStep.second] == " "))
		{
			board[oneStep.first*height + oneStep.second] = firstSymbol;
			moves->south_east->pos = twoStep;
			moves->south_east->capture = oneStep;
			moves->south_east->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
			moves->south_east->moveVal += 2*(moves->moveVal + 2);
			else
			{
				moves->south_east->moveVal += (moves->moveVal + 2);
			}
			//moves->south_east->dangerlevel = dame::dangerLevel(moves->south_east, board);
			moves->south_east = dame::getPossiblePawnMoves(moves->south_east,board);
		}

		/* down left direction */
		oneStep = dame::getStepNeighbors(position, make_pair(1, -1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(1, -1), 2);

		if ((board[oneStep.first*height + oneStep.second] == " ") && (moves->level == 0))
		{
			moves->south_west->pos = oneStep;
			moves->south_west->level++;
			moves->south_west->pathLen++;
			moves->south_west->dangerlevel = dame::dangerLevel(moves->south_west, board);
		}
		else if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(board[oneStep.first*height + oneStep.second] == secondSymbol) &&
			(board[twoStep.first*height + twoStep.second] == " "))
		{
			board[oneStep.first*height + oneStep.second] = firstSymbol;
			moves->south_west->pos = twoStep;
			moves->south_west->capture = oneStep;
			moves->south_west->level += (moves->level + 2);
			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->south_west->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->south_west->moveVal += (moves->moveVal + 2);
			}
			//moves->south_west->dangerlevel = dame::dangerLevel(moves->south_west, board);
			moves->south_west = dame::getPossiblePawnMoves(moves->south_west, board);
			//moves->south_west->pathLen += moves->south_west->level;
		}

		/* top right direction */
		oneStep = dame::getStepNeighbors(position, make_pair(-1, 1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(-1, 1), 2);
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(board[oneStep.first*height + oneStep.second] == secondSymbol) &&
			(board[twoStep.first*height + twoStep.second] == " "))
		{
			board[oneStep.first*height + oneStep.second] = firstSymbol;
			printBoard(board);
			moves->north_east->pos = twoStep;
			moves->north_east->capture = oneStep;
			moves->north_east->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->north_east->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->north_east->moveVal += (moves->moveVal + 2);
			}
			//moves->north_east->dangerlevel = dame::dangerLevel(moves->north_east, board);
			moves->north_east = dame::getPossiblePawnMoves(moves->north_east,board);
			//moves->north_east->pathLen += moves->north_east->level;
		}

		/* top left direction */
		oneStep = dame::getStepNeighbors(position, make_pair(-1, -1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(-1, -1), 2);
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(board[oneStep.first*height + oneStep.second] == secondSymbol) &&
			(board[twoStep.first*height + twoStep.second] == " "))
		{
			board[oneStep.first*height + oneStep.second] = firstSymbol; // Update the board and replace current step symbol
			moves->north_west->pos = twoStep;
			moves->north_west->capture = oneStep;
			moves->north_west->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->north_west->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->north_west->moveVal += (moves->moveVal + 2);
			}
			//moves->north_west->dangerlevel = dame::dangerLevel(moves->north_west, board);
			moves->north_west = dame::getPossiblePawnMoves(moves->north_west, board);
			//moves->north_west->pathLen += moves->north_west->level;
		}

	}
	else
	{
		/* Top left direction */
		oneStep = dame::getStepNeighbors(position, make_pair(-1, -1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(-1, -1), 2);
		if ((dame::getSymbol(oneStep,board) == " ") && (moves->level == 0))
		{
			moves->north_west->pos = oneStep;
			moves->north_west->level++;
			moves->north_west->pathLen++;
			moves->north_west->dangerlevel = dame::dangerLevel(moves->north_west, board);
		}
		else if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(dame::getSymbol(oneStep,board) == firstSymbol) &&
			(dame::getSymbol(twoStep, board) == " "))
		{
			board[oneStep.first*height + oneStep.second] = secondSymbol; // Update the board and replace current step symbol
			moves->north_west->pos = twoStep;
			moves->north_west->capture = oneStep;
			moves->north_west->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->north_west->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->north_west->moveVal += (moves->moveVal + 2);
			}
			//moves->north_west->dangerlevel = dame::dangerLevel(moves->north_west, board);
			moves->north_west = dame::getPossiblePawnMoves(moves->north_west, board);
		}

		/* top right direction */
		oneStep = dame::getStepNeighbors(position, make_pair(-1, 1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(-1, 1), 2);
		if ((dame::getSymbol(oneStep, board) == " ") && (moves->level == 0))
		{
			moves->north_east->pos = oneStep;
			moves->north_east->level++;
			moves->north_east->pathLen++;
			moves->north_east->dangerlevel = dame::dangerLevel(moves->north_east, board);
		}
		else if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(dame::getSymbol(oneStep, board) == firstSymbol) &&
			(dame::getSymbol(twoStep, board) == " "))
		{
			board[oneStep.first*height + oneStep.second] = secondSymbol;
			//dame::printBoard();
			moves->north_east->pos = twoStep;
			moves->north_east->capture = oneStep;
			moves->north_east->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->north_east->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->north_east->moveVal += (moves->moveVal + 2);
			}

			moves->north_east = dame::getPossiblePawnMoves(moves->north_east, board);
		}

		/* down right direction */
		oneStep = dame::getStepNeighbors(position, make_pair(1, 1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(1, 1), 2);
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(dame::getSymbol(oneStep, board) == firstSymbol) &&
			(dame::getSymbol(twoStep, board) == " "))
		{
			board[oneStep.first*height + oneStep.second] = secondSymbol;
			moves->south_east->pos = twoStep;
			moves->south_east->capture = oneStep;
			moves->south_east->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->south_east->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->south_east->moveVal += (moves->moveVal + 2);
			}

			//moves->south_east->dangerlevel = dame::dangerLevel(moves->south_east, board);
			moves->south_east = dame::getPossiblePawnMoves(moves->south_east, board);
		}

		/* down left direction */
		oneStep = dame::getStepNeighbors(position, make_pair(1, -1), 1);
		twoStep = dame::getStepNeighbors(position, make_pair(1, -1), 2);
		if ((find(LEGAL_GRID.begin(), LEGAL_GRID.end(), twoStep) != LEGAL_GRID.end()) &&
			(dame::getSymbol(oneStep, board) == firstSymbol) &&
			(dame::getSymbol(twoStep, board) == " "))
		{
			board[oneStep.first*height + oneStep.second] = secondSymbol;
			moves->south_west->pos = twoStep;
			moves->south_west->capture = oneStep;
			moves->south_west->level += (moves->level + 2);

			if (Crowns[make_pair(dame::switchPlayerId(moves->player), oneStep)]) // Assign high value if it captures a crown
				moves->south_west->moveVal += 2 * (moves->moveVal + 2);
			else
			{
				moves->south_west->moveVal += (moves->moveVal + 2);
			}

			//moves->south_west->dangerlevel = dame::dangerLevel(moves->south_west, board);
			moves->south_west = dame::getPossiblePawnMoves(moves->south_west,board);
		}

		}


		short path1 = moves->north_east->pathLen, movesValue1 = moves->north_east->moveVal;
		short path2 = moves->north_west->pathLen, movesValue2 = moves->north_west->moveVal;
		short path3 = moves->south_east->pathLen, movesValue3 = moves->south_east->moveVal;
		short path4 = moves->south_west->pathLen, movesValue4 = moves->south_west->moveVal;

	short max_path = max(path1, path2), max_moveVal = max(movesValue1,movesValue2);
	max_path = max(max_path, path3); max_path = max(max_path, path4);
	max_moveVal = max(max_moveVal, movesValue3); max_moveVal = max(max_moveVal, movesValue4);
	moves->pathLen = (moves->level) + max_path;
	moves->dangerlevel = dame::dangerLevel(moves, board);
	moves->moveVal = (moves->moveVal) + max_moveVal;
	return moves;
}

// Instantiate the template functions
template crownNode* dame::getPossibleCrownMoves<crownNode>(crownNode*, vector<string>&);
template Node* dame::getPossiblePawnMoves<Node>(Node*, vector<string>&);

Node *dame::getPossibleMoves(Node* node, vector<string>&myboard) // template for getting moves
{
	if (node->crown)
	{
		node->crownMember = dame::getPossibleCrownMoves(node->crownMember, myboard);
	}
	else
	{
		node = dame::getPossiblePawnMoves(node, myboard);
	}
	return node;
}

template <class T> void dame::move(T* x, pair<short, short> destination, short player)
{
	/* Moves 'player' from 'origin' to 'destination'*/

	pair<short, short> origin = x->pos, capture = x->capture;

	board[origin.first*height + origin.second] = " ";
	board[destination.first*height + destination.second] = symbols[player];

	Crowns[make_tuple(player, destination)] = Crowns[make_tuple(player, origin)];
	Crowns[make_tuple(player, capture)] = false; // What has been captured is not a crown anymore

	switch (player)
	{
	case 1:

		Crowns[make_tuple(player, destination)] = Crowns[make_tuple(player,origin)];
		if (destination.first == height-1)
		{
			Crowns[make_tuple(player,destination)] = true;
		}
	case 2:

		if (destination.first==0)
		{
			Crowns[make_tuple(player, destination)] = true;
		}
	default:
		cout << "Invalid player id entered" << endl;
		break;
	}
}


//template crownNode *dame::getPossibleMoves(crownNode*, vector<string>&);
//template Node *dame::getPossibleMoves(Node*, vector<string>&);
