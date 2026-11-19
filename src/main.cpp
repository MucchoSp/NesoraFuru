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
	writeWavFile("FuruOutA.wav", furu.Synthesize("a"), 48000);
	writeWavFile("FuruOutI.wav", furu.Synthesize("i"), 48000);
	writeWavFile("FuruOutU.wav", furu.Synthesize("u"), 48000);
	writeWavFile("FuruOutE.wav", furu.Synthesize("e"), 48000);
	writeWavFile("FuruOutO.wav", furu.Synthesize("o"), 48000);

#ifdef _WIN32
	PlaySoundA("FuruOutA.wav", NULL, SND_FILENAME | SND_SYNC);
	PlaySoundA("FuruOutI.wav", NULL, SND_FILENAME | SND_SYNC);
	PlaySoundA("FuruOutU.wav", NULL, SND_FILENAME | SND_SYNC);
	PlaySoundA("FuruOutE.wav", NULL, SND_FILENAME | SND_SYNC);
	PlaySoundA("FuruOutO.wav", NULL, SND_FILENAME | SND_SYNC);
#endif
}
