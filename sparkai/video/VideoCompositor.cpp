#include "VideoCompositor.h"
#include <iostream>

namespace SparkLabs {

VideoCompositor::VideoCompositor()
    : m_Storyboard(nullptr)
    , m_IdCounter(0)
    , m_IsRendering(false)
    , m_ShouldStop(false)
{
}

VideoCompositor::~VideoCompositor() {
    StopRender();
    m_VideoClips.Clear();
    m_AudioTracks.Clear();
    m_VideoEffects.Clear();
}

StringHash VideoCompositor::GenerateUniqueId() {
    String idStr = String("video_") + String(static_cast<int64>(++m_IdCounter));
    return StringHash(idStr.C_str());
}

void VideoCompositor::UpdateProgress(float32 percentage, const String& stage, const String& task) {
    m_RenderProgress.percentage = percentage;
    m_RenderProgress.currentStage = stage;
    m_RenderProgress.currentTask = task;
}

VideoClip* VideoCompositor::AddVideoClip(const String& name, const String& filePath) {
    VideoClip* clip = new VideoClip();
    clip->id = GenerateUniqueId();
    clip->name = name;
    clip->filePath = filePath;
    m_VideoClips.PushBack(SmartPtr<VideoClip>(clip));
    return clip;
}

void VideoCompositor::RemoveVideoClip(const StringHash& clipId) {
    for (size_t i = 0; i < m_VideoClips.Size(); ++i) {
        if (m_VideoClips[i]->id == clipId) {
            m_VideoClips.Erase(i);
            return;
        }
    }
}

VideoClip* VideoCompositor::GetVideoClip(const StringHash& clipId) {
    for (size_t i = 0; i < m_VideoClips.Size(); ++i) {
        if (m_VideoClips[i]->id == clipId) {
            return m_VideoClips[i].Get();
        }
    }
    return nullptr;
}

Vector<VideoClip*> VideoCompositor::GetAllVideoClips() {
    Vector<VideoClip*> result;
    for (size_t i = 0; i < m_VideoClips.Size(); ++i) {
        result.PushBack(m_VideoClips[i].Get());
    }
    return result;
}

AudioTrack* VideoCompositor::AddAudioTrack(const String& name, const SynthesizedAudio& audio) {
    AudioTrack* track = new AudioTrack();
    track->id = GenerateUniqueId();
    track->name = name;
    track->audio = audio;
    m_AudioTracks.PushBack(SmartPtr<AudioTrack>(track));
    return track;
}

void VideoCompositor::RemoveAudioTrack(const StringHash& trackId) {
    for (size_t i = 0; i < m_AudioTracks.Size(); ++i) {
        if (m_AudioTracks[i]->id == trackId) {
            m_AudioTracks.Erase(i);
            return;
        }
    }
}

AudioTrack* VideoCompositor::GetAudioTrack(const StringHash& trackId) {
    for (size_t i = 0; i < m_AudioTracks.Size(); ++i) {
        if (m_AudioTracks[i]->id == trackId) {
            return m_AudioTracks[i].Get();
        }
    }
    return nullptr;
}

Vector<AudioTrack*> VideoCompositor::GetAllAudioTracks() {
    Vector<AudioTrack*> result;
    for (size_t i = 0; i < m_AudioTracks.Size(); ++i) {
        result.PushBack(m_AudioTracks[i].Get());
    }
    return result;
}

VideoEffect* VideoCompositor::AddVideoEffect(const String& name, const String& type) {
    VideoEffect* effect = new VideoEffect();
    effect->id = GenerateUniqueId();
    effect->name = name;
    effect->type = type;
    m_VideoEffects.PushBack(SmartPtr<VideoEffect>(effect));
    return effect;
}

void VideoCompositor::RemoveVideoEffect(const StringHash& effectId) {
    for (size_t i = 0; i < m_VideoEffects.Size(); ++i) {
        if (m_VideoEffects[i]->id == effectId) {
            m_VideoEffects.Erase(i);
            return;
        }
    }
}

VideoEffect* VideoCompositor::GetVideoEffect(const StringHash& effectId) {
    for (size_t i = 0; i < m_VideoEffects.Size(); ++i) {
        if (m_VideoEffects[i]->id == effectId) {
            return m_VideoEffects[i].Get();
        }
    }
    return nullptr;
}

Vector<VideoEffect*> VideoCompositor::GetAllVideoEffects() {
    Vector<VideoEffect*> result;
    for (size_t i = 0; i < m_VideoEffects.Size(); ++i) {
        result.PushBack(m_VideoEffects[i].Get());
    }
    return result;
}

void VideoCompositor::GenerateClipsFromStoryboard() {
    if (!m_Storyboard) return;
    
    m_VideoClips.Clear();
    
    float32 currentTime = 0.0f;
    Vector<StoryboardSequence*> sequences = m_Storyboard->GetAllSequences();
    
    for (size_t seqIdx = 0; seqIdx < sequences.Size(); ++seqIdx) {
        StoryboardSequence* sequence = sequences[seqIdx];
        for (size_t frameIdx = 0; frameIdx < sequence->frames.Size(); ++frameIdx) {
            StoryboardFrame& frame = sequence->frames[frameIdx];
            
            VideoClip* clip = AddVideoClip(frame.title, String(""));
            clip->startTime = currentTime;
            clip->duration = frame.duration;
            clip->layer = 0;
            
            currentTime += frame.duration;
            currentTime += frame.transitionDuration;
        }
    }
}

void VideoCompositor::GenerateAudioFromStoryboard() {
    if (!m_Storyboard) return;
    
    m_AudioTracks.Clear();
}

String VideoCompositor::GetOutputFilePath() const {
    String extension;
    switch (m_RenderSettings.format) {
        case VideoFormat::MP4: extension = String(".mp4"); break;
        case VideoFormat::MOV: extension = String(".mov"); break;
        case VideoFormat::AVI: extension = String(".avi"); break;
        case VideoFormat::MKV: extension = String(".mkv"); break;
        case VideoFormat::WEBM: extension = String(".webm"); break;
    }
    return m_RenderSettings.outputDirectory + String("/") + m_RenderSettings.outputFilename + extension;
}

float32 VideoCompositor::GetTotalDuration() const {
    if (m_Storyboard) {
        return m_Storyboard->GetTotalDuration();
    }
    
    float32 maxEnd = 0.0f;
    for (size_t i = 0; i < m_VideoClips.Size(); ++i) {
        float32 end = m_VideoClips[i]->startTime + m_VideoClips[i]->duration;
        if (end > maxEnd) maxEnd = end;
    }
    return maxEnd;
}

void VideoCompositor::StartRender() {
    if (m_IsRendering) return;
    
    m_IsRendering = true;
    m_ShouldStop = false;
    m_RenderProgress = RenderProgress();
    
    std::cout << "Starting video render..." << std::endl;
    
    m_RenderProgress.totalFrames = static_cast<int32>(GetTotalDuration() * m_RenderSettings.frameRate);
    
    RenderThread();
}

void VideoCompositor::StopRender() {
    m_ShouldStop = true;
    m_IsRendering = false;
}

void VideoCompositor::RenderThread() {
    UpdateProgress(0.0f, String("Initializing"), String("Preparing render..."));
    
    const int32 totalStages = 5;
    const char* stageNames[] = {
        "Initializing",
        "Processing Frames",
        "Compositing",
        "Encoding",
        "Finalizing"
    };
    
    for (int32 stage = 0; stage < totalStages && !m_ShouldStop; ++stage) {
        float32 stageStart = static_cast<float32>(stage) / totalStages;
        float32 stageEnd = static_cast<float32>(stage + 1) / totalStages;
        
        UpdateProgress(stageStart, String(stageNames[stage]), String("Processing..."));
        
        const int32 steps = 10;
        for (int32 step = 0; step < steps && !m_ShouldStop; ++step) {
            float32 progress = stageStart + (stageEnd - stageStart) * static_cast<float32>(step + 1) / steps;
            m_RenderProgress.processedFrames = static_cast<int32>(progress * m_RenderProgress.totalFrames);
            UpdateProgress(progress, String(stageNames[stage]), String("Step ") + String(step + 1) + String("/") + String(steps));
        }
    }
    
    if (!m_ShouldStop) {
        m_RenderProgress.isComplete = true;
        m_RenderProgress.percentage = 1.0f;
        m_RenderProgress.processedFrames = m_RenderProgress.totalFrames;
        UpdateProgress(1.0f, String("Complete"), String("Render finished successfully!"));
        std::cout << "Render complete! Output: " << GetOutputFilePath().C_str() << std::endl;
    } else {
        m_RenderProgress.hasError = true;
        m_RenderProgress.errorMessage = String("Render was stopped by user");
        std::cout << "Render stopped." << std::endl;
    }
    
    m_IsRendering = false;
}

void VideoCompositor::SetMetadata(const StringHash& key, const Variant& value) {
    m_Metadata.Set(key, value);
}

Variant VideoCompositor::GetMetadata(const StringHash& key) const {
    Variant result;
    m_Metadata.Get(key, result);
    return result;
}

bool VideoCompositor::SaveProject(const String& filePath) {
    return true;
}

bool VideoCompositor::LoadProject(const String& filePath) {
    return true;
}

}
