# SparkLabs Engine - AI System Documentation

This document provides comprehensive documentation on the AI systems within SparkLabs Engine, covering the AI Core Runtime, Behavior Tree system, Neural Network integration, NPC Brain architecture, and Memory system.

## Table of Contents

1. [AI Core Runtime](#ai-core-runtime)
2. [Behavior Tree System](#behavior-tree-system)
3. [Neural Network Integration](#neural-network-integration)
4. [NPC Brain Architecture](#npc-brain-architecture)
5. [Memory System](#memory-system)

---

## AI Core Runtime

### Overview

The AI Core Runtime is the central intelligence layer of SparkLabs Engine, providing all AI-related capabilities including decision making, neural network inference, behavior execution, and memory management.

### Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              AI Core Runtime                                │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌─────────────────────────────────────────────────────────────────────────┐ │
│  │                           AIBrain                                        │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────────┐  │ │
│  │  │   Brain    │  │   Neural   │  │    AI      │  │       AI        │  │ │
│  │  │  Controller│  │   Network  │  │  Blackboard│  │     EventBus     │  │ │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────────┘  │ │
│  │                                                                         │ │
│  │  ┌─────────────────────────────────────────────────────────────────────┐│ │
│  │  │                    Behavior Tree Execution                          ││ │
│  │  │   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐          ││ │
│  │  │   │Sequence│───▶│Selector │───▶│Parallel │───▶│Decorator│          ││ │
│  │  │   └─────────┘    └─────────┘    └─────────┘    └─────────┘          ││ │
│  │  │         │              │              │              │             ││ │
│  │  │         ▼              ▼              ▼              ▼             ││ │
│  │  │   ┌─────────────────────────────────────────────────────────────┐  ││ │
│  │  │   │                     Action Nodes                              │  ││ │
│  │  │   │  MoveTo  │  Attack  │  Wait  │  Custom  │  Condition         │  ││ │
│  │  │   └─────────────────────────────────────────────────────────────┘  │  ││ │
│  │  └─────────────────────────────────────────────────────────────────────┘│ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
│                                    │                                            │
│                                    ▼                                            │
│  ┌─────────────────────────────────────────────────────────────────────────┐ │
│  │                         ONNX Runtime                                      │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────────┐  │ │
│  │  │    CPU     │  │    GPU     │  │   Tensor   │  │    Memory       │  │ │
│  │  │  Provider  │  │  Provider  │  │   Engine   │  │    Pool         │  │ │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────────┘  │ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

### AIBrain Component

The `AIBrain` is the primary AI controller component that can be attached to any game object.

```cpp
namespace SparkLabs {

class AIBrain : public Component {
    DECLARE_RTTI

public:
    AIBrain();
    virtual ~AIBrain();

    virtual void OnUpdate(float32 deltaTime) override;

    void AttachNode(AINode* node);
    void DetachNode(AINode* node);

    SmartPtr<AINode> GetRootNode() const { return m_RootNode; }
    void SetRootNode(SmartPtr<AINode> root) { m_RootNode = root; }

    SmartPtr<AIBlackboard> GetBlackboard() const { return m_Blackboard; }
    SmartPtr<AIEventBus> GetEventBus() const { return m_EventBus; }
    SmartPtr<NeuralNetwork> GetNeuralNetwork() const { return m_NeuralNetwork; }

    void SetNeuralNetwork(SmartPtr<NeuralNetwork> network);

    TensorRef Think(const TensorRef& input);

    const Vector<AIMemoryEntry>& GetMemory() const { return m_Memory; }
    void AddMemory(const AIMemoryEntry& entry);
    void ClearMemory();

    void AddToMemory(const String& content, MemoryType type, float64 importance = 0.5);

private:
    SmartPtr<AINode> m_RootNode;
    SmartPtr<AIBlackboard> m_Blackboard;
    SmartPtr<AIEventBus> m_EventBus;
    SmartPtr<NeuralNetwork> m_NeuralNetwork;
    Vector<AIMemoryEntry> m_Memory;
    Vector<AINode*> m_AttachedNodes;
};

}
```

**Key Responsibilities:**
- Coordinates all AI subsystems
- Manages behavior tree execution
- Handles neural network inference requests
- Maintains AI memory
- Processes sensory input and generates actions

### AIBlackboard

The AIBlackboard serves as shared data repository for AI systems, enabling communication between different AI components and behavior tree nodes.

```cpp
namespace SparkLabs {

class AIBlackboard {
public:
    AIBlackboard();
    ~AIBlackboard();

    void SetValue(const StringHash& key, const Variant& value);
    bool GetValue(const StringHash& key, Variant& outValue) const;
    bool HasKey(const StringHash& key) const;
    void RemoveKey(const StringHash& key);

    void Clear();
    Vector<StringHash> GetAllKeys() const;

private:
    std::map<StringHash, Variant> m_Data;
};

}
```

**Usage Pattern:**
```cpp
// In AI initialization
SmartPtr<AIBlackboard> bb = MakeSmartPtr<AIBlackboard>();
bb->SetValue(StringHash("health"), 100.0f);
bb->SetValue(StringHash("enemy"), targetEntity.Get());

// In behavior tree node
float32 health;
if (bb->GetValue(StringHash("health"), health)) {
    // Make decisions based on health
}
```

### AIEventBus

The AIEventBus provides an inter-AI communication system for decoupled event handling.

```cpp
namespace SparkLabs {

class AIEventBus {
public:
    using EventCallback = std::function<void(const Variant&)>;

    AIEventBus();
    ~AIEventBus();

    void Subscribe(const StringHash& eventType, EventCallback callback);
    void Unsubscribe(const StringHash& eventType, EventCallback callback);

    void Publish(const StringHash& eventType, const Variant& data);
    void Clear();

private:
    std::map<StringHash, Vector<EventCallback>> m_Subscribers;
};

}
```

**Event Flow:**
```
Publisher                     EventBus                      Subscriber
   │                             │                              │
   │ Publish("PlayerSpotted")   │                              │
   │───────────────────────────▶│                              │
   │                             │ Notify("PlayerSpotted")     │
   │                             │─────────────────────────────▶│
   │                             │                              │
```

---

## Behavior Tree System

### Overview

The Behavior Tree system provides a hierarchical task execution framework for AI decision making. It uses a tree structure where nodes represent behaviors, and the tree traversal determines AI actions.

### Node Types

#### Base Node (BehaviorTreeNode)

```cpp
namespace SparkLabs {

enum class NodeStatus {
    Success,   // Node completed successfully
    Failure,   // Node failed
    Running    // Node is still executing
};

class BehaviorTreeNode {
public:
    BehaviorTreeNode();
    virtual ~BehaviorTreeNode() = default;

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) = 0;

    void SetParent(BehaviorTreeNode* parent);
    BehaviorTreeNode* GetParent() const { return m_Parent; }

    void AddChild(SmartPtr<BehaviorTreeNode> child);
    const Vector< SmartPtr<BehaviorTreeNode> >& GetChildren() const { return m_Children; }

protected:
    BehaviorTreeNode* m_Parent;
    Vector< SmartPtr<BehaviorTreeNode> > m_Children;
};

}
```

### Composite Nodes

Composite nodes control the flow of execution among their children.

#### Sequence Node

Executes children in order until one fails. Returns success when all children succeed.

```
┌─────────────────┐
│     Sequence     │
├─────────────────┤
│ Child 1 ──▶ OK  │
│ Child 2 ──▶ OK  │
│ Child 3 ──▶ OK  │
│ Result: Success │
└─────────────────┘
```

```cpp
class SequenceNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

#### Selector Node

Executes children in order until one succeeds. Returns failure when all children fail.

```
┌─────────────────┐
│     Selector     │
├─────────────────┤
│ Child 1 ──▶ Fail │
│ Child 2 ──▶ Fail │
│ Child 3 ──▶ OK   │
│ Result: Success  │
└─────────────────┘
```

```cpp
class SelectorNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

#### Parallel Node

Executes all children simultaneously. Completes when a specified number of children succeed or fail.

```cpp
class ParallelNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

    void SetSuccessThreshold(float32 threshold);
    void SetFailureThreshold(float32 threshold);

private:
    float32 m_SuccessThreshold;
    float32 m_FailureThreshold;
};
```

#### Random Selector Node

Selects and executes a random child each tick.

```cpp
class RandomSelectorNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

### Decorator Nodes

Decorator nodes modify the behavior of their single child.

#### Repeater Node

Repeats child execution a specified number of times or indefinitely.

```cpp
class RepeaterNode : public BehaviorTreeNode {
public:
    RepeaterNode();
    void SetCount(int32 count);
    void SetInfinite(boolean infinite);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    int32 m_Count;
    int32 m_ExecutedCount;
    boolean m_Infinite;
};
```

#### Inverter Node

Inverts the result of the child node (success → failure, failure → success).

```cpp
class InverterNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

#### Succeeder Node

Always returns success regardless of child result.

```cpp
class SucceederNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

#### Failer Node

Always returns failure regardless of child result.

```cpp
class FailerNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};
```

#### Condition Node

Evaluates a condition function and returns success if true, failure if false.

```cpp
class ConditionNode : public BehaviorTreeNode {
public:
    using ConditionFunc = std::function<bool(Agent*)>;

    ConditionNode(ConditionFunc condition);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    ConditionFunc m_Condition;
};
```

### Action Nodes

Action nodes are leaf nodes that perform specific actions.

#### MoveTo Action

```cpp
class MoveToAction : public BehaviorTreeNode {
public:
    MoveToAction(const Vector3& target);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    Vector3 m_Target;
};
```

#### Attack Action

```cpp
class AttackAction : public BehaviorTreeNode {
public:
    AttackAction(Agent* target);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    Agent* m_Target;
};
```

#### Wait Action

```cpp
class WaitAction : public BehaviorTreeNode {
public:
    WaitAction(float32 duration);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    float32 m_Duration;
    float32 m_Elapsed;
};
```

### Agent Class

The Agent is the executor of behavior trees, providing an interface for actions to interact with the game world.

```cpp
namespace SparkLabs {

class Agent {
public:
    Agent();
    virtual ~Agent();

    void AttachBehaviorTree(SmartPtr<BehaviorTree> tree);
    void DetachBehaviorTree();

    SmartPtr<BehaviorTree> GetBehaviorTree() const { return m_Tree; }

    virtual void Update(float32 deltaTime);

    void SetBlackboard(SmartPtr<AIBlackboard> blackboard);
    SmartPtr<AIBlackboard> GetBlackboard() const { return m_Blackboard; }

    virtual Vector3 GetPosition() const = 0;
    virtual void MoveTo(const Vector3& position) = 0;

protected:
    SmartPtr<BehaviorTree> m_Tree;
    SmartPtr<AIBlackboard> m_Blackboard;
};

}
```

### Behavior Tree Construction Example

```cpp
SmartPtr<BehaviorTree> CreateEnemyBehaviorTree() {
    SmartPtr<BehaviorTree> tree = MakeSmartPtr<BehaviorTree>();

    // Root selector: Try combat, then patrol, then idle
    SmartPtr<SelectorNode> root = MakeSmartPtr<SelectorNode>();

    // Combat sequence
    SmartPtr<SequenceNode> combatSeq = MakeSmartPtr<SequenceNode>();
    combatSeq->AddChild(MakeSmartPtr<ConditionNode>([](Agent* agent) {
        return agent->GetBlackboard()->HasKey(StringHash("enemy"));
    }));
    combatSeq->AddChild(MakeSmartPtr<MoveToAction>(Vector3(0, 0, 10)));
    combatSeq->AddChild(MakeSmartPtr<AttackAction>(nullptr));

    // Patrol sequence
    SmartPtr<SequenceNode> patrolSeq = MakeSmartPtr<SequenceNode>();
    patrolSeq->AddChild(MakeSmartPtr<MoveToAction>(Vector3(5, 0, 5)));
    patrolSeq->AddChild(MakeSmartPtr<WaitAction>(2.0f));
    patrolSeq->AddChild(MakeSmartPtr<MoveToAction>(Vector3(-5, 0, -5)));
    patrolSeq->AddChild(MakeSmartPtr<WaitAction>(2.0f));

    // Idle sequence
    SmartPtr<SequenceNode> idleSeq = MakeSmartPtr<SequenceNode>();
    idleSeq->AddChild(MakeSmartPtr<WaitAction>(5.0f));

    // Assemble tree
    root->AddChild(combatSeq);
    root->AddChild(patrolSeq);
    root->AddChild(idleSeq);

    tree->SetRoot(root);
    return tree;
}
```

### Behavior Tree Execution

```cpp
void BehaviorTree::Execute(float32 deltaTime) {
    if (!m_Running || !m_Root) return;

    NodeStatus status = Tick(m_Agent, deltaTime);

    if (status != NodeStatus::Running) {
        m_Running = false;
    }
}

NodeStatus BehaviorTree::Tick(Agent* agent, float32 deltaTime) {
    if (!m_Root) return NodeStatus::Failure;
    return m_Root->Execute(agent, deltaTime);
}
```

---

## Neural Network Integration

### Overview

SparkLabs provides seamless neural network integration through ONNX Runtime, allowing AI systems to use trained ML models for decision making, dialogue generation, and other intelligent behaviors.

### NeuralNetwork Class

```cpp
namespace SparkLabs {

class NeuralNetwork {
public:
    NeuralNetwork();
    ~NeuralNetwork();

    bool LoadModel(const String& path);
    bool LoadModelFromMemory(const uint8* data, size_t size);

    TensorRef Forward(const TensorRef& input);
    Vector<TensorRef> ForwardBatch(const Vector<TensorRef>& inputs);

    void SetInputMapping(const Vector<StringHash>& inputNames, const Vector<TensorShape>& shapes);
    void SetOutputMapping(const Vector<StringHash>& outputNames, const Vector<TensorShape>& shapes);

    TensorRef GetOutput(const StringHash& outputName);
    Vector<StringHash> GetOutputNames() const;

    bool IsLoaded() const { return m_Loaded; }

private:
    bool m_Loaded;
    void* m_ModelHandle;
    std::map<StringHash, Tensor> m_Inputs;
    std::map<StringHash, Tensor> m_Outputs;
};

}
```

### Tensor System

```cpp
namespace SparkLabs {

class Tensor {
public:
    Tensor();
    Tensor(const TensorShape& shape, TensorDataType type);
    Tensor(const Tensor& other);
    Tensor(Tensor&& other) noexcept;

    ~Tensor();

    void Reshape(const TensorShape& shape);
    TensorShape GetShape() const { return m_Shape; }

    void* GetData() { return m_Data; }
    const void* GetData() const { return m_Data; }

    template<typename T>
    T* GetData() { return static_cast<T*>(m_Data); }

    template<typename T>
    const T* GetData() const { return static_cast<T*>(m_Data); }

    size_t GetElementCount() const;
    size_t GetByteSize() const;

    void SetData(const void* data, size_t size);

private:
    TensorShape m_Shape;
    TensorDataType m_Type;
    void* m_Data;
    size_t m_Size;
};

}
```

### ONNX Inference Engine

```cpp
namespace SparkLabs {

class ONNXInferenceEngine {
public:
    ONNXInferenceEngine();
    ~ONNXInferenceEngine();

    bool Initialize(const ONNXModelLoader::ModelInfo& modelInfo);
    void Shutdown();

    TensorRef Infer(const TensorRef& input);
    Vector<TensorRef> InferBatch(const Vector<TensorRef>& inputs);

    void SetExecutionProvider(ExecutionProvider provider);
    ExecutionProvider GetExecutionProvider() const;

    bool IsGPUEnabled() const { return m_GPUEnabled; }

private:
    void* m_Session;
    ExecutionProvider m_Provider;
    boolean m_GPUEnabled;
};

}
```

### Neural Network Usage

```cpp
// Create and load neural network
SmartPtr<NeuralNetwork> network = MakeSmartPtr<NeuralNetwork>();
network->LoadModel("path/to/decision_model.onnx");

// Prepare input tensor
float32 inputData[] = { 1.0f, 0.5f, 0.3f, /* ... */ };
TensorRef input = CreateTensorRef({1, 10}, inputData);

// Run inference
TensorRef output = network->Forward(input);

// Process output
const float32* scores = output.GetData<float32>();
int32 bestAction = std::max_element(scores, scores + numActions) - scores;
```

### GPU Acceleration

The ONNX Runtime integration supports multiple execution providers:

```cpp
enum class ExecutionProvider {
    CPU,        // Default CPU execution
    CUDA,       // NVIDIA GPU via CUDA
    CoreML,     // Apple Silicon GPU via CoreML
    DirectML    // Windows GPU via DirectML
};
```

Enable GPU support in CMake:
```bash
cmake .. -DSPARK_AI_ENABLE_GPU=ON
```

---

## NPC Brain Architecture

### Overview

The NPC Brain system provides advanced AI capabilities for non-player characters, including emotional modeling, attention mechanisms, dialogue generation, and personality traits.

### NPCBrain Component

```cpp
namespace SparkLabs {

struct NPCContext {
    Vector3 position;
    Vector<Agent*> nearbyAgents;
    float32 timeOfDay;
    StringHash locationType;
};

class NPCBrain : public Component {
    DECLARE_RTTI

public:
    NPCBrain();
    virtual ~NPCBrain();

    void OnAttached() override;
    void OnDetached() override;
    void Update(float32 deltaTime) override;

    TensorRef Think(const NPCContext& context);
    String GenerateDialogue(const DialogueContext& context);

    SmartPtr<AIMemory> GetMemory() const { return m_Memory; }
    SmartPtr<AttentionMechanism> GetAttention() const { return m_Attention; }
    SmartPtr<EmotionalState> GetEmotion() const { return m_Emotion; }
    SmartPtr<DialogueGenerator> GetDialogueGenerator() const { return m_DialogueGenerator; }
    SmartPtr<NPCPersonality> GetPersonality() const { return m_Personality; }

    void SetDecisionNetwork(SmartPtr<NeuralNetwork> network);
    void SetDialogueNetwork(SmartPtr<NeuralNetwork> network);
    void SetPersonality(SmartPtr<NPCPersonality> personality);

private:
    void InitializeComponents();
    TensorRef GatherContext(const NPCContext& context);
    TensorRef RunDecisionNetwork(const TensorRef& input);

    SmartPtr<NeuralNetwork> m_DecisionNetwork;
    SmartPtr<NeuralNetwork> m_DialogueNetwork;
    SmartPtr<AIMemory> m_Memory;
    SmartPtr<AttentionMechanism> m_Attention;
    SmartPtr<EmotionalState> m_Emotion;
    SmartPtr<DialogueGenerator> m_DialogueGenerator;
    SmartPtr<NPCPersonality> m_Personality;

    float32 m_ThinkInterval;
    float32 m_ThinkTimer;
    float32 m_DialogueInterval;
    float32 m_DialogueTimer;
};

}
```

### NPC Personality System

```cpp
namespace SparkLabs {

class NPCPersonality {
public:
    enum class Trait {
        Friendly,
        Hostile,
        Greedy,
        Brave,
        Timid,
        Honest,
        Deceitful,
        Patient,
        Impulsive
    };

    void SetTrait(Trait trait, float32 value);
    float32 GetTrait(Trait trait) const;

    float32 GetFriendliness() const;
    float32 GetAggressiveness() const;
    float32 GetGreed() const;

private:
    std::map<Trait, float32> m_Traits;
};

}
```

**Trait Influence:**
- Friendly: Affects dialogue tone and willingness to help
- Hostile: Affects combat behavior and dialogue aggression
- Greedy: Influences trade negotiations and treasure interest
- Brave/Timid: Determines fear responses and challenge acceptance
- Honest/Deceitful: Affects truthfulness in dialogue
- Patient/Impulsive: Influences waiting behavior and spontaneous actions

### Emotional State System

```cpp
namespace SparkLabs {

class EmotionalState {
public:
    enum class Emotion {
        Happy,
        Sad,
        Angry,
        Fearful,
        Surprised,
        Disgusted,
        Neutral
    };

    void SetEmotion(Emotion emotion, float32 intensity);
    Emotion GetCurrentEmotion() const;
    float32 GetEmotionIntensity() const;

    void Update(float32 deltaTime);
    void ApplyStimulus(Emotion stimulus, float32 strength);

private:
    Emotion m_CurrentEmotion;
    float32 m_Intensity;
    std::map<Emotion, float32> m_PendingEmotions;
};

}
```

**Emotion Dynamics:**
- Emotions decay over time back to neutral
- Strong stimuli can trigger rapid emotion transitions
- Multiple emotions can blend together
- Emotional state influences dialogue generation

### Attention Mechanism

```cpp
namespace SparkLabs {

class AttentionMechanism {
public:
    struct AttentionTarget {
        Agent* target;
        float32 attentionWeight;
        Vector3 lastKnownPosition;
        float32 timeSinceLastSeen;
    };

    void AddTarget(Agent* target);
    void RemoveTarget(Agent* target);
    void UpdateTarget(Agent* target, const Vector3& position);

    void Update(float32 deltaTime);

    Vector<const AttentionTarget*> GetTopTargets(size_t count) const;
    Agent* GetMostAttendedTarget() const;

    float32 GetAttentionFor(Agent* target) const;

private:
    std::map<Agent*, AttentionTarget> m_Targets;
    float32 m_DecayRate;
};

}
```

**Attention Dynamics:**
- Attention decays over time when targets are not visible
- Movement increases attention weight
- Nearby objects receive higher baseline attention
- Emotional state can amplify attention to certain stimuli

### Dialogue Generation

```cpp
namespace SparkLabs {

class DialogueGenerator {
public:
    struct DialogueContext {
        Agent* speaker;
        Agent* listener;
        float32 relationship;
        StringHash situation;
        Vector<String> recentTopics;
        EmotionalState::Emotion speakerEmotion;
    };

    String GenerateGreeting(NPCBrain* npc, Agent* player);
    String GenerateResponse(NPCBrain* npc, const String& playerInput);
    String GenerateAmbient(NPCBrain* npc);

private:
    String SelectPhrase(const Vector<String>& candidates);
    float32 CalculateTone(DialogueContext& context);

    Vector<String> m_GreetingPhrases;
    Vector<String> m_AmbientPhrases;
};

}
```

### NPC Brain Processing Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                     NPC Brain Update Loop                          │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  1. Context Gathering (Think Interval)                           │
│     ┌──────────────────────────────────────────────────────────┐ │
│     │  GatherContext()                                          │ │
│     │  - NPC position and nearby agents                         │ │
│     │  - Time of day and location type                         │ │
│     │  - Memory context                                         │ │
│     └──────────────────────────────────────────────────────────┘ │
│                              │                                    │
│                              ▼                                    │
│  2. Attention Update                                             │
│     ┌──────────────────────────────────────────────────────────┐ │
│     │  m_Attention->Update(deltaTime)                          │ │
│     │  - Decay attention weights                               │ │
│     │  - Update visible targets                                │ │
│     │  - Focus on most important stimuli                        │ │
│     └──────────────────────────────────────────────────────────┘ │
│                              │                                    │
│                              ▼                                    │
│  3. Neural Network Inference (Decision Network)                  │
│     ┌──────────────────────────────────────────────────────────┐ │
│     │  RunDecisionNetwork(tensor)                               │ │
│     │  - Process context through neural network                  │ │
│     │  - Generate action probabilities                          │ │
│     │  - Store decision in memory                               │ │
│     └──────────────────────────────────────────────────────────┘ │
│                              │                                    │
│                              ▼                                    │
│  4. Emotion Update                                              │
│     ┌──────────────────────────────────────────────────────────┐ │
│     │  m_Emotion->Update(deltaTime)                            │ │
│     │  - Decay current emotion                                 │ │
│     │  - Apply new stimuli                                     │ │
│     │  - Trigger emotion transitions                            │ │
│     └──────────────────────────────────────────────────────────┘ │
│                              │                                    │
│                              ▼                                    │
│  5. Memory Update                                              │
│     ┌──────────────────────────────────────────────────────────┐ │
│     │  m_Memory->Update(deltaTime)                             │ │
│     │  - Add recent events to memory                           │ │
│     │  - Decay old memories                                   │ │
│     │  - Consolidate important memories                        │ │
│     └──────────────────────────────────────────────────────────┘ │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

### Complete NPC Setup Example

```cpp
SmartPtr<GameObject> merchant = MakeSmartPtr<GameObject>("Merchant");
SmartPtr<NPCBrain> brain = merchant->AddComponent<NPCBrain>();

// Configure personality
SmartPtr<NPCPersonality> personality = MakeSmartPtr<NPCPersonality>();
personality->SetTrait(NPCPersonality::Trait::Friendly, 0.85f);
personality->SetTrait(NPCPersonality::Trait::Greedy, 0.7f);
personality->SetTrait(NPCPersonality::Trait::Honest, 0.9f);
brain->SetPersonality(personality);

// Set up decision neural network
SmartPtr<NeuralNetwork> decisionNet = MakeSmartPtr<NeuralNetwork>();
decisionNet->LoadModel("models/merchant_decision.onnx");
brain->SetDecisionNetwork(decisionNet);

// Set up dialogue neural network
SmartPtr<NeuralNetwork> dialogueNet = MakeSmartPtr<NeuralNetwork>();
dialogueNet->LoadModel("models/merchant_dialogue.onnx");
brain->SetDialogueNetwork(dialogueNet);

// Update in game loop
void MerchantController::Update(float32 deltaTime) {
    NPCContext ctx;
    ctx.position = GetOwner()->GetComponent<Transform>()->GetPosition();
    ctx.timeOfDay = GetGameTime();
    ctx.locationType = StringHash("market");

    // Get nearby agents
    for (auto agent : GetNearbyAgents()) {
        ctx.nearbyAgents.PushBack(agent);
    }

    brain->Update(deltaTime);

    // Generate dialogue if player is nearby
    if (ctx.nearbyAgents.Contains(playerAgent)) {
        DialogueContext dlgCtx;
        dlgCtx.speaker = brain.Get();
        dlgCtx.listener = playerAgent;
        dlgCtx.relationship = GetRelationshipWith(playerAgent);
        dlgCtx.situation = StringHash("trade");

        String dialogue = brain->GenerateDialogue(dlgCtx);
        DisplayDialogue(dialogue);
    }
}
```

---

## Memory System

### Overview

The AI Memory system enables NPCs and AI entities to retain information over time, supporting both short-term and long-term memory with importance-based retention and decay.

### Memory Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              AI Memory System                                │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌───────────────────────────────────────────────────────────────────────┐ │
│  │                          Memory Hierarchy                               │ │
│  │                                                                        │ │
│  │   ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐   │ │
│  │   │    Semantic     │    │     Episodic    │    │     Short       │   │ │
│  │   │    Memory       │    │     Memory      │    │     Term        │   │ │
│  │   │                 │    │                 │    │                 │   │ │
│  │   │  - World facts  │    │  - Event        │    │  - Current      │   │ │
│  │   │  - NPC relations│    │    sequences    │    │    observations │   │ │
│  │   │  - Location info│    │  - Experiences  │    │  - Immediate    │   │ │
│  │   │  - Learned      │    │  - Stories      │    │    context      │   │ │
│  │   │    knowledge    │    │                 │    │                 │   │ │
│  │   └────────┬────────┘    └────────┬────────┘    └────────┬────────┘   │ │
│  │            │                      │                      │            │ │
│  │            └──────────────────────┼──────────────────────┘            │ │
│  │                                   ▼                                     │ │
│  │                    ┌─────────────────────────┐                         │ │
│  │                    │     Consolidation      │                         │ │
│  │                    │         Process        │                         │ │
│  │                    │                         │                         │ │
│  │                    │  Important Short-Term   │                         │ │
│  │                    │  ─────────────────────▶ │ Semantic/Episodic       │ │
│  │                    │                         │                         │ │
│  │                    └─────────────────────────┘                         │ │
│  │                                                                       │ │
│  └───────────────────────────────────────────────────────────────────────┘ │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Memory Entry Structure

```cpp
namespace SparkLabs {

struct AIMemoryEntry {
    StringHash id;
    String content;
    float64 timestamp;
    MemoryType type;
    float64 importance;
    float64 decayRate;
    Vector<StringHash> relatedEntities;
    std::map<StringHash, Variant> metadata;
};

enum class MemoryType {
    ShortTerm,    // Working memory, rapid decay
    LongTerm,     // Persistent memory, slow decay
    Episodic,     // Event sequences, medium decay
    Semantic      // Facts and knowledge, minimal decay
};

}
```

### AIMemory Class

```cpp
namespace SparkLabs {

class AIMemory {
public:
    AIMemory();
    ~AIMemory();

    void AddMemory(const String& content, MemoryType type, float32 importance);
    void AddMemory(const AIMemoryEntry& entry);

    void Update(float32 deltaTime);

    Vector<const AIMemoryEntry*> GetRecentMemories(size_t count) const;
    Vector<const AIMemoryEntry*> GetMemoriesByType(MemoryType type) const;
    Vector<const AIMemoryEntry*> GetMemoriesContaining(const String& keyword) const;

    void Decay(float32 deltaTime);
    void Consolidate();

    void Clear();
    void ClearByType(MemoryType type);

    const Vector<AIMemoryEntry>& GetAllMemories() const { return m_Memories; }

private:
    void RemoveExpiredMemories();
    void NormalizeImportances();

    Vector<AIMemoryEntry> m_Memories;
    float32 m_BaseDecayRate;
    float32 m_ConsolidationThreshold;
};

}
```

### Memory Operations

#### Adding Memory

```cpp
// Add a simple memory
brain->AddToMemory("Player bought a sword", MemoryType::ShortTerm, 0.7f);

// Add a detailed memory entry
AIMemoryEntry entry;
entry.id = StringHash("purchase_001");
entry.content = "Player named 'Hero' purchased Iron Sword for 100 gold";
entry.type = MemoryType::Episodic;
entry.importance = 0.85f;
entry.decayRate = 0.01f;
entry.relatedEntities.PushBack(StringHash("player_hero"));
entry.metadata.Set(StringHash("item"), StringHash("iron_sword"));
entry.metadata.Set(StringHash("price"), 100.0f);

brain->AddMemory(entry);
```

#### Retrieving Memories

```cpp
// Get recent memories
Vector<const AIMemoryEntry*> recent = brain->GetMemory()->GetRecentMemories(5);

// Get memories by type
Vector<const AIMemoryEntry*> facts = brain->GetMemory()->GetMemoriesByType(MemoryType::Semantic);

// Search memories by keyword
Vector<const AIMemoryEntry*> playerMemories =
    brain->GetMemory()->GetMemoriesContaining("Player");
```

#### Memory Decay

```cpp
void AIMemory::Decay(float32 deltaTime) {
    for (auto& entry : m_Memories) {
        // Apply type-specific decay
        float32 decayMultiplier = 1.0f;
        switch (entry.type) {
            case MemoryType::ShortTerm:
                decayMultiplier = 10.0f;
                break;
            case MemoryType::Episodic:
                decayMultiplier = 1.0f;
                break;
            case MemoryType::LongTerm:
                decayMultiplier = 0.1f;
                break;
            case MemoryType::Semantic:
                decayMultiplier = 0.01f;
                break;
        }

        entry.importance -= entry.decayRate * decayMultiplier * deltaTime;
    }

    RemoveExpiredMemories();
}
```

#### Memory Consolidation

```cpp
void AIMemory::Consolidate() {
    for (const auto& entry : m_Memories) {
        if (entry.type == MemoryType::ShortTerm &&
            entry.importance > m_ConsolidationThreshold) {

            // Create long-term version
            AIMemoryEntry consolidated = entry;
            consolidated.type = MemoryType::LongTerm;
            consolidated.importance *= 1.5f;  // Boost after consolidation
            consolidated.decayRate *= 0.5f;

            AddMemory(consolidated);
        }
    }
}
```

### Memory Integration with NPC Brain

```cpp
void NPCBrain::Update(float32 deltaTime) {
    m_ThinkTimer += deltaTime;

    if (m_ThinkTimer >= m_ThinkInterval) {
        m_ThinkTimer = 0.0f;

        // Gather context and think
        NPCContext ctx = GatherContext();
        Think(ctx);

        // Add to memory
        StringHash eventId = StringHash("think_event_") + m_ThinkTimer;
        brain->AddToMemory(
            String("Processed situation at ") + GetCurrentLocation(),
            MemoryType::ShortTerm,
            0.5f
        );
    }

    // Update memory system
    if (m_Memory) {
        m_Memory->Update(deltaTime);

        // Periodic consolidation
        m_ConsolidationTimer += deltaTime;
        if (m_ConsolidationTimer >= 60.0f) {  // Every minute
            m_Memory->Consolidate();
            m_ConsolidationTimer = 0.0f;
        }
    }

    // Update emotional state
    if (m_Emotion) {
        m_Emotion->Update(deltaTime);
    }

    // Update attention
    if (m_Attention) {
        m_Attention->Update(deltaTime);
    }
}
```

### Context Assembly for Neural Networks

```cpp
TensorRef NPCBrain::GatherContext(const NPCContext& context) {
    Vector<float32> contextData;

    // Position encoding (3 values)
    contextData.PushBack(context.position.x);
    contextData.PushBack(context.position.y);
    contextData.PushBack(context.position.z);

    // Time of day (normalized)
    contextData.PushBack(context.timeOfDay / 24.0f);

    // Nearby agent count (clamped)
    contextData.PushBack(static_cast<float32>(
        Math::Min(context.nearbyAgents.Size(), 10u)));

    // Location type encoding
    contextData.PushBack(static_cast<float32>(
        context.locationType.GetHash()) / 4294967295.0f);

    // Emotional state encoding (2 values)
    if (m_Emotion) {
        contextData.PushBack(static_cast<float32>(
            m_Emotion->GetCurrentEmotion()));
        contextData.PushBack(m_Emotion->GetEmotionIntensity());
    } else {
        contextData.PushBack(0.0f);
        contextData.PushBack(0.0f);
    }

    // Memory context (recent memories as features)
    auto recentMemories = m_Memory->GetRecentMemories(5);
    for (size_t i = 0; i < 5; ++i) {
        if (i < recentMemories.Size()) {
            contextData.PushBack(recentMemories[i]->importance);
        } else {
            contextData.PushBack(0.0f);
        }
    }

    return CreateTensorRef({1, static_cast<size_t>(contextData.Size())},
                           contextData.Data());
}
```

---

## System Integration

### AI System Flow Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           Complete AI System Flow                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│   ┌─────────────┐      ┌─────────────┐      ┌─────────────┐                  │
│   │   Game      │      │   Scene     │      │   Entity    │                  │
│   │   Loop      │─────▶│   Update    │─────▶│   Update    │                  │
│   └─────────────┘      └─────────────┘      └──────┬──────┘                  │
│                                                    │                         │
│                                                    ▼                         │
│   ┌─────────────────────────────────────────────────────────────────────┐   │
│   │                         Component Update                              │   │
│   │                                                                       │   │
│   │   ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │   │
│   │   │  AIBrain   │  │  NPCBrain   │  │  Transform │  │   Custom    │  │   │
│   │   │  Update    │  │   Update    │  │   Update   │  │  Component  │  │   │
│   │   └──────┬──────┘  └──────┬──────┘  └─────────────┘  └─────────────┘  │   │
│   │          │                │                                       │   │
│   │          │                │                                        │   │
│   │          ▼                ▼                                        │   │
│   │   ┌─────────────────────────────────────────────────────────────┐   │   │
│   │   │                    AI Subsystems                             │   │   │
│   │   │                                                               │   │   │
│   │   │   ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐       │   │   │
│   │   │   │Behavior │  │ Neural  │  │ Memory  │  │Emotion  │       │   │   │
│   │   │   │  Tree   │─▶│Network  │─▶│ System  │─▶│ System  │       │   │   │
│   │   │   └─────────┘  └─────────┘  └─────────┘  └─────────┘       │   │   │
│   │   │        │                                               │   │   │
│   │   │        ▼                                               │   │   │
│   │   │   ┌─────────────────────────────────────────────────┐   │   │   │
│   │   │   │                  Actions                        │   │   │   │
│   │   │   │  MoveTo  │  Attack  │  Dialogue  │  Custom      │   │   │   │
│   │   │   └─────────────────────────────────────────────────┘   │   │   │
│   │   │                                                               │   │   │
│   │   └───────────────────────────────────────────────────────────────┘   │   │
│   │                                                                       │   │
│   └───────────────────────────────────────────────────────────────────────┘   │
│                                                                              │
│   ┌─────────────┐      ┌─────────────┐      ┌─────────────┐                  │
│   │   Event     │      │   Render    │      │   Present   │                  │
│   │   Bus       │◀─────│   Pipeline  │◀─────│   Frame     │                  │
│   └─────────────┘      └─────────────┘      └─────────────┘                  │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

For more information on system architecture, see [ARCHITECTURE.md](ARCHITECTURE.md). For API details, see [API_REFERENCE.md](API_REFERENCE.md).
