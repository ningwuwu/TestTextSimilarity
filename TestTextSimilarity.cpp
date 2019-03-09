#define _CRT_SECURE_NO_WARNINGS
#include "TestTextSimilarity.h"
using namespace std;




TestTextSimilarity::TestTextSimilarity(string dict)
	: DICT(dict)
	, DICT_PATH(dict + "/jieba.dict.utf8")
	, HMM_PATH(dict + "/hmm_model.utf8")
	, USER_DICT_PATH(dict + "/user.dict.utf8")
	, IDF_PATH(dict + "/idf.utf8")
	, STOP_WORD_PATH(dict + "/stop_words.utf8")
	, _maxWordNumber(100)
	, _jieba(DICT_PATH,
	HMM_PATH,
	USER_DICT_PATH,
	IDF_PATH,
	STOP_WORD_PATH)

{
	getStopWordTable(STOP_WORD_PATH.c_str());
}
//获取词频
TestTextSimilarity::wordFreq TestTextSimilarity::getWordFreq(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "open file: " << filename << "failed!" << endl;
		return wordFreq();
	}
	string line;
	wordFreq wf;
	while (!fin.eof())
	{
		getline(fin, line);
		line = GBKTOUTF8(line);
		vector<string> words;
		//对当前文本进行分词
		_jieba.Cut(line, words, true);
		//统计词频
		for (const auto& e : words)
		{
			//去掉停用词
			if (_stopWordSet.count(e) > 0)
				continue;
			else{
				if (wf.count(e) > 0)
					wf[e]++;
				else
					wf[e] = 1;
			}
		}
	}
	return wf;
}
bool cmpReverse(pair<string, int>lp, pair<string, int>rp)
{
	return lp.second > rp.second;
}
vector<pair<string, int>> TestTextSimilarity::sortByValueReverse(TestTextSimilarity::wordFreq& wf)
{
	//map是<string, value>类型，转成vector类型排序
	vector<pair<string, int>> wfvector(wf.begin(), wf.end());
	sort(wfvector.begin(), wfvector.end(), cmpReverse);
	return wfvector;
}
string TestTextSimilarity::GBKTOUTF8(string ws)
{
	int len = MultiByteToWideChar(CP_ACP, 0, ws.c_str(), -1, nullptr, 0);
	wchar_t* wchar = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, ws.c_str(), -1, wchar, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
	char* schar = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, wchar, -1, schar, len, nullptr, nullptr);
	string tmp = schar;
	if (wchar)
	{
		delete[] wchar;
		wchar = nullptr;
	}
	if (schar)
	{
		delete[] schar;
		schar = nullptr;
	}
	return tmp;
}

string TestTextSimilarity::UTF8TOGBK(string ms)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, ms.c_str(), -1, nullptr, 0);
	wchar_t* wchar = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, ms.c_str(), -1, wchar, len);
	len = WideCharToMultiByte(CP_ACP, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
	char* schar = new char[len];
	WideCharToMultiByte(CP_ACP, 0, wchar, -1, schar, len, nullptr, nullptr);
	string tmp = schar;
	if (wchar)
	{
		delete[] wchar;
		wchar = nullptr;
	}
	if (schar)
	{
		delete[] schar;
		schar = nullptr;
	}
	return tmp;
}
//获取停用词词典
void TestTextSimilarity::getStopWordTable(const char* stopWordFile)
{
	ifstream fin(stopWordFile);
	if (!fin.is_open())
	{
		cout << "open file: " << stopWordFile << "failed!" << endl;
		return;
	}
	string line;
	while (!fin.eof())
	{
		getline(fin, line);
		_stopWordSet.insert(line);
	}
	fin.close();
}
//取出词频的前n个，存入wset
void TestTextSimilarity::selectTopWords(std::vector<std::pair<std::string, int>>& wfvec, TestTextSimilarity::wordSet& wset)
{
	int len = wfvec.size();
	int sz = len > _maxWordNumber ? _maxWordNumber : len;
	for (int i = 0; i < sz; ++i)
	{
		wset.insert(wfvec[i].first);
	}
}
//构建词频向量
vector<double> TestTextSimilarity::getOneHot(wordSet& wset, wordFreq& wf)
{
	//遍历出wordSet中每一个词
	vector<double> oneHot;
	for (const auto& e : wset)
	{
		if (wf.count(e))
			oneHot.push_back(wf[e]);
		else
			oneHot.push_back(0);
	}
	return oneHot;
}
double TestTextSimilarity::cosine(std::vector<double> oneHot1, std::vector<double> oneHot2)
{
	
	assert(oneHot1.size() == oneHot2.size());
	double modular1 = 0, modular2 = 0;
	double products = 0;
	for (int i = 0; i < oneHot1.size(); ++i)
	{
		products += oneHot1[i] * oneHot2[i];
	}
	for (int i = 0; i < oneHot1.size(); ++i)
	{
		modular1 += pow(oneHot1[i], 2);
	}
	modular1 = pow(modular1, 0.5);
	for (int i = 0; i < oneHot2.size(); ++i)
	{
		modular2 += pow(oneHot1[i], 2);
	}
	modular2 = pow(modular2, 0.5);
	return products / (modular2 * modular1);
}
