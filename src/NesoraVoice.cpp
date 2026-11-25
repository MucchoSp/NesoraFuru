#include "NesoraFuru.h"

using namespace nesora;



NesoraSynthesizeVector NesoraFuruConstantData::GenerateVector(double len) const {
    NesoraSynthesizeVector out;

    for (const auto& vec : vectors) {
        if (vec.time < 0.0)
            out.AddVowelVector(len + vec.time, vec.v);
        else
            out.AddVowelVector(vec.time, vec.v);
    }

    for (const auto& vol : volumes) {
        if (vol.time < 0.0)
            out.AddVolume(len + vol.time, vol.v);
        else
            out.AddVolume(vol.time, vol.v);
    }

    for (const auto& fric : fricatives) {
        if (fric.time < 0.0)
            out.AddFricative(len + fric.time, fric.v);
        else
            out.AddFricative(fric.time, fric.v);
    }

    for (const auto& freq : frequencys) {
        if (freq.time < 0.0)
            out.AddFrequency(len + freq.time, freq.v);
        else
            out.AddFrequency(freq.time, freq.v);
    }

    return out;
}






void NesoraFuruVoice::LoadVoice(std::string filepath) {
    std::ifstream stream(filepath);

    if (!stream.is_open()) {
        std::cout << "ファイルを開くのに失敗しちゃったよ" << std::endl;
        return;
    }

    if (!nlohmann::json::accept(stream, true)) {
        std::cout << "jsonのフォーマットが不正で読み込めなかったよ" << std::endl;
        return;
    }

    stream.seekg(0, std::ios::beg);

	nlohmann::json json = nlohmann::json::parse(stream);

    if (!json.contains("name")) {
        std::cout << "nameがないよ" << std::endl;
        return;
    }
    if (!json.contains("version")) {
        std::cout << "versionがないよ" << std::endl;
        return;
    }
    if (!json.contains("f0")) {
        std::cout << "f0がないよ" << std::endl;
        return;
    }
    if (!json.contains("formants")) {
        std::cout << "formantsがないよ" << std::endl;
        return;
    }
    if (!json.contains("consonants")) {
        std::cout << "consonantsがないよ" << std::endl;
        return;
    }

    data.name = json.at("name").get<std::string>();
    data.version = json.at("version").get<std::string>();
    data.f0 = json.at("f0").get<double>();

    std::array<std::string, 8> vowel_list = { "u","i","A","a","m","m","n","n" };
    for (int i = 0;i < vowel_list.size();i++) {
        const size_t formantNum = json.at("formants").at(vowel_list[i]).size();
        data.voice.vowelFormantCube[i].buffer.resize(formantNum);
        for (size_t j = 0;j < formantNum;j++) {
            data.voice.vowelFormantCube[i].buffer[j].f = json.at("formants").at(vowel_list[i])[j].at("frequency").get<double>();
            data.voice.vowelFormantCube[i].buffer[j].h = json.at("formants").at(vowel_list[i])[j].at("intencity").get<double>();
            data.voice.vowelFormantCube[i].buffer[j].w = json.at("formants").at(vowel_list[i])[j].at("width").get<double>();
        }
    }

    data.constants = json.at("consonants").get<std::map<std::string, NesoraFuruConstantData>>();

    //std::cout << "name:" << data.name << std::endl << "version:" << data.version << std::endl << "f0:" << data.f0 << std::endl;
    //std::cout << "formants" << std::endl;
    //for (int i = 0;i < vowel_list.size();i++) {
    //    std::cout << vowel_list[i] << " :" << std::endl;
    //    for (int j = 0;j < data.voice.vowelFormantCube[i].buffer.size();j++) {
    //        std::cout << "  (" << data.voice.vowelFormantCube[i].buffer[j].f << ", " << data.voice.vowelFormantCube[i].buffer[j].h << ", " << data.voice.vowelFormantCube[i].buffer[j].w << ")" << std::endl;
    //    }
    //}
    //std::cout << "constants" << std::endl;
    //for (const auto& cons : data.constants) {
    //    std::cout << cons.first << " :" << std::endl;
    //    for (int j = 0;j < cons.second.volumes.size();j++) {
    //        std::cout << "  (" << cons.second.volumes[j].time << ", " << cons.second.volumes[j].v << "), ";
    //    }
    //    std::cout << std::endl;
    //}
    
}

void NesoraFuruVoice::SetNesoraFuru() {

}



NesoraTimeArray<NesoraSynthesizeVowelVector> NesoraFuruVoice::GenerateFromants(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency) {
    NesoraTimeArray<NesoraSynthesizeVowelVector> formants;
    const size_t paddingTime = 44;

    formants[0] = NesoraVowelVectorConstant::A;
    for (const auto& p : segments) {
        if (p.text == "a" or p.text == "A") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::A;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::A;
        }
        else if (p.text == "i" or p.text == "I") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::I;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::I;
        }
        else if (p.text == "u" or p.text == "U") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::U;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::U;
        }
        else if (p.text == "e" or p.text == "E") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::E;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::E;
        }
        else if (p.text == "o" or p.text == "O") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::O;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::O;
        }
        else if (p.text == "N") {
            formants[p.begin_frame * frame_size] = NesoraVowelVectorConstant::nasalN;
            formants[p.end_frame * frame_size - paddingTime] = NesoraVowelVectorConstant::nasalN;
        }
        else if (p.text == "pau" or p.text == "sil" or p.text == "cl") {
        }
        else {
            const auto constant = data.constants.find(p.text);

            if (constant == data.constants.end()) {
                std::cout << p.text << " を何て読めばいいか分からないかも" << std::endl;
            }
            else {
                for (const auto& vec : (*constant).second.vectors) {
                    if (vec.time < 0.0)
                        formants[p.end_frame * frame_size + vec.time * sampling_frequency] = vec.v;
                    else
                        formants[p.begin_frame * frame_size + vec.time * sampling_frequency] = vec.v;
                }
            }
        }
    }

    return formants;
}


NesoraTimeArray<double> NesoraFuruVoice::GenerateFricatives(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency, double fric) {
    NesoraTimeArray<double> fricatives;
    const size_t paddingTime = 88;

    fricatives[0] = 0;
    for (const auto& p : segments) {
        if (p.text == "a" or p.text == "i" or p.text == "u" or p.text == "e" or p.text == "o" or p.text == "N") {
            fricatives[p.begin_frame * frame_size] = fric;
            fricatives[p.end_frame * frame_size - paddingTime] = fric;
        }
        else if (p.text == "A" or p.text == "I" or p.text == "U" or p.text == "E" or p.text == "O") {
            fricatives[p.begin_frame * frame_size] = 1.0;
            fricatives[p.end_frame * frame_size - paddingTime] = 1.0;
        }
        else if (p.text == "pau" or p.text == "sil" or p.text == "cl") {
        }
        else {
            const auto constant = data.constants.find(p.text);

            if (constant == data.constants.end()) {
                std::cout << p.text << " を何て読めばいいか分からないかも" << std::endl;
            }
            else {
                for (const auto& vec : (*constant).second.fricatives) {
                    if (vec.time < 0.0)
                        fricatives[p.end_frame * frame_size + vec.time * sampling_frequency] = vec.v;
                    else
                        fricatives[p.begin_frame * frame_size + vec.time * sampling_frequency] = vec.v;
                }
            }
        }
    }

    return fricatives;
}

NesoraTimeArray<double> NesoraFuruVoice::GenerateVolumes(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency, double volume) {
    NesoraTimeArray<double> volumes;
    const size_t paddingTime = 88;

    volumes[0] = 0;
    for (const auto& p : segments) {
        if (p.text == "a" or p.text == "i" or p.text == "u" or p.text == "e" or p.text == "o" or p.text == "N") {
            volumes[p.begin_frame * frame_size] = volume;
            volumes[p.end_frame * frame_size - paddingTime] = volume;
        }
        else if (p.text == "A" or p.text == "I" or p.text == "U" or p.text == "E" or p.text == "O") {
            volumes[p.begin_frame * frame_size] = volume;
            volumes[p.end_frame * frame_size - paddingTime] = volume;
        }
        else if (p.text == "pau" or p.text == "sil" or p.text == "cl") {
            volumes[p.begin_frame * frame_size] = 0;
            volumes[p.end_frame * frame_size - paddingTime] = 0;
        }
        else {
            const auto constant = data.constants.find(p.text);

            if (constant == data.constants.end()) {
                std::cout << p.text << " を何て読めばいいか分からないかも" << std::endl;
            }
            else {
                for (const auto& vec : (*constant).second.volumes) {
                    if (vec.time < 0.0)
                        volumes[p.end_frame * frame_size + vec.time * sampling_frequency] = vec.v * volume;
                    else
                        volumes[p.begin_frame * frame_size + vec.time * sampling_frequency] = vec.v * volume;
                }
            }
        }
    }

    return volumes;
}


NesoraSynthesizeVector NesoraFuruVoice::GenerateSynthesizeVector(NesoraProsodyData r) {
    NesoraSynthesizeVector svec;
    svec.SetVoice(data.voice);


    int len = r.f0.size() * r.frame_size;
    svec.AddFrequency(0, data.f0);
    for (size_t i = 0; i < r.f0.size(); i++) {
        if (r.f0[i] == 0)
            svec.AddFrequency(i * r.frame_size, data.f0);
        else
            svec.AddFrequency(i * r.frame_size, r.f0[i]);
    }

    svec.SetFricatives(GenerateFricatives(r.phones, r.frame_size, (double)r.frame_size / r.frame_seconds, 0.5));
    svec.SetVolumes(GenerateVolumes(r.phones, r.frame_size, (double)r.frame_size / r.frame_seconds, 0.2));
    svec.SetVowelVectors(GenerateFromants(r.phones, r.frame_size, (double)r.frame_size / r.frame_seconds));

    return svec;
}

