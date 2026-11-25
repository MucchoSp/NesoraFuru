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

	auto start = std::chrono::system_clock::now();
	try {
		nesora::NesoraFuru furu;
		furu.LoadVoice("../../../NesoraFuru.json");
		writeWavFile("FuruOut.wav", furu.Synthesize("例えば、こういう日本語の文章を読み上げられるようにするために形態素解析をする必要があるんだけど、自前で作るのはありえん大変なので、OpenJTalkを使ったってわけ。"), 48000);
	}
	catch (const std::exception& e) {
		std::cerr << "エラー: " << e.what() << '\n';
	}
	auto end = std::chrono::system_clock::now();
	std::cout << "time: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0 << " ms" << std::endl;

	return 0;
}
