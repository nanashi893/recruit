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
#include<fstream>
#include<time.h>
using namespace std;

int main()
{
	// 教科データ
	static const struct {
		const char* name;		// 教科名
		QuestionList(*create)();// 問題作成のアドレス
	} subjectData[] = {
		{ "数学", CreateMathematicsExam },
		{ "国語", CreateJapaneseExam },
		{ "英語", CreateEnglishExam },
		{ "物理", CreatePhysicsExam },
		{ "地理", CreateGeographyExam },
		{ "政治", CreatePoliticsExam },
		{ "経済", CreateEconomicsExam },
	};

	vector<Question> questions(3);

	cout << "[リクルート試験対策クイズ]\n";

	cout << "教材を選んでください\n";
	cout << "0=総合テスト\n";
	for (int i = 0; i < size(subjectData); i++)
	{
		cout << i + 1 << '=' << subjectData[i].name << '\n';
	}

	vector<int> questionCounts(size(subjectData));	// 各教科の問題数
	int subject;
	cin >> subject;
	if (subject > 0 && subject <= size(subjectData)) {
		questions = subjectData[subject - 1].create();
	}
	else if (subject == 0)
	{
		// 総合テスト
		questions.clear();
		for (int i = 0; i < size(subjectData); i++) {
			QuestionList tmp = subjectData[i].create();
			questions.insert(questions.end(), tmp.begin(), tmp.end());
			questionCounts[i] = (int)tmp.size();
		}
	}

	vector<int> correctCounts(size(subjectData));	// 各教科の正答数
	int currentSubjectNo = 0;		// 出題中の強化番号
	int currentAnsweredCount = 0;	// 回答済みの問題数
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
			correctCounts[currentSubjectNo]++;	// 正答数を増やす
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
				correctCounts[currentSubjectNo]++;	// 正答数を増やす
			}
			else {
				cout << "間違い!　正解は" << e.a << "(または";
				for (auto& b : e.b) {
					cout << "、" << b;
				}
				cout << ")\n";
			}
		} // if answer == e.a

		// 「回答済み問題数」が「教科の問題数」以上になったら、次の教科に進む
		if (subject == 0) {
			currentAnsweredCount++;	// 回答済み問題数を増やす
			if (currentAnsweredCount >= questionCounts[currentSubjectNo]) {
				currentSubjectNo++;	// 次の教科に進む
				currentAnsweredCount = 0;	// 回答済み問題数をリセット
			}
		} // if subject == 0
	} //for questions

	// 成績を表示
	cout << "\n--- 成績 ---\n";
	if (subject > 0 && subject <= size(subjectData)) {
		cout << subjectData[subject - 1].name << ":"
			<< correctCounts[0] << '/' << questions.size() << '\n';
	} else if (subject == 0) {
		// 教科ごとの成績を表示しつつ、正答の合計を計算
		size_t totalCorrectCount = 0;	//正答数の合計
		for (int i = 0; i < size(subjectData); i++)	{
			cout << subjectData[i].name << "; "
				<< correctCounts[i] << '/' << questionCounts[i] << '\n';
		}
		cout << "合計: " << totalCorrectCount << '/' << questions.size() << '\n';
	}

	// 成績をファイルに出力する
	static const char filename[] = "リクルート対策試験成績表.txt";
	ofstream ofs(filename, ios_base::app);
	if (!ofs) {
		cerr << "エラー:" << filename << "を開けません\n";
	} else {
		// 現在の時刻(協定世界時)を取得
		const time_t t = time(nullptr);

		// 協定世界時を時間構造体型に変換
		tm examDate;
		localtime_s(&examDate, &t);

		// 時間構造体を文字列に変換
		char strDate[100];
		strftime(strDate, size(strDate), "%Y/%m/%d(%a) %T", &examDate);

		if (subject > 0 && subject <= size(subjectData)) {
			// 教科テストの場合、試験した成績だけを出力し、それ以外は空欄とする
			ofs << strDate;
			for (int i = 0; i < size(subjectData); i++) {
				ofs << ',';
				if (i == subject - 1) {
					ofs << correctCounts[0] << '/' << questions.size();
				}
			}
			ofs << "\n";
			cout << "成績を" << filename << "に出力しました\n";
		} else if (subject == 0) {
			// 総合テストの場合、全ての教科の成績を出力する
			ofs << strDate;
			for (int i = 0; i < size(subjectData); i++)	{
				ofs << ',' << correctCounts[i] << '/' << questionCounts[i];
			}
			ofs << "\n";
			cout << "成績を" << filename << "に出力しました\n";
		}
	} // if !ofs
}
