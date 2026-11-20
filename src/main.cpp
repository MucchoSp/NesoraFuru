// Im going to create a new Nesora Furu!!!
//

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif

#include "NesoraFuru.h"

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	std::cout << "Welcome to Nesora0-4" << std::endl;

	NesoraFuru::NesoraFuru furu;
	furu.LoadVoice("../../../NesoraFuru.json");
	//writeWavFile("FuruOutA.wav", furu.Synthesize("a"), 48000);
//	writeWavFile("FuruOutI.wav", furu.Synthesize("i"), 48000);
//	writeWavFile("FuruOutU.wav", furu.Synthesize("u"), 48000);
//	writeWavFile("FuruOutE.wav", furu.Synthesize("e"), 48000);
//	writeWavFile("FuruOutO.wav", furu.Synthesize("o"), 48000);
//
//#ifdef _WIN32
//	PlaySoundA("FuruOutA.wav", NULL, SND_FILENAME | SND_SYNC);
//	PlaySoundA("FuruOutI.wav", NULL, SND_FILENAME | SND_SYNC);
//	PlaySoundA("FuruOutU.wav", NULL, SND_FILENAME | SND_SYNC);
//	PlaySoundA("FuruOutE.wav", NULL, SND_FILENAME | SND_SYNC);
//	PlaySoundA("FuruOutO.wav", NULL, SND_FILENAME | SND_SYNC);
//#endif

	//NesoraFuru::NesoraAnalyzer analyzer("D:\\open_jtalk_dic_utf_8-1.11", "D:\\MMDAgent_Example-1.8\\Voice\\mei\\mei_normal.htsvoice");
	//auto r = analyzer.Analyze("例えばこういう日本語の文章を音諳が読み上げられるようにするために形態素解析をする必要があるんだけど、自前で作るのはありえん大変なのでOpenJTalkを使ったってわけ。", 1.0);

	//for (const auto& p : r.phones) {
	//	std::cout << p.text << ": [" << p.begin_frame << ", " << p.end_frame << ")" << std::endl;
	//}

	//std::ofstream ofs_csv_file("test_output.csv");
	//for (std::size_t i = 0; i < r.f0.size(); ++i)
	//	ofs_csv_file << i << ", " << r.f0[i] << std::endl;

	writeWavFile("FuruOut.wav", furu.Synthesize("例えば、こういう日本語の文章を読み上げられるようにするために形態素解析をする必要があるんだけど、自前で作るのはありえん大変なので、OpenJTalkを使ったってわけ。"), 48000);

	//NesoraFuru::NesoraFuru shion;
	//shion.LoadVoice("../../../NesoraShion.json");
	//writeWavFile("ShionOut.wav", shion.Synthesize("例えば、こういう日本語の文章を読み上げられるようにするために形態素解析をする必要があるんだけど、自前で作るのはありえん大変なので、OpenJTalkを使ったってわけ。"), 48000);

}
