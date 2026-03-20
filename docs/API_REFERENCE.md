# SparkLabs Engine API Reference

This document provides detailed API documentation for the SparkLabs Engine, covering core classes, modules, and usage examples.

## Table of Contents

1. [Core Module](#core-module)
2. [Engine Module](#engine-module)
3. [AI Module](#ai-module)
4. [Render Module](#render-module)

---

## Core Module

### Type Definitions (`core/Types.h`)

Fundamental type definitions used throughout the engine.

```cpp
namespace SparkLabs {
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using float32 = float;
    using float64 = double;

    using boolean = bool;
    using String = std::string;
}
```

### StringHash (`core/StringHash.h`)

Hash-based string identifier for fast lookups.

```cpp
namespace SparkLabs {

class StringHash {
public:
    StringHash() : m_Hash(0) {}
    explicit StringHash(const char* str);
    explicit StringHash(const String& str);

    bool operator==(const StringHash& other) const;
    bool operator!=(const StringHash& other) const;

    uint32 GetHash() const { return m_Hash; }

private:
    uint32 m_Hash;
};

}
```

**Usage Example:**
```cpp
StringHash playerId("Player");
if (playerId == otherHash) {
    // Match found
}
```

### HashMap (`core/Types.h`)

Template container with string hash-based key lookup.

```cpp
template<typename T>
struct HashMap {
    HashMap();

    T* Get(const StringHash& key);
    const T* Get(const StringHash& key) const;

    void Set(const StringHash& key, T* value);
    bool Contains(const StringHash& key) const;
    bool Remove(const StringHash& key);

    void Clear();
    size_t Size() const;
};
```

**Usage Example:**
```cpp
HashMap<Component> components;
components.Set(StringHash("health"), &healthComponent);

Component* comp = components.Get(StringHash("health"));
if (comp) {
    // Found component
}
```

### SmartPtr (`core/SmartPtr.h`)

Reference-counted smart pointer implementation.

```cpp
namespace SparkLabs {

template<typename T>
class SmartPtr {
public:
    SmartPtr();
    SmartPtr(T* ptr);
    SmartPtr(const SmartPtr& other);
    SmartPtr(SmartPtr&& other) noexcept;

    ~SmartPtr();

    SmartPtr& operator=(const SmartPtr& other);
    SmartPtr& operator=(SmartPtr&& other) noexcept;

    T* Get() const { return m_Ptr; }
    T* operator->() const { return m_Ptr; }
    T& operator*() const { return *m_Ptr; }

    boolean IsValid() const { return m_Ptr != nullptr; }
    uint32 GetRefCount() const { return m_RefCount ? *m_RefCount : 0; }

private:
    T* m_Ptr;
    uint32* m_RefCount;
};

template<typename T, typename... Args>
SmartPtr<T> MakeSmartPtr(Args&&... args);

template<typename T>
SmartPtr<T> DynamicCast(const SmartPtrBase& ptr);

}
```

**Usage Example:**
```cpp
// Create a new object with smart pointer
SmartPtr<Scene> scene = MakeSmartPtr<Scene>("MainScene");

// Check validity
if (scene.IsValid()) {
    scene->Update(deltaTime);
}

// Dynamic casting
SmartPtr<Component> component = gameObject->GetComponent<AIBrain>();
SmartPtr<AIBrain> brain = DynamicCast<AIBrain>(component);
```

### Vector (`core/io/Vector.h`)

Dynamic array container with type safety.

```cpp
template<typename T>
class Vector {
public:
    Vector();
    Vector(size_t initialSize);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    void PushBack(const T& value);
    void PushBack(T&& value);

    template<typename... Args>
    T& EmplaceBack(Args&&... args);

    void PopBack();
    void Clear();

    size_t Size() const;
    size_t Capacity() const;

    bool IsEmpty() const { return Size() == 0; }

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T* Data() { return m_Data; }
    const T* Data() const { return m_Data; }
};
```

**Usage Example:**
```cpp
Vector<SmartPtr<GameObject>> enemies;
enemies.PushBack(enemy1);
enemies.PushBack(enemy2);

for (size_t i = 0; i < enemies.Size(); ++i) {
    enemies[i]->Update(deltaTime);
}
```

### Math Types

#### Vector2 (`core/math/Vector2.h`)

```cpp
class Vector2 {
public:
    float32 x, y;

    Vector2();
    Vector2(float32 x, float32 y);

    static float32 Dot(const Vector2& a, const Vector2& b);
    static float32 Cross(const Vector2& a, const Vector2& b);
    static float32 Distance(const Vector2& a, const Vector2& b);
    static float32 DistanceSquared(const Vector2& a, const Vector2& b);

    float32 Length() const;
    float32 LengthSquared() const;

    void Normalize();
    Vector2 Normalized() const;

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float32 scalar) const;
};
```

#### Vector3 (`core/math/Vector3.h`)

```cpp
class Vector3 {
public:
    float32 x, y, z;

    Vector3();
    Vector3(float32 x, float32 y, float32 z);

    static float32 Dot(const Vector3& a, const Vector3& b);
    static Vector3 Cross(const Vector3& a, const Vector3& b);
    static float32 Distance(const Vector3& a, const Vector3& b);

    float32 Length() const;
    void Normalize();

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
};
```

#### Vector4 (`core/math/Vector4.h`)

```cpp
class Vector4 {
public:
    float32 x, y, z, w;

    Vector4();
    Vector4(float32 x, float32 y, float32 z, float32 w);
};
```

#### Matrix4x4 (`core/math/Matrix4x4.h`)

```cpp
class Matrix4x4 {
public:
    Matrix4x4();

    static Matrix4x4 Identity();
    static Matrix4x4 Translation(const Vector3& pos);
    static Matrix4x4 Scale(const Vector3& scale);
    static Matrix4x4 Rotation(const Quaternion& quat);
    static Matrix4x4 Perspective(float32 fov, float32 aspect, float32 near, float32 far);
    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

    Matrix4x4 operator*(const Matrix4x4& other) const;
    Vector3 TransformVector(const Vector3& v) const;
    Vector3 TransformPoint(const Vector3& p) const;

    float32 Determinant() const;
    Matrix4x4 Transposed() const;
    Matrix4x4 Inverse() const;
};
```

#### Quaternion (`core/math/Quaternion.h`)

```cpp
class Quaternion {
public:
    float32 x, y, z, w;

    Quaternion();
    Quaternion(float32 x, float32 y, float32 z, float32 w);

    static Quaternion Identity();
    static Quaternion FromAxisAngle(const Vector3& axis, float32 angle);
    static Quaternion FromEulerAngles(float32 pitch, float32 yaw, float32 roll);
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float32 t);

    Quaternion operator*(const Quaternion& other) const;
    Vector3 operator*(const Vector3& v) const;

    void Normalize();
    Quaternion Normalized() const;

    Vector3 ToEulerAngles() const;
};
```

### Threading (`core/Threading/`)

#### Mutex (`core/Threading/Mutex.h`)

```cpp
class Mutex {
public:
    Mutex();
    ~Mutex();

    void Lock();
    boolean TryLock();
    void Unlock();
};
```

#### Signal (`core/Threading/Signal.h`)

```cpp
class Signal {
public:
    Signal();
    ~Signal();

    void Wait();
    boolean Wait(uint32 timeoutMs);
    void Signal();
    void Reset();
};
```

### SparkObject (`core/SparkObject.h`)

Base class for all engine objects.

```cpp
namespace SparkLabs {

class SparkObject {
public:
    virtual ~SparkObject() = default;
};

}
```

---

## Engine Module

### Scene System

#### Scene (`engine/scene/Scene.h`)

Main container for all game entities.

```cpp
namespace SparkLabs {

class Scene : public SparkObject {
public:
    Scene();
    explicit Scene(const String& name);
    virtual ~Scene();

    const String& GetName() const;
    void SetName(const String& name);

    SmartPtr<GameObject> GetRoot() const;

    void AddEntity(SmartPtr<GameObject> entity);
    void RemoveEntity(SmartPtr<GameObject> entity);

    SmartPtr<GameObject> FindEntity(StringHash id) const;
    SmartPtr<GameObject> FindEntityByName(const String& name) const;
    Vector< SmartPtr<GameObject> > GetEntitiesWithTag(StringHash tag) const;

    template<typename Func>
    void IterateActiveEntities(Func&& func);

    virtual void OnUpdate(float32 deltaTime);
    void Update(float32 deltaTime);

    bool IsActive() const;
    void SetActive(bool active);
};

}
```

**Usage Example:**
```cpp
SmartPtr<Scene> scene = MakeSmartPtr<Scene>("GameScene");

SmartPtr<GameObject> player = MakeSmartPtr<GameObject>("Player");
SmartPtr<GameObject> enemy = MakeSmartPtr<GameObject>("Enemy");

scene->AddEntity(player);
scene->AddEntity(enemy);

// Update all entities
scene->Update(deltaTime);

// Find entities
SmartPtr<GameObject> found = scene->FindEntityByName("Player");
Vector<SmartPtr<GameObject>> enemies = scene->GetEntitiesWithTag(StringHash("enemy"));
```

#### GameObject (`engine/scene/GameObject.h`)

Entity in the scene hierarchy.

```cpp
namespace SparkLabs {

class GameObject : public Node {
public:
    GameObject();
    explicit GameObject(const String& name);
    virtual ~GameObject();

    const String& GetName() const;
    void SetName(const String& name);

    const String& GetTag() const;
    void SetTag(const String& tag);

    bool IsActive() const;
    void SetActive(bool active);

    Scene* GetScene() const;
    void SetScene(Scene* scene);

    template<typename T, typename... Args>
    SmartPtr<T> AddComponent(Args&&... args);

    template<typename T>
    SmartPtr<T> GetComponent() const;

    template<typename T>
    Vector< SmartPtr<T> > GetComponents() const;

    template<typename T>
    bool HasComponent() const;

    SmartPtr<GameObject> FindChildByName(const String& name) const;
    Vector< SmartPtr<GameObject> > FindByTag(const StringHash& tag) const;

    virtual void OnTransformChanged() override;
    void Update(float32 deltaTime);
};

}
```

**Usage Example:**
```cpp
SmartPtr<GameObject> npc = MakeSmartPtr<GameObject>("Guard");
npc->SetTag("enemy");

// Add components
SmartPtr<AIBrain> brain = npc->AddComponent<AIBrain>();
SmartPtr<Transform> transform = npc->AddComponent<Transform>();

// Get components
SmartPtr<AIBrain> foundBrain = npc->GetComponent<AIBrain>();
if (foundBrain) {
    foundBrain->Update(deltaTime);
}

// Check for component
if (npc->HasComponent<AIBrain>()) {
    // NPC has AI
}
```

#### Component (`engine/scene/Component.h`)

Attachable behavior module base class.

```cpp
namespace SparkLabs {

class Component : public SparkObject {
public:
    virtual ~Component() = default;

    virtual void OnAttached();
    virtual void OnDetached();
    virtual void Update(float32 deltaTime);

    Node* GetOwner() const { return m_Owner.Get(); }
    SmartPtr<Node> GetOwnerPtr() const { return m_Owner; }

protected:
    SmartPtr<Node> m_Owner;
};

}
```

#### Transform (`engine/scene/Transform.h`)

Position, rotation, and scale component.

```cpp
namespace SparkLabs {

class Transform : public Component {
public:
    virtual void OnTransformChanged() override;

    const Vector3& GetPosition() const;
    void SetPosition(const Vector3& pos);

    const Quaternion& GetRotation() const;
    void SetRotation(const Quaternion& rot);

    const Vector3& GetScale() const;
    void SetScale(const Vector3& scale);

    Matrix4x4 GetWorldMatrix() const;
    Matrix4x4 GetLocalMatrix() const;

    Vector3 GetForward() const;
    Vector3 GetUp() const;
    Vector3 GetRight() const;
};

}
```

### NPC System

#### NPCBrain (`engine/npc/NPCBrain.h`)

Central AI controller for non-player characters.

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

    SmartPtr<AIMemory> GetMemory() const;
    SmartPtr<AttentionMechanism> GetAttention() const;
    SmartPtr<EmotionalState> GetEmotion() const;
    SmartPtr<DialogueGenerator> GetDialogueGenerator() const;
    SmartPtr<NPCPersonality> GetPersonality() const;

    void SetDecisionNetwork(SmartPtr<NeuralNetwork> network);
    void SetDialogueNetwork(SmartPtr<NeuralNetwork> network);
    void SetPersonality(SmartPtr<NPCPersonality> personality);
};

}
```

**Usage Example:**
```cpp
SmartPtr<GameObject> merchant = MakeSmartPtr<GameObject>("Merchant");
SmartPtr<NPCBrain> brain = merchant->AddComponent<NPCBrain>();

// Set up personality
SmartPtr<NPCPersonality> personality = MakeSmartPtr<NPCPersonality>();
personality->SetTrait(NPCPersonality::Trait::Friendly, 0.9f);
personality->SetTrait(NPCPersonality::Trait::Greedy, 0.3f);
brain->SetPersonality(personality);

// Set up neural networks
brain->SetDecisionNetwork(decisionNet);
brain->SetDialogueNetwork(dialogueNet);

// Generate dialogue
DialogueContext ctx;
ctx.playerRelation = 0.8f;
ctx.situation = "greeting";

String dialogue = brain->GenerateDialogue(ctx);
```

#### NPCPersonality (`engine/npc/NPCPersonality.h`)

Personality trait management for NPCs.

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

#### EmotionalState (`engine/npc/EmotionalState.h`)

Manages NPC emotions and mood transitions.

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

#### AIMemory (`engine/npc/AIMemory.h`)

Long-term and short-term memory storage for NPCs.

```cpp
namespace SparkLabs {

class AIMemory {
public:
    enum class MemoryType {
        ShortTerm,
        LongTerm,
        Episodic,
        Semantic
    };

    void AddMemory(const String& content, MemoryType type, float32 importance);
    void Update(float32 deltaTime);

    Vector<const AIMemoryEntry*> GetRecentMemories(size_t count) const;
    Vector<const AIMemoryEntry*> GetMemoriesByType(MemoryType type) const;

    void Decay(float32 deltaTime);
    void Consolidate();
};

}
```

### Narrative System

#### StoryGraph (`engine/narrative/StoryGraph.h`)

Branching narrative structure management.

```cpp
namespace SparkLabs {

struct StoryNode {
    StringHash id;
    StringHash type;
    String content;
    Vector<StringHash> possibleNext;
    std::map<StringHash, Variant> properties;
};

enum class NodeType { Beginning, PlotPoint, Choice, Climax, Resolution, Branch };

class StoryGraph {
public:
    StoryGraph();
    ~StoryGraph();

    SmartPtr<StoryNode> GetNode(StringHash id);
    Vector< SmartPtr<StoryNode> > GetNextNodes(StringHash currentId);
    void AddNode(const StoryNode& node);
    void Connect(StringHash from, StringHash to);
    SmartPtr<StoryNode> Traverse(const StoryDecision& decision);

    struct StoryDecision {
        Vector<StringHash> availableChoices;
        StringHash selectedChoice;
        Vector<String> context;
    };

    bool HasNode(StringHash id) const;
    void Clear();
    size_t GetNodeCount() const;
};

}
```

**Usage Example:**
```cpp
StoryGraph story;

// Add story nodes
StoryNode beginning;
beginning.id = StringHash("intro");
beginning.type = StringHash("Beginning");
beginning.content = "The hero arrives at the village...";
story.AddNode(beginning);

StoryNode choice;
choice.id = StringHash("choice1");
choice.type = StringHash("Choice");
choice.content = "Should the hero help the villagers?";
story.AddNode(choice);

// Connect nodes
story.Connect(StringHash("intro"), StringHash("choice1"));

// Traverse story based on player choice
StoryGraph::StoryDecision decision;
decision.availableChoices.PushBack(StringHash("help"));
decision.availableChoices.PushBack(StringHash("ignore"));
decision.selectedChoice = StringHash("help");

SmartPtr<StoryNode> nextNode = story.Traverse(decision);
```

#### ProceduralQuestGenerator (`engine/narrative/ProceduralQuestGenerator.h`)

Automatic quest creation system.

```cpp
namespace SparkLabs {

class ProceduralQuestGenerator {
public:
    struct QuestTemplate {
        StringHash id;
        StringHash type;
        Vector<StringHash> requiredTags;
        float32 difficulty;
    };

    void AddTemplate(const QuestTemplate& template);
    SmartPtr<Quest> GenerateQuest(const Vector<StringHash>& contextTags);
    SmartPtr<Quest> GenerateQuestWithDifficulty(float32 targetDifficulty);

private:
    Vector<QuestTemplate> m_Templates;
};

}
```

### Gameplay System

#### ChallengeBalancing (`engine/gameplay/ChallengeBalancing.h`)

Dynamic difficulty adjustment based on player performance.

```cpp
namespace SparkLabs {

class ChallengeBalancing {
public:
    ChallengeBalancing();
    ~ChallengeBalancing();

    void AddParameter(const AdaptiveParameter& param);
    void RemoveParameter(AdaptiveParameter::ParameterType type);

    void SetFeedbackLoop(PlayerFeedbackLoop* feedback);
    void Update(float32 deltaTime);

    float32 GetDifficulty() const;
    void SetTargetDifficulty(float32 difficulty);

    void Reset();

private:
    Vector<AdaptiveParameter> m_Parameters;
    PlayerFeedbackLoop* m_FeedbackLoop;
    float32 m_TargetDifficulty;
};

}
```

**Usage Example:**
```cpp
ChallengeBalancing balancing;
balancing.AddParameter(AdaptiveParameter::ParameterType::EnemyCount, 10, 5, 20);
balancing.AddParameter(AdaptiveParameter::ParameterType::EnemyHealth, 100.0f, 50.0f, 200.0f);

PlayerFeedbackLoop feedback;
feedback.SetTargetEngagement(0.75f);
balancing.SetFeedbackLoop(&feedback);

// In game loop
feedback.RecordPlayerAction(PlayerFeedbackLoop::Action::TakeDamage, 0.1f);
feedback.RecordPlayerAction(PlayerFeedbackLoop::Action::DefeatEnemy, 1.0f);
balancing.Update(deltaTime);

// Read adjusted values
float32 enemyCount = balancing.GetParameter(AdaptiveParameter::ParameterType::EnemyCount);
```

#### AdaptiveParameter (`engine/gameplay/AdaptiveParameter.h`)

Individual tunable game parameter that adapts to player performance.

```cpp
namespace SparkLabs {

class AdaptiveParameter {
public:
    enum class ParameterType {
        EnemyCount,
        EnemyHealth,
        EnemyDamage,
        PuzzleComplexity,
        TimeLimit,
        HintFrequency,
        ResourceDropRate
    };

    AdaptiveParameter(ParameterType type, float32 baseValue, float32 minValue, float32 maxValue);

    void Adjust(float32 delta);
    void Reset();

    ParameterType GetType() const { return m_Type; }
    float32 GetBaseValue() const { return m_BaseValue; }
    float32 GetCurrentValue() const { return m_CurrentValue; }
    float32 GetMinValue() const { return m_MinValue; }
    float32 GetMaxValue() const { return m_MaxValue; }

private:
    ParameterType m_Type;
    float32 m_BaseValue;
    float32 m_CurrentValue;
    float32 m_MinValue;
    float32 m_MaxValue;
};

}
```

#### PlayerFeedbackLoop (`engine/gameplay/PlayerFeedbackLoop.h`)

Real-time player response tracking and analysis.

```cpp
namespace SparkLabs {

class PlayerFeedbackLoop {
public:
    enum class Action {
        DefeatEnemy,
        TakeDamage,
        CompletePuzzle,
        FailPuzzle,
        FindSecret,
        MissOpportunity,
        UseItem,
        CustomAction
    };

    void RecordPlayerAction(Action action, float32 impact);
    void Update(float32 deltaTime);

    void SetTargetEngagement(float32 target);
    float32 GetCurrentEngagement() const;

    float32 GetSuccessRate() const;
    float32 GetFrustrationLevel() const;

private:
    float32 CalculateEngagement();
    std::map<Action, Vector<float32>> m_ActionHistory;
    float32 m_TargetEngagement;
    float32 m_CurrentEngagement;
};

}
```

---

## AI Module

### AIBrain (`ai/brain/AIBrain.h`)

Main AI controller component attached to game objects.

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

    SmartPtr<AINode> GetRootNode() const;
    void SetRootNode(SmartPtr<AINode> root);

    SmartPtr<AIBlackboard> GetBlackboard() const;
    SmartPtr<AIEventBus> GetEventBus() const;
    SmartPtr<NeuralNetwork> GetNeuralNetwork() const;

    void SetNeuralNetwork(SmartPtr<NeuralNetwork> network);

    TensorRef Think(const TensorRef& input);

    const Vector<AIMemoryEntry>& GetMemory() const;
    void AddMemory(const AIMemoryEntry& entry);
    void ClearMemory();

    void AddToMemory(const String& content, MemoryType type, float64 importance = 0.5);
};

}
```

**Usage Example:**
```cpp
SmartPtr<GameObject> enemy = MakeSmartPtr<GameObject>("Enemy");
SmartPtr<AIBrain> brain = enemy->AddComponent<AIBrain>();

// Set up neural network
SmartPtr<NeuralNetwork> network = MakeSmartPtr<NeuralNetwork>();
network->LoadModel("path/to/model.onnx");
brain->SetNeuralNetwork(network);

// Add to memory
brain->AddToMemory("Player spotted at location X", MemoryType::ShortTerm, 0.9f);

// Process input through neural network
TensorRef input = CreateTensor({1, 10}, inputData);
TensorRef output = brain->Think(input);
```

### AIBlackboard (`ai/brain/AIBlackboard.h`)

Shared data repository for AI systems.

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

### AIEventBus (`ai/brain/AIEventBus.h`)

Inter-AI communication system.

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

### NeuralNetwork (`ai/brain/NeuralNetwork.h`)

Neural network wrapper for AI processing.

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

### Behavior Tree (`ai/behavior/`)

#### BehaviorTree (`ai/behavior/BehaviorTree.h`)

Hierarchical task execution structure.

```cpp
namespace SparkLabs {

class BehaviorTree {
public:
    BehaviorTree();
    ~BehaviorTree();

    SmartPtr<BehaviorTreeNode> GetRoot() const;
    void SetRoot(SmartPtr<BehaviorTreeNode> root);

    Agent* GetAgent() const;
    void SetAgent(Agent* agent);

    void Execute(float32 deltaTime);
    void Reset();
    NodeStatus Tick(Agent* agent, float32 deltaTime);

    bool IsRunning() const { return m_Running; }
    void Stop() { m_Running = false; }

private:
    SmartPtr<BehaviorTreeNode> m_Root;
    Agent* m_Agent;
    bool m_Running;
};

}
```

#### BehaviorTreeNode (`ai/behavior/BehaviorTreeNode.h`)

Base class for all behavior tree nodes.

```cpp
namespace SparkLabs {

enum class NodeStatus {
    Success,
    Failure,
    Running
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

#### CompositeNodes (`ai/behavior/CompositeNodes.h`)

Control flow nodes: Sequence, Selector, Parallel.

```cpp
namespace SparkLabs {

class SequenceNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

class SelectorNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

class ParallelNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

    void SetSuccessThreshold(float32 threshold);
    void SetFailureThreshold(float32 threshold);

private:
    float32 m_SuccessThreshold;
    float32 m_FailureThreshold;
};

class RandomSelectorNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

}
```

#### DecoratorNodes (`ai/behavior/DecoratorNodes.h`)

Nodes that modify child node behavior.

```cpp
namespace SparkLabs {

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

class InverterNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

class SucceederNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

class FailerNode : public BehaviorTreeNode {
public:
    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;
};

class ConditionNode : public BehaviorTreeNode {
public:
    using ConditionFunc = std::function<bool(Agent*)>;

    ConditionNode(ConditionFunc condition);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    ConditionFunc m_Condition;
};

}
```

#### ActionNodes (`ai/behavior/ActionNodes.h`)

Leaf nodes that perform actions.

```cpp
namespace SparkLabs {

class MoveToAction : public BehaviorTreeNode {
public:
    MoveToAction(const Vector3& target);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    Vector3 m_Target;
};

class AttackAction : public BehaviorTreeNode {
public:
    AttackAction(Agent* target);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    Agent* m_Target;
};

class WaitAction : public BehaviorTreeNode {
public:
    WaitAction(float32 duration);

    virtual NodeStatus Execute(Agent* agent, float32 deltaTime) override;

private:
    float32 m_Duration;
    float32 m_Elapsed;
};

}
```

#### Agent (`ai/behavior/Agent.h`)

AI actor that executes behavior trees.

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

### ONNX Runtime (`ai/onnx/`)

#### ONNXInferenceEngine (`ai/onnx/ONNXInferenceEngine.h`)

Neural network inference wrapper.

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

#### Tensor (`ai/onnx/Tensor.h`)

Multi-dimensional array for neural network data.

```cpp
namespace SparkLabs {

class Tensor {
public:
    Tensor();
    Tensor(const TensorShape& shape, TensorDataType type);
    Tensor(const Tensor& other);
    Tensor(Tensor&& other) noexcept;

    ~Tensor();

    Tensor& operator=(const Tensor& other);
    Tensor& operator=(Tensor&& other) noexcept;

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

---

## Render Module

### RenderContext (`render/RenderContext.h`)

Rendering state and configuration.

```cpp
namespace SparkLabs {

class Camera;

struct Light {
    Vector3 position;
    float32 intensity;
    Vector3 color;
};

class RenderContext {
public:
    RenderContext();
    ~RenderContext();

    void SetCamera(Camera* camera);
    Camera* GetCamera() const { return m_Camera; }

    void AddLight(const Light& light);
    void RemoveLight(int32 index);
    const Vector<Light>& GetLights() const { return m_Lights; }

    void SetViewProjection(const Matrix4x4& vp);
    const Matrix4x4& GetViewProjection() const { return m_ViewProjection; }

    void SetInvViewProjection(const Matrix4x4& invvp);
    const Matrix4x4& GetInvViewProjection() const { return m_InvViewProjection; }

    void Clear();

private:
    Camera* m_Camera;
    Vector<Light> m_Lights;
    Matrix4x4 m_ViewProjection;
    Matrix4x4 m_InvViewProjection;
};

}
```

### Material (`render/Material.h`)

Surface appearance definition.

```cpp
namespace SparkLabs {

class Material {
public:
    Material();
    ~Material();

    void SetShader(SmartPtr<Shader> shader);
    SmartPtr<Shader> GetShader() const { return m_Shader; }

    void SetTexture(const String& name, SmartPtr<Texture> texture);
    SmartPtr<Texture> GetTexture(const String& name) const;

    void SetUniform(const String& name, const Variant& value);
    bool GetUniform(const String& name, Variant& outValue) const;

    void SetAlbedo(const Vector3& color);
    Vector3 GetAlbedo() const { return m_Albedo; }

    void SetMetallic(float32 metallic);
    float32 GetMetallic() const { return m_Metallic; }

    void SetRoughness(float32 roughness);
    float32 GetRoughness() const { return m_Roughness; }

private:
    SmartPtr<Shader> m_Shader;
    std::map<String, SmartPtr<Texture>> m_Textures;
    std::map<String, Variant> m_Uniforms;
    Vector3 m_Albedo;
    float32 m_Metallic;
    float32 m_Roughness;
};

}
```

### Mesh (`render/Mesh.h`)

3D geometry data structure.

```cpp
namespace SparkLabs {

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVertices(Vector<Vector3> vertices);
    const Vector<Vector3>& GetVertices() const { return m_Vertices; }

    void SetIndices(Vector<uint32> indices);
    const Vector<uint32>& GetIndices() const { return m_Indices; }

    void SetNormals(Vector<Vector3> normals);
    const Vector<Vector3>& GetNormals() const { return m_Normals; }

    void SetUVs(Vector<Vector2> uvs);
    const Vector<Vector2>& GetUVs() const { return m_UVs; }

    void CalculateNormals();
    void CalculateTangents();

    size_t GetVertexCount() const { return m_Vertices.Size(); }
    size_t GetIndexCount() const { return m_Indices.Size(); }

private:
    Vector<Vector3> m_Vertices;
    Vector<uint32> m_Indices;
    Vector<Vector3> m_Normals;
    Vector<Vector2> m_UVs;
    Vector<Vector4> m_Tangents;
};

}
```

### Shader (`render/Shader.h`)

GPU program management.

```cpp
namespace SparkLabs {

class Shader {
public:
    enum class Type {
        Vertex,
        Fragment,
        Compute,
        Geometry
    };

    Shader(Type type);
    ~Shader();

    bool LoadFromFile(const String& path);
    bool LoadFromSource(const String& source);

    Type GetType() const { return m_Type; }
    const String& GetSource() const { return m_Source; }

    bool IsCompiled() const { return m_Compiled; }
    const String& GetCompileError() const { return m_Error; }

private:
    Type m_Type;
    String m_Source;
    bool m_Compiled;
    String m_Error;
};

}
```

### Texture (`render/Texture.h`)

Image and buffer textures.

```cpp
namespace SparkLabs {

class Texture {
public:
    enum class Format {
        R8,
        RG8,
        RGB8,
        RGBA8,
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,
        R32F,
        RGBA32F,
        Depth24,
        Depth32F
    };

    enum class Filter {
        Nearest,
        Linear,
        Trilinear
    };

    enum class WrapMode {
        Repeat,
        Clamp,
        Mirror
    };

    Texture();
    ~Texture();

    bool LoadFromFile(const String& path);
    bool LoadFromMemory(const uint8* data, size_t width, size_t height, Format format);

    void SetFilter(Filter filter);
    void SetWrapMode(WrapMode mode);

    size_t GetWidth() const { return m_Width; }
    size_t GetHeight() const { return m_Height; }
    Format GetFormat() const { return m_Format; }

private:
    size_t m_Width;
    size_t m_Height;
    Format m_Format;
    Filter m_Filter;
    WrapMode m_WrapMode;
};

}
```

### Neural Rendering (`render/neural/`)

#### NeuralAmbientOcclusion (`render/neural/NeuralAmbientOcclusion.h`)

AI-powered ambient occlusion.

```cpp
namespace SparkLabs {

class NeuralAmbientOcclusion {
public:
    NeuralAmbientOcclusion();
    ~NeuralAmbientOcclusion();

    void Initialize(SmartPtr<RenderContext> context);

    void SetIntensity(float32 intensity);
    float32 GetIntensity() const { return m_Intensity; }

    void SetRadius(float32 radius);
    float32 GetRadius() const { return m_Radius; }

    SmartPtr<Texture> Process(SmartPtr<Texture> depthTexture, SmartPtr<Texture> normalTexture);

private:
    SmartPtr<RenderContext> m_Context;
    SmartPtr<NeuralNetwork> m_Network;
    float32 m_Intensity;
    float32 m_Radius;
};

}
```

#### NeuralAntiAlias (`render/neural/NeuralAntiAlias.h`)

Intelligent edge smoothing.

```cpp
namespace SparkLabs {

class NeuralAntiAlias {
public:
    NeuralAntiAlias();
    ~NeuralAntiAlias();

    void Initialize(SmartPtr<RenderContext> context);

    SmartPtr<Texture> Process(SmartPtr<Texture> inputTexture);

    void SetStrength(float32 strength);
    float32 GetStrength() const { return m_Strength; }

private:
    SmartPtr<RenderContext> m_Context;
    SmartPtr<NeuralNetwork> m_Network;
    float32 m_Strength;
};

}
```

#### NeuralUpscaleEffect (`render/neural/NeuralUpscaleEffect.h`)

AI-based resolution upscaling.

```cpp
namespace SparkLabs {

class NeuralUpscaleEffect {
public:
    NeuralUpscaleEffect();
    ~NeuralUpscaleEffect();

    void Initialize(SmartPtr<RenderContext> context);

    SmartPtr<Texture> Upscale(SmartPtr<Texture> input, size_t targetWidth, size_t targetHeight);

    void SetSharpness(float32 sharpness);
    float32 GetSharpness() const { return m_Sharpness; }

private:
    SmartPtr<RenderContext> m_Context;
    SmartPtr<NeuralNetwork> m_Network;
    float32 m_Sharpness;
};

}
```

---

## Common Patterns

### Component Attachment Pattern

```cpp
// Create game object
SmartPtr<GameObject> entity = MakeSmartPtr<GameObject>("Entity");

// Attach multiple components
SmartPtr<Transform> transform = entity->AddComponent<Transform>();
SmartPtr<AIBrain> brain = entity->AddComponent<AIBrain>();
SmartPtr<MeshRenderer> renderer = entity->AddComponent<MeshRenderer>();

// Configure components
transform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
brain->SetNeuralNetwork(network);
renderer->SetMesh(mesh);
renderer->SetMaterial(material);
```

### AI Behavior Tree Setup

```cpp
// Create behavior tree
SmartPtr<BehaviorTree> tree = MakeSmartPtr<BehaviorTree>();

// Create root selector
SmartPtr<SelectorNode> root = MakeSmartPtr<SelectorNode>();

// Create sequence for combat
SmartPtr<SequenceNode> combatSequence = MakeSmartPtr<SequenceNode>();
combatSequence->AddChild(MakeSmartPtr<ConditionNode>(IsEnemyNearby));
combatSequence->AddChild(MakeSmartPtr<MoveToAction>(enemyPos));
combatSequence->AddChild(MakeSmartPtr<AttackAction>(enemy));

// Create sequence for patrol
SmartPtr<SequenceNode> patrolSequence = MakeSmartPtr<SequenceNode>();
patrolSequence->AddChild(MakeSmartPtr<MoveToAction>(patrolPoint));
patrolSequence->AddChild(MakeSmartPtr<WaitAction>(2.0f));

// Add to root
root->AddChild(combatSequence);
root->AddChild(patrolSequence);

tree->SetRoot(root);
agent->AttachBehaviorTree(tree);
```

### Scene Iteration Pattern

```cpp
scene->IterateActiveEntities([](SmartPtr<GameObject> entity) {
    // Update each entity
    entity->Update(deltaTime);

    // Find specific components
    SmartPtr<AIBrain> brain = entity->GetComponent<AIBrain>();
    if (brain) {
        brain->Update(deltaTime);
    }
});
```

---

For additional support, please refer to the [Architecture Documentation](ARCHITECTURE.md) and [AI System Documentation](AI_SYSTEM.md).
