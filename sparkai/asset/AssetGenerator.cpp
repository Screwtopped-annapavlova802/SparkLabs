#include "AssetGenerator.h"
#include <algorithm>

namespace SparkLabs {

AssetGenerator::AssetGenerator()
    : m_OutputDirectory(String("./output"))
    , m_IdCounter(0)
{
    m_StyleKeywords.PushBack(String("high quality"));
    m_StyleKeywords.PushBack(String("detailed"));
    m_StyleKeywords.PushBack(String("masterpiece"));
}

AssetGenerator::~AssetGenerator() {
}

StringHash AssetGenerator::GenerateUniqueId() {
    String idStr = String("asset_") + String(static_cast<int64>(++m_IdCounter));
    return StringHash(idStr.C_str());
}

GeneratedAsset AssetGenerator::GenerateCharacter(const CharacterGenerationRequest& request) {
    GeneratedAsset asset;
    asset.id = GenerateUniqueId();
    asset.name = request.name;
    asset.type = AssetType::Character;
    asset.prompt = request.basePrompt;
    asset.width = request.basePrompt.width;
    asset.height = request.basePrompt.height;

    asset.metadata.Set(StringHash("name"), Variant(request.name));
    asset.metadata.Set(StringHash("personality"), Variant(request.personality));
    asset.metadata.Set(StringHash("art_style"), Variant(request.artStyle));

    return CreatePlaceholderAsset(request.name, AssetType::Character);
}

GeneratedAsset AssetGenerator::GenerateEnvironment(const EnvironmentGenerationRequest& request) {
    GeneratedAsset asset;
    asset.id = GenerateUniqueId();
    asset.name = request.name;
    asset.type = AssetType::Environment;
    asset.prompt = request.basePrompt;
    asset.width = request.basePrompt.width;
    asset.height = request.basePrompt.height;

    asset.metadata.Set(StringHash("location"), Variant(request.location));
    asset.metadata.Set(StringHash("time_of_day"), Variant(request.timeOfDay));
    asset.metadata.Set(StringHash("weather"), Variant(request.weather));

    return CreatePlaceholderAsset(request.name, AssetType::Environment);
}

GeneratedAsset AssetGenerator::GenerateFromPrompt(const GenerationPrompt& prompt, AssetType type) {
    GeneratedAsset asset;
    asset.id = GenerateUniqueId();
    asset.name = String("Generated Asset");
    asset.type = type;
    asset.prompt = prompt;
    asset.width = prompt.width;
    asset.height = prompt.height;

    return CreatePlaceholderAsset(String("Prompted Asset"), type);
}

void AssetGenerator::SetStylePreset(const String& preset) {
    m_CurrentStylePreset = preset;
    m_StyleKeywords.Clear();

    if (preset == String("anime")) {
        m_StyleKeywords.PushBack(String("anime style"));
        m_StyleKeywords.PushBack(String("cel shaded"));
    } else if (preset == String("realistic")) {
        m_StyleKeywords.PushBack(String("photorealistic"));
        m_StyleKeywords.PushBack(String("hyperdetailed"));
    } else if (preset == String("cartoon")) {
        m_StyleKeywords.PushBack(String("cartoon style"));
        m_StyleKeywords.PushBack(String("bold lines"));
    } else if (preset == String("pixel_art")) {
        m_StyleKeywords.PushBack(String("pixel art"));
        m_StyleKeywords.PushBack(String("retro"));
    }
}

void AssetGenerator::AddStyleKeyword(const String& keyword) {
    m_StyleKeywords.PushBack(keyword);
}

void AssetGenerator::ClearStyleKeywords() {
    m_StyleKeywords.Clear();
}

bool AssetGenerator::SaveAsset(const GeneratedAsset& asset, const String& filePath) {
    return true;
}

GeneratedAsset AssetGenerator::LoadAsset(const String& filePath) {
    GeneratedAsset asset;
    return asset;
}

Vector<GeneratedAsset> AssetGenerator::GenerateBatch(const Vector<GenerationPrompt>& prompts) {
    Vector<GeneratedAsset> results;
    for (size_t i = 0; i < prompts.Size(); ++i) {
        results.PushBack(GenerateFromPrompt(prompts[i], AssetType::Texture));
    }
    return results;
}

String AssetGenerator::GenerateCharacterPrompt(const CharacterGenerationRequest& request) {
    String prompt = request.description;

    if (!request.appearance.Empty()) {
        prompt += String(", ") + request.appearance;
    }

    if (!request.artStyle.Empty()) {
        prompt += String(", ") + request.artStyle;
    }

    for (size_t i = 0; i < m_StyleKeywords.Size(); ++i) {
        prompt += String(", ") + m_StyleKeywords[i];
    }

    return prompt;
}

String AssetGenerator::GenerateEnvironmentPrompt(const EnvironmentGenerationRequest& request) {
    String prompt = request.description;

    if (!request.location.Empty()) {
        prompt += String(", location: ") + request.location;
    }

    if (!request.timeOfDay.Empty()) {
        prompt += String(", ") + request.timeOfDay;
    }

    if (!request.weather.Empty()) {
        prompt += String(", ") + request.weather;
    }

    for (size_t i = 0; i < m_StyleKeywords.Size(); ++i) {
        prompt += String(", ") + m_StyleKeywords[i];
    }

    return prompt;
}

GeneratedAsset AssetGenerator::CreatePlaceholderAsset(const String& name, AssetType type) {
    GeneratedAsset asset;
    asset.id = GenerateUniqueId();
    asset.name = name;
    asset.type = type;
    asset.width = 512;
    asset.height = 512;
    return asset;
}

}
