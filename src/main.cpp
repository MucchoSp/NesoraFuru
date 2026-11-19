// Im going to create a new Nesora Furu!!!
//

#ifdef _WIN32
#include <windows.h>
#endif

#include "NesoraFuru.h"

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	std::cout << "Welcome to Nesora0-4" << std::endl;

	NesoraFuru::NesoraFuru furu;
	furu.LoadVoice("../../../NesoraFuru.json");
	// writeWavFile("FuruOutA.wav", furu.Synthesize("a"), 48000);
	// writeWavFile("FuruOutI.wav", furu.Synthesize("i"), 48000);
	// writeWavFile("FuruOutU.wav", furu.Synthesize("u"), 48000);
	writeWavFile("FuruOutE.wav", furu.Synthesize("e"), 48000);
}
