#include "Storyboard.h"

namespace SparkLabs {

Storyboard::Storyboard()
    : m_Name("Untitled Storyboard")
    , m_IdCounter(0)
{
}

Storyboard::~Storyboard() {
    m_Sequences.Clear();
}

StringHash Storyboard::GenerateUniqueId() {
    String idStr = String("storyboard_") + String(static_cast<int64>(++m_IdCounter));
    return StringHash(idStr.C_str());
}

void Storyboard::UpdateSequenceDuration(StoryboardSequence* sequence) {
    if (!sequence) return;
    
    float32 total = 0.0f;
    for (size_t i = 0; i < sequence->frames.Size(); ++i) {
        total += sequence->frames[i].duration;
        total += sequence->frames[i].transitionDuration;
    }
    sequence->totalDuration = total;
}

StoryboardSequence* Storyboard::CreateSequence(const String& name) {
    StoryboardSequence* sequence = new StoryboardSequence();
    sequence->id = GenerateUniqueId();
    sequence->name = name;
    AddSequence(sequence);
    return sequence;
}

void Storyboard::AddSequence(StoryboardSequence* sequence) {
    if (!sequence) return;
    m_Sequences.PushBack(SmartPtr<StoryboardSequence>(sequence));
}

void Storyboard::RemoveSequence(const StringHash& sequenceId) {
    for (size_t i = 0; i < m_Sequences.Size(); ++i) {
        if (m_Sequences[i]->id == sequenceId) {
            m_Sequences.Erase(i);
            return;
        }
    }
}

StoryboardSequence* Storyboard::GetSequence(const StringHash& sequenceId) {
    for (size_t i = 0; i < m_Sequences.Size(); ++i) {
        if (m_Sequences[i]->id == sequenceId) {
            return m_Sequences[i].Get();
        }
    }
    return nullptr;
}

Vector<StoryboardSequence*> Storyboard::GetAllSequences() {
    Vector<StoryboardSequence*> result;
    for (size_t i = 0; i < m_Sequences.Size(); ++i) {
        result.PushBack(m_Sequences[i].Get());
    }
    return result;
}

StoryboardFrame* Storyboard::CreateFrame(StoryboardSequence* sequence, const String& title) {
    if (!sequence) return nullptr;
    
    StoryboardFrame* frame = new StoryboardFrame();
    frame->id = GenerateUniqueId();
    frame->order = static_cast<int32>(sequence->frames.Size());
    frame->title = title;
    AddFrame(sequence, frame);
    return frame;
}

void Storyboard::AddFrame(StoryboardSequence* sequence, StoryboardFrame* frame) {
    if (!sequence || !frame) return;
    sequence->frames.PushBack(*frame);
    UpdateSequenceDuration(sequence);
}

void Storyboard::RemoveFrame(StoryboardSequence* sequence, const StringHash& frameId) {
    if (!sequence) return;
    
    for (size_t i = 0; i < sequence->frames.Size(); ++i) {
        if (sequence->frames[i].id == frameId) {
            sequence->frames.Erase(i);
            for (size_t j = i; j < sequence->frames.Size(); ++j) {
                sequence->frames[j].order = static_cast<int32>(j);
            }
            UpdateSequenceDuration(sequence);
            return;
        }
    }
}

void Storyboard::ReorderFrames(StoryboardSequence* sequence, const Vector<StringHash>& newOrder) {
    if (!sequence) return;
    
    Vector<StoryboardFrame> newFrames;
    for (size_t i = 0; i < newOrder.Size(); ++i) {
        for (size_t j = 0; j < sequence->frames.Size(); ++j) {
            if (sequence->frames[j].id == newOrder[i]) {
                sequence->frames[j].order = static_cast<int32>(newFrames.Size());
                newFrames.PushBack(sequence->frames[j]);
                break;
            }
        }
    }
    sequence->frames = newFrames;
    UpdateSequenceDuration(sequence);
}

void Storyboard::GenerateFromStoryAnalysis(const StoryAnalysisResult& analysis) {
    m_Name = analysis.title + String(" - Storyboard");
    
    for (size_t beatIdx = 0; beatIdx < analysis.beats.Size(); ++beatIdx) {
        const StoryBeat& beat = analysis.beats[beatIdx];
        
        StoryboardSequence* sequence = CreateSequence(beat.description);
        sequence->storyBeatId = beat.id;
        sequence->description = beat.description;
        
        StoryboardFrame* frame1 = CreateFrame(sequence, String("Opening - ") + beat.description);
        frame1->shotType = ShotType::Wide;
        frame1->action = beat.action;
        frame1->dialogue = beat.dialogue;
        frame1->sceneId = beat.sceneId;
        frame1->characterIds = beat.characterIds;
        frame1->duration = 3.0f;
        
        StoryboardFrame* frame2 = CreateFrame(sequence, String("Action - ") + beat.description);
        frame2->shotType = ShotType::Medium;
        frame2->action = beat.action;
        frame2->dialogue = beat.dialogue;
        frame2->sceneId = beat.sceneId;
        frame2->characterIds = beat.characterIds;
        frame2->duration = 4.0f;
        
        StoryboardFrame* frame3 = CreateFrame(sequence, String("Close-up - ") + beat.description);
        frame3->shotType = ShotType::CloseUp;
        frame3->action = beat.action;
        frame3->dialogue = beat.dialogue;
        frame3->sceneId = beat.sceneId;
        frame3->characterIds = beat.characterIds;
        frame3->duration = 2.0f;
    }
}

void Storyboard::AutoGenerateTransitions() {
    for (size_t seqIdx = 0; seqIdx < m_Sequences.Size(); ++seqIdx) {
        StoryboardSequence* sequence = m_Sequences[seqIdx].Get();
        for (size_t i = 0; i < sequence->frames.Size(); ++i) {
            if (i == 0) {
                sequence->frames[i].transitionIn = TransitionType::FadeIn;
            } else {
                sequence->frames[i].transitionIn = TransitionType::Cut;
            }
            
            if (i == sequence->frames.Size() - 1) {
                sequence->frames[i].transitionOut = TransitionType::FadeOut;
            } else {
                sequence->frames[i].transitionOut = TransitionType::Cut;
            }
            
            sequence->frames[i].transitionDuration = 0.5f;
        }
        UpdateSequenceDuration(sequence);
    }
}

void Storyboard::AutoCalculateDurations() {
    for (size_t seqIdx = 0; seqIdx < m_Sequences.Size(); ++seqIdx) {
        StoryboardSequence* sequence = m_Sequences[seqIdx].Get();
        for (size_t i = 0; i < sequence->frames.Size(); ++i) {
            StoryboardFrame& frame = sequence->frames[i];
            
            int32 wordCount = 0;
            const char* cstr = frame.dialogue.C_str();
            for (int32 j = 0; cstr[j] != '\0'; ++j) {
                if (cstr[j] == ' ' || cstr[j] == '\n') {
                    ++wordCount;
                }
            }
            if (frame.dialogue.Length() > 0) {
                ++wordCount;
            }
            
            const float32 wordsPerMinute = 150.0f;
            float32 dialogueDuration = (wordCount / wordsPerMinute) * 60.0f;
            
            frame.duration = 2.0f + dialogueDuration;
        }
        UpdateSequenceDuration(sequence);
    }
}

float32 Storyboard::GetTotalDuration() const {
    float32 total = 0.0f;
    for (size_t i = 0; i < m_Sequences.Size(); ++i) {
        total += m_Sequences[i]->totalDuration;
    }
    return total;
}

int32 Storyboard::GetTotalFrameCount() const {
    int32 count = 0;
    for (size_t i = 0; i < m_Sequences.Size(); ++i) {
        count += static_cast<int32>(m_Sequences[i]->frames.Size());
    }
    return count;
}

void Storyboard::SetMetadata(const StringHash& key, const Variant& value) {
    m_Metadata.Set(key, value);
}

Variant Storyboard::GetMetadata(const StringHash& key) const {
    Variant result;
    m_Metadata.Get(key, result);
    return result;
}

bool Storyboard::SaveToFile(const String& filePath) {
    return true;
}

bool Storyboard::LoadFromFile(const String& filePath) {
    return true;
}

}
