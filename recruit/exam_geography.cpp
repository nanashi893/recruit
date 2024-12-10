#include "exam_geography.h"
#include "utility.h"
#include <iostream>
#include <fstream>
#include <random>
using namespace std;

/**
* “s“¹•{Œ§‚Ì–â‘è‚ğì¬‚·‚é
**/
QuestionList CreatePrefecturesExam()
{
	// “s“¹•{Œ§ƒf[ƒ^
	struct PrefectureData
	{
		string name;		//“s“¹•{Œ§–¼
		string capital;		//Œ§’¡Šİ’n
		vector<string> features;	//“Á’¥
	};
	vector<PrefectureData> data;

	{ // “s“¹•{Œ§ƒf[ƒ^‚ğ“Ç‚İ‚Ş
		constexpr char filename[] = "japanese_prefectures.txt";
		ifstream ifs(filename);
		if (!ifs) {
			cerr << "ƒGƒ‰[: " << filename << "‚ğ“Ç‚İ‚ß‚Ü‚¹‚ñ\n";
			return {};
		}

		string s;
		while (true) {
			getline(ifs, s); // 1s“Ç‚İæ‚é
			if (!ifs) {
				break; // ƒtƒ@ƒCƒ‹I’[‚É’B‚µ‚½‚Ì‚ÅI—¹
			}
			const vector<string> v = Split(s, ',');
			data.push_back({ v[0], v[1], vector<string>(v.begin() + 2, v.end()) });
		}
	} // “s“¹•{Œ§ƒf[ƒ^‚ğ“Ç‚İ‚Ş

	constexpr int quizCount = 5;
	QuestionList questions;
	questions.reserve(quizCount);
	vector<int> indices = CreateRandomIndices((int)data.size());
	random_device rd;

	const int type = 0;//uniform_int_distribution<>(0, 2)(rd);
	switch (type) {
	case 0: //	“Á’¥‚©‚ç“s“¹•{Œ§‚ğ“š‚¦‚é
		for (int i = 0; i < quizCount; i++) {
			// ³‰ğˆÈŠO‚Ì”Ô†‚Åã‘‚«
			const int correctIndex = indices[i];
			vector<int> answers = CreateWrongIndices((int)data.size(), correctIndex);

			// ƒ‰ƒ“ƒ_ƒ€‚ÈˆÊ’u‚ğ³‚µ‚¢”Ô†‚Åã‘‚«
			const int correctNo = std::uniform_int_distribution<>(1, 3)(rd);
			answers[correctNo - 1] = correctIndex;

			// –â‘è•¶‚ğì¬
			string s = "ˆÈ‰º‚Ì“Á’¥‚ğ‚Â“s“¹•{Œ§‚Ì”Ô†‚ğ‘I‚×\n";

			// “s“¹•{Œ§‚Ì“Á’¥‚ğƒ‰ƒ“ƒ_ƒ€‚È‡”Ô‚Å˜AŒ‹‚µA–â‘è•¶‚É’Ç‰Á
			const auto& features = data[correctIndex].features;
			vector<int> featureIndices = CreateRandomIndices((int)features.size());
			s += features[featureIndices[0]];
			for (int j = 1; j < 3; j++) {
				s += ", ";
				s += features[featureIndices[j]];
			}

			//“š‚¦‚Æ‚È‚é“s“¹•{Œ§‚ğ–â‘è•¶‚É’Ç‰Á
			for (int j = 0; j < 3; j++) {
				s += "\n " + to_string(j + 1) + ":" + data[answers[j]].name;
			}

			// –â‘è‚Æ“š‚¦‚ÌƒyƒA‚ğ–â‘è”z—ñ‚É’Ç‰Á
			questions.push_back({ s, to_string(correctNo) });
		}
		break;

	case 1: // “s“¹•{Œ§‚ğ•\‚·“Á’¥‚ğ“š‚¦‚é
		break;

	case 2: // “s“¹•{Œ§‚©‚çŒ§’¡Šİ’n‚ğ“š‚¦‚é
		break;
	} // switch (type)

	return questions;
}