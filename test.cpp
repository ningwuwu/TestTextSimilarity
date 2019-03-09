#include"TestTextSimilarity.h"
using namespace std;
void getTextSimilarity()
{
	TestTextSimilarity ts("../dict");
	TestTextSimilarity::wordFreq wf = ts.getWordFreq("aaa.txt");
	TestTextSimilarity::wordFreq wf2 = ts.getWordFreq("bbb.txt");
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);
	vector<pair<string, int>> wfvec2 = ts.sortByValueReverse(wf2);
	cout << "wfvec: " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << ts.UTF8TOGBK(wfvec[i].first) << ": " << wfvec[i].second << endl;
	}
	cout << endl;
	cout << "wfvec2: " << endl;
	for (int i = 0; i < 10; ++i)
	{
		cout << ts.UTF8TOGBK(wfvec2[i].first) << ": " << wfvec2[i].second << endl;
	}
	cout << endl;
	TestTextSimilarity::wordSet wset;
	ts.selectTopWords(wfvec, wset);
	ts.selectTopWords(wfvec2, wset);
	cout << "wset: " << endl;
	for (const auto& e : wset)
	{
		cout << ts.UTF8TOGBK(e) << " ";
	}
	cout << endl;
	vector<double> oneHot = ts.getOneHot(wset, wf);
	vector<double> oneHot2 = ts.getOneHot(wset, wf2);
	cout << "oneHot: " << endl;
	for (const auto& v : oneHot)
	{
		cout << v << " ";
	}
	cout << endl;
	cout << "oneHot2: " << endl;
	for (const auto& v : oneHot2)
	{
		cout << v << " ";
	}
	cout << endl;
	cout << "cosine: " << ts.cosine(oneHot, oneHot2) << endl;
}
int main(int argc, char** argv)
{
	getTextSimilarity();
	system("pause");
	return 0;
}


