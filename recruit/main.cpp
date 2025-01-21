#include"question.h"
#include"exam_mathematics.h"
#include"exam_japanese.h"
#include"exam_english.h"
#include"exam_science.h"
#include"exam_geography.h"
#include"exam_politics.h"
#include"exam_economics.h"
#include"utility.h"
#include<iostream>
#include<string>
#include<vector>
#include<random>
using namespace std;

int main()
{
	vector<Question> questions(3);

	cout << "[リクルート試験対策クイズ]\n";

	cout << "教材を選んでください\n1=数学\n2=国語\n3=英語\n4=理科\n5=地理\n6=政治\n7=経済\n";
	int subject;
	cin >> subject;
	if (subject == 1)
	{
		questions = CreateMathematicsExam();
	}
	else if (subject == 2)
	{
		questions = CreateKanjiExam();
		const QuestionList idiomExam = CreateIdiomExam();
		questions.insert(questions.end(), idiomExam.begin(), idiomExam.end());
		QuestionList homophoneExam = CreateHomophoneExam();
		questions.insert(questions.end(), homophoneExam.begin(), homophoneExam.end());
		QuestionList antonymExam = CreateAntonymExam();
		questions.insert(questions.end(), antonymExam.begin(), antonymExam.end());
		QuestionList synonyExam = CreateSynonymExam();
		questions.insert(questions.end(), synonyExam.begin(), synonyExam.end());
	}
	else if(subject == 3)
	{
		questions = CreatEnglishWordExam();
		QuestionList phraseExam = CreateEnglishPhraseExam();
		questions.insert(questions.end(), phraseExam.begin(), phraseExam.end());
	}
	else if (subject == 4)
	{
		questions = CreatePhysicsExam();
	}
	else if (subject == 5) {
		questions = CreatePrefecturesExam();
	}
	else if (subject == 6) {
		questions = CreatePoliticsExam(); 
	}
	else if (subject == 7) {
		questions = CreateEconomicsExam();
	}

	for (const auto& e : questions) {
		cout << e.q << "\n";
		string answer;
		cin >> answer;

		const string ascii = ConvertSjisNumberToAscii(answer);
		// 変換が成功した場合は文字列を置き換える
		if (!ascii.empty()) {
			answer = ascii;
		}

		if (answer == e.a) {
			cout << "正解！\n";
		}
		else if(e.b.empty())
		{
			// 答えが一つだけの場合
			cout << "間違い！正解は" << e.a << "\n";
		}
		else
		{
			// 答えが複数ある場合、いずれかと一致すれば正解とする
			bool isMatch = false;
			for (const auto& b : e.b) {
				if (answer == b) {
					isMatch = true;	// 一致する答えが見つかった
					break;
				}
			}

			// 比較結果を出力
			if (isMatch) {
				cout << "正解!\n";
			}
			else {
				cout << "間違い!　正解は" << e.a << "(または";
				for (auto& b : e.b) {
					cout << "、" << b;
				}
				cout << ")\n";
			}
		} // if answer == e.a
	} //for questions
}
