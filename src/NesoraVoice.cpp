#include "NesoraFuru.h"

using namespace NesoraFuru;

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

    data.name = json.at("name").get<std::string>();
    data.version = json.at("version").get<std::string>();
    data.f0 = json.at("f0").get<double>();
    data.formants = json.at("formants").get<std::map<std::string, std::vector<FormantParameter>>>();

    std::cout << "name:" << data.name << std::endl << "version:" << data.version << std::endl << "f0:" << data.f0 << std::endl;
    std::cout << "formants" << std::endl;
    for (auto& phoneme : data.formants) {
        std::cout << phoneme.first << " :" << std::endl;
        for (int i = 0;i < 4;i++) {
            std::cout << "  (" << phoneme.second[i].frequency << ", " << phoneme.second[i].intensity << ", " << phoneme.second[i].width << ")" << std::endl;
        }
    }
    
}

void NesoraFuruVoice::SetNesoraFuru() {

}



std::vector<NesoraFormants> NesoraFuruVoice::GenerateFromants(std::vector<NesoraSegment> segments, size_t frame_size) {
    std::vector<NesoraFormants> formants;

    formants.resize(segments.back().end_frame * frame_size, data.formants["a"]);
    for (const auto& p : segments) {
        if (p.text == "a") {
            std::fill(formants.begin() + p.begin_frame * frame_size, formants.begin() + p.end_frame * frame_size, data.formants["a"]);
        }
        else if (p.text == "i") {
            std::fill(formants.begin() + p.begin_frame * frame_size, formants.begin() + p.end_frame * frame_size, data.formants["i"]);
        }
        else if (p.text == "u") {
            std::fill(formants.begin() + p.begin_frame * frame_size, formants.begin() + p.end_frame * frame_size, data.formants["u"]);
        }
        else if (p.text == "e") {
            std::fill(formants.begin() + p.begin_frame * frame_size, formants.begin() + p.end_frame * frame_size, data.formants["e"]);
        }
        else if (p.text == "o") {
            std::fill(formants.begin() + p.begin_frame * frame_size, formants.begin() + p.end_frame * frame_size, data.formants["o"]);
        }
        else {
        }


    }

    return formants;
}

