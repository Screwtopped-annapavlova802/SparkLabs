#include "VoiceSynthesis.h"
#include <algorithm>

namespace SparkLabs {

VoiceSynthesis::VoiceSynthesis()
    : m_DefaultSampleRate(44100)
    , m_OutputDirectory(String("./output/audio"))
    , m_IdCounter(0)
{
    VoiceProfile defaultVoice;
    defaultVoice.id = GenerateUniqueId();
    defaultVoice.name = String("Default");
    defaultVoice.gender = VoiceGender::Neutral;
    defaultVoice.language = String("en");
    m_Voices.Set(defaultVoice.id, defaultVoice);
    m_DefaultVoiceId = defaultVoice.id;

    VoiceProfile maleVoice;
    maleVoice.id = GenerateUniqueId();
    maleVoice.name = String("Male");
    maleVoice.gender = VoiceGender::Male;
    maleVoice.pitch = 0.8f;
    maleVoice.language = String("en");
    m_Voices.Set(maleVoice.id, maleVoice);

    VoiceProfile femaleVoice;
    femaleVoice.id = GenerateUniqueId();
    femaleVoice.name = String("Female");
    femaleVoice.gender = VoiceGender::Female;
    femaleVoice.pitch = 1.2f;
    femaleVoice.language = String("en");
    m_Voices.Set(femaleVoice.id, femaleVoice);
}

VoiceSynthesis::~VoiceSynthesis() {
}

StringHash VoiceSynthesis::GenerateUniqueId() {
    String idStr = String("audio_") + String(static_cast<int64>(++m_IdCounter));
    return StringHash(idStr.C_str());
}

SynthesizedAudio VoiceSynthesis::SynthesizeSpeech(const SpeechRequest& request) {
    SynthesizedAudio audio = CreatePlaceholderAudio(request.text);
    audio.request = request;
    return audio;
}

SynthesizedAudio VoiceSynthesis::SynthesizeDialogueLine(const DialogueLine& line, const VoiceProfile& voice) {
    SpeechRequest request;
    request.text = line.text;
    request.voiceId = voice.id;
    request.emotion = line.emotion;
    request.pitch = voice.pitch;
    request.speed = voice.speed;
    request.volume = voice.volume;

    SynthesizedAudio audio = SynthesizeSpeech(request);
    audio.metadata.Set(StringHash("speaker"), Variant(line.speakerName));
    audio.metadata.Set(StringHash("emotion"), Variant(static_cast<int32>(line.emotion)));
    return audio;
}

Vector<SynthesizedAudio> VoiceSynthesis::SynthesizeSequence(
    const DialogueSequence& sequence,
    const Map<StringHash, VoiceProfile>& voices)
{
    Vector<SynthesizedAudio> results;

    for (size_t i = 0; i < sequence.lines.Size(); ++i) {
        const DialogueLine& line = sequence.lines[i];

        VoiceProfile* voice = nullptr;
        voices.Get(line.speakerId, voice);

        if (!voice) {
            voice = GetVoice(m_DefaultVoiceId);
        }

        if (voice) {
            SynthesizedAudio audio = SynthesizeDialogueLine(line, *voice);
            results.PushBack(audio);
        }
    }

    return results;
}

void VoiceSynthesis::RegisterVoice(const VoiceProfile& voice) {
    m_Voices.Set(voice.id, voice);
}

void VoiceSynthesis::UnregisterVoice(const StringHash& voiceId) {
    m_Voices.Remove(voiceId);
}

VoiceProfile* VoiceSynthesis::GetVoice(const StringHash& voiceId) {
    VoiceProfile* voice = nullptr;
    m_Voices.Get(voiceId, voice);
    return voice;
}

Vector<VoiceProfile> VoiceSynthesis::GetAllVoices() const {
    Vector<VoiceProfile> voices;
    for (const auto& pair : m_Voices.GetPairs()) {
        voices.PushBack(pair.value);
    }
    return voices;
}

bool VoiceSynthesis::SaveAudio(const SynthesizedAudio& audio, const String& filePath) {
    return true;
}

SynthesizedAudio VoiceSynthesis::LoadAudio(const String& filePath) {
    SynthesizedAudio audio;
    return audio;
}

SynthesizedAudio VoiceSynthesis::CreatePlaceholderAudio(const String& text) {
    SynthesizedAudio audio;
    audio.id = GenerateUniqueId();
    audio.sampleRate = m_DefaultSampleRate;
    audio.channels = 1;
    audio.bitsPerSample = 16;
    audio.duration = EstimateDuration(text, 1.0f);
    audio.totalSamples = static_cast<size_t>(audio.duration * audio.sampleRate);
    return audio;
}

float32 VoiceSynthesis::EstimateDuration(const String& text, float32 speed) {
    int32 wordCount = 0;
    const char* cstr = text.C_str();
    for (int32 i = 0; cstr[i] != '\0'; ++i) {
        if (cstr[i] == ' ' || cstr[i] == '\n') {
            ++wordCount;
        }
    }
    if (text.Length() > 0) {
        ++wordCount;
    }

    const float32 wordsPerMinute = 150.0f / speed;
    return (wordCount / wordsPerMinute) * 60.0f;
}

}
