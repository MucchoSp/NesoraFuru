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
	writeWavFile("FuruOut.wav", furu.Synthesize(""), 48000);
}
