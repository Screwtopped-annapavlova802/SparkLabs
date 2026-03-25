#ifndef SPARKLABS_SPARKAI_NARRATIVE_STORYPARSER_H
#define SPARKLABS_SPARKAI_NARRATIVE_STORYPARSER_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/io/Vector.h"
#include "../../core/Map.h"
#include "../../core/Variant.h"

namespace SparkLabs {

struct CharacterProfile {
    StringHash id;
    String name;
    String description;
    String personality;
    String appearance;
    Vector<String> traits;
    Map<StringHash, Variant> metadata;

    CharacterProfile() : id(StringHash()) {}
};

struct SceneDescription {
    StringHash id;
    String name;
    String location;
    String description;
    String timeOfDay;
    String weather;
    Vector<String> environmentTags;
    Map<StringHash, Variant> metadata;

    SceneDescription() : id(StringHash()) {}
};

struct StoryBeat {
    StringHash id;
    int32 order;
    String description;
    StringHash sceneId;
    Vector<StringHash> characterIds;
    String dialogue;
    String action;
    Map<StringHash, Variant> metadata;

    StoryBeat() : id(StringHash()), order(0) {}
};

struct StoryAnalysisResult {
    String title;
    String genre;
    String summary;
    Vector<CharacterProfile> characters;
    Vector<SceneDescription> scenes;
    Vector<StoryBeat> beats;
    Vector<String> themes;
    Map<StringHash, Variant> metadata;
};

class StoryParser {
public:
    StoryParser();
    ~StoryParser();

    StoryAnalysisResult ParseText(const String& text);
    StoryAnalysisResult ParseFile(const String& filePath);

    void SetCharacterExtractEnabled(bool enabled) { m_ExtractCharacters = enabled; }
    void SetSceneExtractEnabled(bool enabled) { m_ExtractScenes = enabled; }
    void SetBeatExtractEnabled(bool enabled) { m_ExtractBeats = enabled; }

    void AddCharacterKeyword(const String& keyword);
    void AddSceneKeyword(const String& keyword);
    void AddDialogueIndicator(const String& indicator);

private:
    void ExtractCharacters(const String& text, StoryAnalysisResult& result);
    void ExtractScenes(const String& text, StoryAnalysisResult& result);
    void ExtractBeats(const String& text, StoryAnalysisResult& result);
    void ExtractThemes(const String& text, StoryAnalysisResult& result);
    void AnalyzeGenreAndSummary(const String& text, StoryAnalysisResult& result);

    StringHash GenerateUniqueId();

    bool m_ExtractCharacters;
    bool m_ExtractScenes;
    bool m_ExtractBeats;
    Vector<String> m_CharacterKeywords;
    Vector<String> m_SceneKeywords;
    Vector<String> m_DialogueIndicators;
    uint64 m_IdCounter;
};

}

#endif
