#ifndef SPARKLABS_SPARKAI_WORKFLOW_NODES_AIGENERATIONNODES_H
#define SPARKLABS_SPARKAI_WORKFLOW_NODES_AIGENERATIONNODES_H

#include "../WorkflowGraph.h"
#include <Vector.h>
#include <String.h>

namespace SparkLabs {

class ImageGenerationNode : public WorkflowNode {
    SPARKLABS_OBJECT(ImageGenerationNode)
public:
    ImageGenerationNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetModel(const String& model);
    String GetModel() const;

    void SetWidth(int32 width);
    int32 GetWidth() const;

    void SetHeight(int32 height);
    int32 GetHeight() const;

    void SetSteps(int32 steps);
    int32 GetSteps() const;

    void SetGuidanceScale(float32 scale);
    float32 GetGuidanceScale() const;

    void SetSeed(uint64 seed);
    uint64 GetSeed() const;

private:
    String m_Model;
    int32 m_Width;
    int32 m_Height;
    int32 m_Steps;
    float32 m_GuidanceScale;
    uint64 m_Seed;
};

class TextGenerationNode : public WorkflowNode {
    SPARKLABS_OBJECT(TextGenerationNode)
public:
    TextGenerationNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetModel(const String& model);
    String GetModel() const;

    void SetMaxTokens(int32 tokens);
    int32 GetMaxTokens() const;

    void SetTemperature(float32 temp);
    float32 GetTemperature() const;

private:
    String m_Model;
    int32 m_MaxTokens;
    float32 m_Temperature;
};

class VideoGenerationNode : public WorkflowNode {
    SPARKLABS_OBJECT(VideoGenerationNode)
public:
    VideoGenerationNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetModel(const String& model);
    String GetModel() const;

    void SetDuration(int32 frames);
    int32 GetDuration() const;

    void SetFps(int32 fps);
    int32 GetFps() const;

private:
    String m_Model;
    int32 m_Duration;
    int32 m_Fps;
};

class AudioGenerationNode : public WorkflowNode {
    SPARKLABS_OBJECT(AudioGenerationNode)
public:
    AudioGenerationNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetModel(const String& model);
    String GetModel() const;

    void SetDuration(float32 seconds);
    float32 GetDuration() const;

    void SetSampleRate(int32 rate);
    int32 GetSampleRate() const;

private:
    String m_Model;
    float32 m_Duration;
    int32 m_SampleRate;
};

class LoadImageNode : public WorkflowNode {
    SPARKLABS_OBJECT(LoadImageNode)
public:
    LoadImageNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetFilePath(const String& path);
    String GetFilePath() const;

private:
    String m_FilePath;
};

class SaveImageNode : public WorkflowNode {
    SPARKLABS_OBJECT(SaveImageNode)
public:
    SaveImageNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetOutputPath(const String& path);
    String GetOutputPath() const;

private:
    String m_OutputPath;
};

class LoadModelNode : public WorkflowNode {
    SPARKLABS_OBJECT(LoadModelNode)
public:
    LoadModelNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetModelPath(const String& path);
    String GetModelPath() const;

    void SetModelType(const String& type);
    String GetModelType() const;

private:
    String m_ModelPath;
    String m_ModelType;
};

class TextPromptNode : public WorkflowNode {
    SPARKLABS_OBJECT(TextPromptNode)
public:
    TextPromptNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetPrompt(const String& prompt);
    String GetPrompt() const;

    void SetNegativePrompt(const String& prompt);
    String GetNegativePrompt() const;

private:
    String m_Prompt;
    String m_NegativePrompt;
};

class ConditioningNode : public WorkflowNode {
    SPARKLABS_OBJECT(ConditioningNode)
public:
    ConditioningNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetStrength(float32 strength);
    float32 GetStrength() const;

private:
    float32 m_Strength;
};

class LatentNode : public WorkflowNode {
    SPARKLABS_OBJECT(LatentNode)
public:
    LatentNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetWidth(int32 width);
    int32 GetWidth() const;

    void SetHeight(int32 height);
    int32 GetHeight() const;

    void SetBatchSize(int32 batch);
    int32 GetBatchSize() const;

private:
    int32 m_Width;
    int32 m_Height;
    int32 m_BatchSize;
};

class KSamplerNode : public WorkflowNode {
    SPARKLABS_OBJECT(KSamplerNode)
public:
    KSamplerNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetSteps(int32 steps);
    int32 GetSteps() const;

    void SetCfg(float32 cfg);
    float32 GetCfg() const;

    void SetSeed(uint64 seed);
    uint64 GetSeed() const;

    void SetSamplerName(const String& name);
    String GetSamplerName() const;

private:
    int32 m_Steps;
    float32 m_Cfg;
    uint64 m_Seed;
    String m_SamplerName;
};

class VAEDecodeNode : public WorkflowNode {
    SPARKLABS_OBJECT(VAEDecodeNode)
public:
    VAEDecodeNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;
};

class VAEEncodeNode : public WorkflowNode {
    SPARKLABS_OBJECT(VAEEncodeNode)
public:
    VAEEncodeNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;
};

class ControlNetNode : public WorkflowNode {
    SPARKLABS_OBJECT(ControlNetNode)
public:
    ControlNetNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetControlNetPath(const String& path);
    String GetControlNetPath() const;

    void SetStrength(float32 strength);
    float32 GetStrength() const;

private:
    String m_ControlNetPath;
    float32 m_Strength;
};

class UpscaleNode : public WorkflowNode {
    SPARKLABS_OBJECT(UpscaleNode)
public:
    UpscaleNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetMethod(const String& method);
    String GetMethod() const;

    void SetScale(float32 scale);
    float32 GetScale() const;

private:
    String m_Method;
    float32 m_Scale;
};

class InpaintNode : public WorkflowNode {
    SPARKLABS_OBJECT(InpaintNode)
public:
    InpaintNode();

    virtual bool Execute(const Map<String, Variant>& inputs, Map<String, Variant>& outputs) override;

    void SetMask(const String& mask);
    String GetMask() const;

private:
    String m_Mask;
};

}
