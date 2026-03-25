#include "StoryParser.h"
#include <algorithm>
#include <cctype>

namespace SparkLabs {

StoryParser::StoryParser()
    : m_ExtractCharacters(true)
    , m_ExtractScenes(true)
    , m_ExtractBeats(true)
    , m_IdCounter(0)
{
    m_CharacterKeywords.PushBack(String("name:"));
    m_CharacterKeywords.PushBack(String("character:"));
    m_CharacterKeywords.PushBack(String("protagonist"));
    m_CharacterKeywords.PushBack(String("antagonist"));

    m_SceneKeywords.PushBack(String("scene:"));
    m_SceneKeywords.PushBack(String("setting:"));
    m_SceneKeywords.PushBack(String("location:"));
    m_SceneKeywords.PushBack(String("int."));
    m_SceneKeywords.PushBack(String("ext."));

    m_DialogueIndicators.PushBack(String("\""));
    m_DialogueIndicators.PushBack(String("'"));
}

StoryParser::~StoryParser() {
}

StoryAnalysisResult StoryParser::ParseText(const String& text) {
    StoryAnalysisResult result;

    AnalyzeGenreAndSummary(text, result);

    if (m_ExtractCharacters) {
        ExtractCharacters(text, result);
    }

    if (m_ExtractScenes) {
        ExtractScenes(text, result);
    }

    if (m_ExtractBeats) {
        ExtractBeats(text, result);
    }

    ExtractThemes(text, result);

    return result;
}

StoryAnalysisResult StoryParser::ParseFile(const String& filePath) {
    StoryAnalysisResult result;
    return result;
}

void StoryParser::AddCharacterKeyword(const String& keyword) {
    m_CharacterKeywords.PushBack(keyword);
}

void StoryParser::AddSceneKeyword(const String& keyword) {
    m_SceneKeywords.PushBack(keyword);
}

void StoryParser::AddDialogueIndicator(const String& indicator) {
    m_DialogueIndicators.PushBack(indicator);
}

StringHash StoryParser::GenerateUniqueId() {
    String idStr = String("story_") + String(static_cast<int64>(++m_IdCounter));
    return StringHash(idStr.C_str());
}

void StoryParser::AnalyzeGenreAndSummary(const String& text, StoryAnalysisResult& result) {
    result.title = String("Untitled Story");
    result.genre = String("drama");
    result.summary = String("A narrative story.");
}

void StoryParser::ExtractCharacters(const String& text, StoryAnalysisResult& result) {
    CharacterProfile protagonist;
    protagonist.id = GenerateUniqueId();
    protagonist.name = String("Protagonist");
    protagonist.description = String("The main character of the story.");
    protagonist.personality = String("brave, determined");
    result.characters.PushBack(protagonist);

    CharacterProfile antagonist;
    antagonist.id = GenerateUniqueId();
    antagonist.name = String("Antagonist");
    antagonist.description = String("The opposing character.");
    antagonist.personality = String("cunning, ambitious");
    result.characters.PushBack(antagonist);
}

void StoryParser::ExtractScenes(const String& text, StoryAnalysisResult& result) {
    SceneDescription opening;
    opening.id = GenerateUniqueId();
    opening.name = String("Opening Scene");
    opening.location = String("Unknown Location");
    opening.description = String("The story begins here.");
    opening.timeOfDay = String("day");
    result.scenes.PushBack(opening);
}

void StoryParser::ExtractBeats(const String& text, StoryAnalysisResult& result) {
    StoryBeat beat1;
    beat1.id = GenerateUniqueId();
    beat1.order = 1;
    beat1.description = String("Introduction");
    result.beats.PushBack(beat1);
}

void StoryParser::ExtractThemes(const String& text, StoryAnalysisResult& result) {
    result.themes.PushBack(String("courage"));
    result.themes.PushBack(String("friendship"));
    result.themes.PushBack(String("adventure"));
}

}
