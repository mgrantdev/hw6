#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>
#include <exception>

#include "boggle.h"

std::vector<std::vector<char>> genBoard(unsigned int n, int seed)
{
	// random number generator
	std::mt19937 r(seed);

	// scrabble letter frequencies
	// A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2,
	// N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
	int freq[26] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1};
	std::vector<char> letters;
	for (char c = 'A'; c <= 'Z'; c++)
	{
		for (int i = 0; i < freq[c - 'A']; i++)
		{
			letters.push_back(c);
		}
	}
	std::vector<std::vector<char>> board(n);
	for (unsigned int i = 0; i < n; i++)
	{
		board[i].resize(n);
		for (unsigned int j = 0; j < n; j++)
		{
			board[i][j] = letters[(r() % letters.size())];
		}
	}
	return board;
}

void printBoard(const std::vector<std::vector<char>> &board)
{
	unsigned int n = board.size();
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			//std::cout << std::setw(2) << board[i][j];
		}
		//std::cout << std::endl;
	}
}

std::pair<std::set<std::string>, std::set<std::string>> parseDict(std::string fname)
{
	std::ifstream dictfs(fname.c_str());
	if (dictfs.fail())
	{
		throw std::invalid_argument("unable to open dictionary file");
	}
	std::set<std::string> dict;
	std::set<std::string> prefix;
	std::string word;
	while (dictfs >> word)
	{
		dict.insert(word);
		for (unsigned int i = word.size() - 1; i >= 1; i--)
		{
			prefix.insert(word.substr(0, i));
		}
	}
	prefix.insert("");
	return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string> &dict, const std::set<std::string> &prefix, std::vector<std::vector<char>> &board)
{
	std::set<std::string> result;
	for (unsigned int i = 0; i < board.size(); i++)
	{
		for (unsigned int j = 0; j < board.size(); j++)
		{
			boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);
		}
	}

	return result;
}

bool boggleHelper(const std::set<std::string> &dict, const std::set<std::string> &prefix, std::vector<std::vector<char>> &board,
				  std::string word, std::set<std::string> &result, unsigned int r, unsigned int c, int dr, int dc)
{
	// add your solution here!
	char letter = board[r][c];				 // cube letter
	char origL = letter;					 // original cube string
	std::string currentWord = word + letter; // build word

	// word found
	//std::cout << "current letter: " << letter << std::endl;
	//std::cout << currentWord << std::endl;
	if (currentWord.length() > 1 && dict.find(currentWord) != dict.end())
	{
		// check for prefixes in results and remove
		std::set<std::string> resultCpy = result;
		if(currentWord.length() > 2) {
			for(std::set<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
				if (currentWord.find(*it) != std::string::npos) {
					resultCpy.erase(*it);
				}
			}
		}
		result = resultCpy;
		result.insert(currentWord);
	}

	// stop if the prefix isn't valid
	if (currentWord.length() > 1 && prefix.find(currentWord) == prefix.end())
	{
		return false;
	}

	if (currentWord.length() == board.size())
		return false;

	// check all neighbor cubes if no direction specified
	if (dr == 0 && dc == 0)
	{
		for (int vert = 0; vert <= 1; vert++)
		{
			for (int horiz = 0; horiz <= 1; horiz++)
			{
				//std::cout << "horizontal direction: " << horiz << std::endl;
				//std::cout << "vertical direction: " << vert << std::endl;
				// watch for grid borders and used cubes
				if (r + vert >= 0 && r + vert < board.size() &&
					c + horiz >= 0 && c + horiz < board[0].size() &&
					board[r + vert][c + horiz] != '-' &&
					!(horiz == 0 && vert == 0) &&
					currentWord.length() < board.size()
				)
				{
					board[r][c] = '-'; // used cube
					//std::cout << "trying neighbors" << std::endl;
					if (boggleHelper(dict, prefix, board, currentWord, result, r + vert, c, vert, horiz) || boggleHelper(dict, prefix, board, currentWord, result, r, c + horiz, vert, horiz) || boggleHelper(dict, prefix, board, currentWord, result, r + vert, c + horiz, vert, horiz))
						return true;
					board[r][c] = origL; // reset cube for different letter arrangement
				}
			}
		}
	}
	else if (dr != 0 && dc == 0)
	{
		// horizontal case
		if (r + dr >= 0 && r + dr < board.size())
		{
			if (boggleHelper(dict, prefix, board, currentWord, result, r + dr, c, dr, dc))
				return true;
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}
	else if (dc != 0 && dr == 0)
	{
		// vertical case
		if (c + dc >= 0 && c + dc < board.size())
		{
			if (boggleHelper(dict, prefix, board, currentWord, result, r, c + dc, dr, dc))
				return true;
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}
	else if(dc != 0 && dr != 0)
	{
		// diagonal case
		//std::cout << "diagonal case" << std::endl;
		if (c + dc >= 0 && c + dc < board.size() && r + dr >= 0 && r + dr < board.size())
		{
			if (boggleHelper(dict, prefix, board, currentWord, result, r + dr, c + dc, dr, dc))
				return true;
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}
	return false;
}
