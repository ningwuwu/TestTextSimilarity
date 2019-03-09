#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <wchar.h>
#include <unordered_set>
#include <cppjieba/jieba.hpp>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

class TestTextSimilarity
{
public:
	
	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> wordSet;
	TestTextSimilarity(std::string dict);
	void getStopWordTable(const char* stopWordFile);
	wordFreq getWordFreq(const char* filename);

	std::string UTF8TOGBK(std::string str);
	std::string GBKTOUTF8(std::string str);
	std::vector<std::pair<std::string, int>> sortByValueReverse(wordFreq& wf);
	void selectTopWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet& wset);
	std::vector<double> getOneHot(wordSet& wset, wordFreq& wf);
	double cosine(std::vector<double> oneHot1, std::vector<double> oneHot2);
private:
	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;
	wordSet _stopWordSet;
	int _maxWordNumber;
};
