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
			std::cout << std::setw(2) << board[i][j];
		}
		std::cout << std::endl;
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
	std::vector<std::vector<std::string>> prefixTrees;
	for (unsigned int i = 0; i < board.size(); i++)
	{
		prefixTrees.resize(3);
		for (unsigned int j = 0; j < board.size(); j++)
		{
			boggleHelper(dict, prefix, prefixTrees[0], board, "", result, i, j, 0, 1);
			boggleHelper(dict, prefix, prefixTrees[1], board, "", result, i, j, 1, 0);
			boggleHelper(dict, prefix, prefixTrees[2], board, "", result, i, j, 1, 1);
		}
	}

	return result;
}

bool boggleHelper(const std::set<std::string> &dict, const std::set<std::string> &prefix, std::vector<std::string> &prefixTree, std::vector<std::vector<char>> &board,
				  std::string word, std::set<std::string> &result, unsigned int r, unsigned int c, int dr, int dc)
{
	// add your solution here!
	char letter = board[r][c];				 // cube letter
	char origL = letter;					 // original cube string
	std::string currentWord = word + letter; // build word

	// word found
	std::cout << "------\ncurrent letter: " << letter << std::endl;
	std::cout << currentWord << std::endl;
	if (currentWord.length() > 1 && dict.find(currentWord) != dict.end())
	{
		// check for prefixes in results and remove
		prefixTree.push_back(currentWord);
	}

	// stop if the prefix isn't valid
	if (currentWord.length() > 1 && prefix.find(currentWord) == prefix.end())
	{
		return false;
	}

	if (currentWord.length() == board.size())
		return false;

	if(currentWord.length() == 1) {
		prefixTree.clear();
	}

 	if (dr != 0 && dc == 0)
	{
		// vertical case
		std::cout << "vert case" << std::endl;
		std::cout << "horizontal direction: " << dc << std::endl;
		std::cout << "vertical direction: " << dr << std::endl;
		if (r + dr >= 0 && r + dr < board.size() && board[r + dr][c + dc] != '-')
		{
			board[r][c] = '-'; // used cube
			if (boggleHelper(dict, prefix, prefixTree, board, currentWord, result, r + dr, c, dr, dc))
			{
				std::cout << "prefix count: " << prefixTree.size() << std::endl;
				return true;
			}
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}
	else if (dc != 0 && dr == 0)
	{
		// horizontal case
		std::cout << "horiz case" << std::endl;
		std::cout << "horizontal direction: " << dc << std::endl;
		std::cout << "vertical direction: " << dr << std::endl;
		if (c + dc >= 0 && c + dc < board.size() && board[r + dr][c + dc] != '-')
		{
			board[r][c] = '-'; // used cube
			if (boggleHelper(dict, prefix, prefixTree, board, currentWord, result, r, c + dc, dr, dc))
			{
				std::cout << "prefix count: " << prefixTree.size() << std::endl;
				return true;
			}
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}
	else if (dc != 0 && dr != 0)
	{
		// diagonal case
		std::cout << "diagonal case" << std::endl;
		std::cout << "horizontal direction: " << dr << std::endl;
		std::cout << "vertical direction: " << dc << std::endl;
		if (c + dc >= 0 && c + dc < board.size() && r + dr >= 0 && r + dr < board.size() && board[r + dr][c + dc] != '-')
		{
			board[r][c] = '-'; // used cube
			if (boggleHelper(dict, prefix, prefixTree, board, currentWord, result, r + dr, c + dc, dr, dc))
			{
				std::cout << "prefix count: " << prefixTree.size() << std::endl;
				return true;
			}
			board[r][c] = origL; // reset cube for different letter arrangement
		}
	}

	if(prefixTree.size() > 0) {
		result.insert(prefixTree.back());
	}
	return false;
}
