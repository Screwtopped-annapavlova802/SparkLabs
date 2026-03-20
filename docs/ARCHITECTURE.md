# SparkLabs Engine Architecture

This document provides an in-depth look at the system architecture of SparkLabs Engine, including module relationships, AI system design, and rendering pipeline overview.

## Table of Contents

1. [System Architecture Diagram](#system-architecture-diagram)
2. [Module Relationships](#module-relationships)
3. [AI System Design](#ai-system-design)
4. [Rendering Pipeline](#rendering-pipeline)

---

## System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              SparkLabs Engine                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐          │
│  │     Editor      │    │    Scripts      │    │    Platform     │          │
│  └────────┬────────┘    └────────┬────────┘    └────────┬────────┘          │
│           │                      │                      │                    │
│           └──────────────────────┼──────────────────────┘                    │
│                                  │                                            │
│  ┌────────────────────────────────┴────────────────────────────────┐         │
│  │                         Engine Core                              │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │    Scene     │  │   Gameplay   │  │  Narrative   │            │         │
│  │  │   System     │  │    System    │  │   System     │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │     NPC      │  │    Asset     │  │   Resource   │            │         │
│  │  │   System     │  │   Pipeline   │  │   Manager    │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  └─────────────────────────────────┬───────────────────────────────┘         │
│                                    │                                          │
│  ┌─────────────────────────────────┴───────────────────────────────┐         │
│  │                          AI Runtime                              │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │    Brain      │  │    Neural    │  │    ONNX      │            │         │
│  │  │   System     │  │   Network    │  │   Runtime    │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │    Behavior   │  │    AI        │  │   Tensor     │            │         │
│  │  │    Trees     │  │   Memory     │  │   Engine     │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  └─────────────────────────────────┬───────────────────────────────┘         │
│                                    │                                          │
│  ┌─────────────────────────────────┴───────────────────────────────┐         │
│  │                       Render Pipeline                             │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │   Classical   │  │    Neural   │  │    Post      │            │         │
│  │  │   Render      │  │   Render     │  │   Process    │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  └─────────────────────────────────┬───────────────────────────────┘         │
│                                    │                                          │
│  ┌─────────────────────────────────┴───────────────────────────────┐         │
│  │                         Core Utilities                            │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │     Math     │  │    Memory    │  │   Threading   │           │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐            │         │
│  │  │   Strings    │  │    Object    │  │     I/O      │            │         │
│  │  └──────────────┘  └──────────────┘  └──────────────┘            │         │
│  └──────────────────────────────────────────────────────────────────┘         │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Module Relationships

### Dependency Hierarchy

```
Level 5: Applications (Editor, Scripts, Platform)
         │
         ▼
Level 4: Engine Systems (Scene, NPC, Narrative, Gameplay, Asset)
         │
         ▼
Level 3: AI Runtime (Brain, Neural Network, Behavior Trees, ONNX)
         │
         ▼
Level 2: Render Pipeline (Classical, Neural, Post-Process)
         │
         ▼
Level 1: Core Utilities (Math, Memory, Threading, Strings, I/O)
```

### Core Module (`core/`)

The foundation layer providing fundamental building blocks:

```
core/
├── Types.h           ──► All basic type definitions
├── SparkObject.h     ──► Base class for all engine objects
├── SmartPtr.h        ──► Reference-counted smart pointers
├── HashMap.h          ──► Hash-based container (in Types.h)
│
├── math/
│   ├── Vector2.h     ──► 2D vector operations
│   ├── Vector3.h     ──► 3D vector operations
│   ├── Vector4.h     ──► 4D vector operations
│   ├── Matrix4x4.h   ──► 4x4 matrix for transformations
│   └── Quaternion.h  ──► Quaternion for rotations
│
├── memory/
│   └── SmartPtr.cpp  ──► Smart pointer implementation
│
├── threading/
│   ├── Mutex.h       ──► Mutual exclusion
│   ├── Signal.h      ──► Event signaling
│   └── ThreadID.h    ──► Thread identification
│
├── string/
│   ├── String.h      ──► String utilities
│   ├── StringHash.h  ──► String hashing
│   └── StringHash.cpp
│
└── io/
    └── Vector.h      ──► Dynamic array container
```

**Key Dependencies:**
- `Types.h` is included by all other core headers
- No dependencies on other engine modules

### Engine Module (`engine/`)

Game systems built on top of core:

```
engine/
├── scene/                  ──► Scene Graph System
│   ├── Scene.h             ──► Scene container
│   ├── GameObject.h        ──► Entity in scene
│   ├── Component.h         ──► Attachable behavior
│   ├── Transform.h          ──► Position/rotation/scale
│   └── Node.h              ──► Hierarchy node
│
├── npc/                    ──► NPC AI System
│   ├── NPCBrain.h          ──► NPC central controller
│   ├── NPCPersonality.h    ──► Personality traits
│   ├── EmotionalState.h   ──► Emotion management
│   ├── AIMemory.h          ──► Memory system
│   ├── AttentionMechanism.h──► Attention model
│   └── DialogueGenerator.h──► Dialogue creation
│
├── narrative/              ──► Story System
│   ├── StoryGraph.h        ──► Story structure
│   ├── StoryArc.h          ──► Story arc
│   ├── BranchingNarrative.h──► Branching paths
│   └── ProceduralQuestGenerator.h
│
├── gameplay/               ──► Adaptive Gameplay
│   ├── ChallengeBalancing.h──► Difficulty adjustment
│   ├── AdaptiveParameter.h──► Tunable parameters
│   ├── PlayerModel.h        ──► Player modeling
│   └── PlayerFeedbackLoop.h──► Feedback tracking
│
├── asset/                  ──► Asset Pipeline
│   ├── AssetPipeline.h      ──► Processing pipeline
│   ├── AssetCache.h         ──► Caching system
│   └── PromptToAsset.h      ──► AI asset generation
│
└── resource/              ──► Resource Management
    ├── ResourceLoader.h     ──► Resource loading
    └── ResourceCache.h      ──► Resource caching
```

**Dependencies:**
- All engine classes depend on `core/` types
- Scene system is the central hub
- NPC and gameplay systems use Scene to find entities

### AI Module (`ai/`)

Intelligence systems integrated with engine:

```
ai/
├── brain/                  ──► AI Brain System
│   ├── AIBrain.h           ──► Main AI controller
│   ├── AINode.h            ──► AI processing node
│   ├── AIBlackboard.h      ──► Shared data
│   ├── AIEventBus.h        ──► Inter-AI events
│   ├── NeuralNetwork.h     ──► NN wrapper
│   └── AIMemoryEntry.h     ──► Memory entries
│
├── behavior/               ──► Behavior Tree System
│   ├── BehaviorTree.h      ──► Tree container
│   ├── BehaviorTreeNode.h  ──► Base node
│   ├── CompositeNodes.h    ──► Sequence/Selector/Parallel
│   ├── DecoratorNodes.h    ──► Modifiers
│   ├── ActionNodes.h       ──► Action leaves
│   └── Agent.h             ──► Behavior executor
│
└── onnx/                   ──► ONNX Runtime Integration
    ├── ONNXInferenceEngine.h──► Inference wrapper
    ├── ONNXModelLoader.h   ──► Model loading
    ├── Tensor.h             ──► Tensor data
    ├── GPUSupport.h         ──► GPU acceleration
    └── MemoryPool.h         ──► Memory optimization
```

**Dependencies:**
- AI brain inherits from `engine/scene/Component.h`
- Behavior trees use `core/` types extensively
- ONNX system is optional (controlled by `SPARK_AI_ENABLE_ONNX`)

### Render Module (`render/`)

Graphics and neural rendering:

```
render/
├── RenderContext.h        ──► Rendering state
├── Material.h             ──► Surface material
├── Mesh.h                 ──► 3D geometry
├── Shader.h               ──► Shader programs
├── Texture.h              ──► Image textures
│
└── neural/                ──► Neural Rendering
    ├── NeuralAmbientOcclusion.h
    ├── NeuralAntiAlias.h
    └── NeuralUpscaleEffect.h
```

**Dependencies:**
- Uses `core/math/` for transformation matrices
- Materials reference textures and shaders
- Neural rendering uses AI neural network integration

---

## AI System Design

### AI System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                           Game Object                                │
│  ┌───────────────────────────────────────────────────────────────┐ │
│  │                        NPCBrain Component                       │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐│ │
│  │  │  Personality │  │   Emotion   │  │      Attention          ││ │
│  │  │   Module    │  │   Module    │  │       Module              ││ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────────┘│ │
│  │                                                               │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐│ │
│  │  │   Memory    │  │   Dialogue │  │      Decision           ││ │
│  │  │   Module    │  │  Generator  │  │       Network           ││ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────────┘│ │
│  └───────────────────────────────────────────────────────────────┘ │
│                               │                                      │
│                               ▼                                      │
│  ┌───────────────────────────────────────────────────────────────┐ │
│  │                         AIBrain                                │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐│ │
│  │  │   Black-    │  │    Event    │  │      Neural             ││ │
│  │  │   board     │  │     Bus     │  │      Network            ││ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────────┘│ │
│  │                                                               │ │
│  │  ┌─────────────────────────────────────────────────────────────┐│ │
│  │  │                    Behavior Tree                            ││ │
│  │  │  ┌─────────┐    ┌─────────┐    ┌─────────┐                 ││ │
│  │  │  │ Sequence │───▶│Selector │───▶│ Parallel│                 ││ │
│  │  │  └─────────┘    └─────────┘    └─────────┘                 ││ │
│  │  │       │              │              │                       ││ │
│  │  │  ┌────┴────┐    ┌────┴────┐    ┌────┴────┐                  ││ │
│  │  │  │ Action  │    │ Action  │    │Decorator│                  ││ │
│  │  │  └─────────┘    └─────────┘    └─────────┘                  ││ │
│  │  └─────────────────────────────────────────────────────────────┘│ │
│  └───────────────────────────────────────────────────────────────┘ │
│                               │                                      │
│                               ▼                                      │
│  ┌───────────────────────────────────────────────────────────────┐ │
│  │                      ONNX Runtime                               │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐│ │
│  │  │    CPU      │  │    GPU      │  │       Tensor              ││ │
│  │  │   Provider  │  │   Provider  │  │       Engine              ││ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────────┘│ │
│  └───────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────┘
```

### AI Brain Processing Flow

```
1. Perception Phase
   ┌──────────────────┐
   │  Gather Context  │
   │  (position,      │
   │   agents, time)  │
   └────────┬─────────┘
            │
            ▼
2. Attention Phase
   ┌──────────────────┐
   │ Filter & Focus   │
   │ Relevant Info    │
   └────────┬─────────┘
            │
            ▼
3. Decision Phase
   ┌──────────────────┐     ┌──────────────────┐
   │  Behavior Tree   │ OR  │   Neural Network │
   │     Ticking       │     │    Inference     │
   └────────┬─────────┘     └────────┬─────────┘
            │                        │
            └───────────┬────────────┘
                        │
                        ▼
4. Action Phase
   ┌──────────────────┐
   │ Execute Decision │
   │ Update Memory    │
   │ Generate Output  │
   └──────────────────┘
```

### Behavior Tree Structure

```
                    ┌─────────────┐
                    │   Selector  │  (Root - try strategies in order)
                    └──────┬──────┘
                           │
         ┌─────────────────┼─────────────────┐
         │                 │                 │
         ▼                 ▼                 ▼
   ┌───────────┐    ┌───────────┐    ┌───────────┐
   │  Sequence │    │  Sequence │    │  Sequence │
   │  Combat   │    │   Patrol   │    │    Idle   │
   └─────┬─────┘    └─────┬─────┘    └─────┬─────┘
         │                 │                 │
         ▼                 ▼                 ▼
   ┌───────────┐    ┌───────────┐    ┌───────────┐
   │Condition: │    │Condition: │    │  Wait     │
   │EnemyNear  │    │!EnemyNear  │    │  5 secs   │
   └─────┬─────┘    └─────┬─────┘    └───────────┘
         │                 │
         ▼                 ▼
   ┌───────────┐    ┌───────────┐
   │  Action:  │    │  Action:   │
   │ MoveTo    │    │ MoveTo    │
   │ Enemy     │    │ PatrolPt   │
   └─────┬─────┘    └─────┬─────┘
         │                 │
         ▼                 ▼
   ┌───────────┐    ┌───────────┐
   │  Action:  │    │  Action:   │
   │  Attack   │    │   Wait     │
   └───────────┘    └───────────┘
```

### Neural Network Integration

```
Input Layer              Hidden Layers             Output Layer
─────────────           ─────────────            ────────────

Game State ─────┐
                │
Player Dist ────┼──► ┌────┐ ──┐
                │    │    │   │
Time of Day ────┼───▶│Layer├──▶┌────┐
                │    │  1 │   │    │
Nearby Agents ──┼───▶│    │ ──┴──▶│Layer│──▶ Action
                │    └────┘      │  N  │    Scores
Location Type ───┼──▶      ──▶   │    │
                │               └────┘
NPC Emotion ─────┘
Memory Context ──┘

         ┌────────────────────────────────────┐
         │          ONNX Runtime              │
         │  ┌──────────────────────────────┐  │
         │  │     Model Execution         │  │
         │  │  ┌────────────────────────┐  │  │
         │  │  │   CPU / CUDA / Metal   │  │  │
         │  │  └────────────────────────┘  │  │
         │  └──────────────────────────────┘  │
         └────────────────────────────────────┘
```

---

## Rendering Pipeline

### Classical Rendering Pipeline

```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│   Application │    │  Culling &   │    │    Vertex    │    │   Geometry   │
│      Stage     │───▶   Sorting     │───▶   Processing  │───▶    Shader    │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
                                                                    │
                                                                    ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│  Display /   │    │    Post-     │    │    Raster-   │    │   Fragment   │
│   Present    │◀───│  Processing  │◀───│   ization    │◀───│   Shader     │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
                           │
                           │
              ┌────────────┴────────────┐
              │                         │
              ▼                         ▼
     ┌───────────────┐          ┌───────────────┐
     │     Neural     │          │   Classical   │
     │    Ambient     │          │      AO       │
     │  Occlusion     │          │               │
     └───────────────┘          └───────────────┘
```

### Neural Rendering Enhancement

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        Neural Rendering Pipeline                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  Classical Render ──▶ Neural AA ──▶ Neural AO ──▶ Neural Upscale        │
│                                                                          │
│  ┌─────────────────────────────────────────────────────────────────────┐ │
│  │                      Neural Anti-Aliasing (Neural AA)               │ │
│  │                                                                      │ │
│  │   Input Frame ──▶ Feature Extraction ──▶ Edge Detection            │ │
│  │                          │                                          │ │
│  │                          ▼                                          │ │
│  │                    Neural Network ──▶ Smoothed Edges                │ │
│  │                                                                      │ │
│  └─────────────────────────────────────────────────────────────────────┘ │
│                                                                          │
│  ┌─────────────────────────────────────────────────────────────────────┐ │
│  │                    Neural Ambient Occlusion (NDAO)                  │ │
│  │                                                                      │ │
│  │   Depth Buffer ──▶ Normal Buffer ──▶ Screen-Space Analysis        │ │
│  │                          │                                          │ │
│  │                          ▼                                          │ │
│  │   ┌─────────────────────────────────────────────────────────────┐   │ │
│  │   │                  Neural Occlusion Network                    │   │ │
│  │   │   ┌────────┐    ┌────────┐    ┌────────┐    ┌────────┐    │   │ │
│  │   │   │ Input  │───▶│ Hidden │───▶│ Hidden │───▶│ Output  │    │   │ │
│  │   │   │ Layer  │    │  Layer │    │  Layer │    │  Layer  │    │   │ │
│  │   │   └────────┘    └────────┘    └────────┘    └────────┘    │   │ │
│  │   └─────────────────────────────────────────────────────────────┘   │ │
│  │                          │                                          │ │
│  │                          ▼                                          │ │
│  │                    Ambient Occlusion Buffer                          │ │
│  │                                                                      │ │
│  └─────────────────────────────────────────────────────────────────────┘ │
│                                                                          │
│  ┌─────────────────────────────────────────────────────────────────────┐ │
│  │                       Neural Upscaling                              │ │
│  │                                                                      │ │
│  │   Low-Res Input ──▶ Feature Extraction ──▶ Upscaling Network      │ │
│  │                          │                    │                    │ │
│  │                          │                    ▼                    │ │
│  │                          │              High-Quality Output         │ │
│  │                          │                                          │ │
│  │                          └──────▶ Detail Enhancement                 │ │
│  │                                                                      │ │
│  └─────────────────────────────────────────────────────────────────────┘ │
│                                                                          │
└─────────────────────────────────────────────────────────────────────────┘
```

### Render Context Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      RenderContext                           │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  Camera                                                      │
│  ┌─────────────────────────────────────────────────────────┐│
│  │  Position        │  Projection Matrix                   ││
│  │  Rotation        │  View Matrix                          ││
│  │  FOV             │  ViewProjection Matrix                ││
│  │  Near/Far        │  InvViewProjection Matrix             ││
│  └─────────────────────────────────────────────────────────┘│
│                                                              │
│  Lights                                                      │
│  ┌─────────────────────────────────────────────────────────┐│
│  │  Light[0]  ──▶  Position, Intensity, Color              ││
│  │  Light[1]  ──▶  Position, Intensity, Color              ││
│  │  ...                                                    ││
│  │  Light[N]  ──▶  Position, Intensity, Color              ││
│  └─────────────────────────────────────────────────────────┘│
│                                                              │
│  Render State                                                │
│  ┌─────────────────────────────────────────────────────────┐│
│  │  Depth Test │  Alpha Blend │  Cull Mode │  Fill Mode   ││
│  └─────────────────────────────────────────────────────────┘│
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Material System

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│   Material  │─────▶│   Shader    │─────▶│   GPU       │
│             │      │   Program   │      │   Programs  │
└──────┬──────┘      └─────────────┘      └─────────────┘
       │
       │         ┌─────────────┐      ┌─────────────┐
       ├────────▶│  Textures   │─────▶│  Samplers    │
       │         └─────────────┘      └─────────────┘
       │
       │         ┌─────────────┐      ┌─────────────┐
       └────────▶│  Uniforms   │─────▶│   Values     │
                 └─────────────┘      └─────────────┘

Material Properties:
  ┌─────────────────────────────────────────────┐
  │  Albedo     ──▶ Base color                  │
  │  Metallic   ──▶ Metalness factor            │
  │  Roughness  ──▶ Surface roughness          │
  │  Normal     ──▶ Normal map                  │
  │  AO         ──▶ Ambient occlusion           │
  │  Emissive   ──▶ Self-illumination           │
  └─────────────────────────────────────────────┘
```

---

## Data Flow

### Entity-Component Update Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                         Main Loop                                │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  1. Input Processing                                             │
│     ┌─────────────┐                                              │
│     │   Input     │ ─── Event Queue ───▶ Game Objects            │
│     └─────────────┘                                              │
│                                                                   │
│  2. AI Processing                                                │
│     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐    │
│     │ Perception  │ ──▶ │  Decision   │ ──▶ │   Action     │    │
│     │   Phase     │     │   Phase     │     │   Phase     │    │
│     └─────────────┘     └─────────────┘     └─────────────┘    │
│                                                                   │
│  3. Physics Update                                              │
│     ┌─────────────┐                                              │
│     │  Transform │ ─── Updated positions to Scene               │
│     │   Update   │                                              │
│     └─────────────┘                                              │
│                                                                   │
│  4. Scene Update                                                │
│     ┌─────────────┐                                              │
│     │  Iterate   │ ───▶ Components ───▶ Systems                  │
│     │   Entities  │                                              │
│     └─────────────┘                                              │
│                                                                   │
│  5. Render                                                      │
│     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐    │
│     │   Cull &    │ ──▶ │  Classical  │ ──▶ │   Neural    │    │
│     │   Sort     │     │   Render     │     │   Render    │    │
│     └─────────────┘     └─────────────┘     └─────────────┘    │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

### Resource Loading Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                     Resource Loading Flow                          │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  Request Resource                                                │
│         │                                                         │
│         ▼                                                         │
│  ┌─────────────────┐                                             │
│  │   Check Cache   │ ─── Cache Hit ───▶ Return Resource           │
│  └────────┬────────┘                                             │
│           │ Cache Miss                                            │
│           ▼                                                      │
│  ┌─────────────────┐                                             │
│  │ Create Load     │                                             │
│  │    Request      │                                             │
│  └────────┬────────┘                                             │
│           │                                                      │
│           ▼                                                      │
│  ┌─────────────────┐                                             │
│  │  Async Loader   │ ─── Background Thread ───▶ Process File     │
│  │    Thread       │                                             │
│  └────────┬────────┘                                             │
│           │                                                      │
│           ▼                                                      │
│  ┌─────────────────┐                                             │
│  │  Parse &        │                                             │
│  │  Transform      │                                             │
│  └────────┬────────┘                                             │
│           │                                                      │
│           ▼                                                      │
│  ┌─────────────────┐                                             │
│  │   Add to        │ ───▶ Ready for Use                          │
│  │   Resource      │                                             │
│  │     Cache       │                                             │
│  └─────────────────┘                                             │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

---

## Threading Model

```
┌──────────────────────────────────────────────────────────────────┐
│                        Thread Architecture                        │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  Main Thread                                                     │
│  ┌──────────────────────────────────────────────────────────────┐│
│  │  Game Loop                                                    ││
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐          ││
│  │  │  Input  │─▶│   AI    │─▶│Physics │─▶│ Render  │          ││
│  │  └─────────┘  └─────────┘  └─────────┘  └─────────┘          ││
│  └──────────────────────────────────────────────────────────────┘│
│                              │                                     │
│          ┌───────────────────┼───────────────────┐                │
│          │                   │                   │                │
│          ▼                   ▼                   ▼                │
│  ┌───────────────┐   ┌───────────────┐   ┌───────────────┐        │
│  │   Resource    │   │      AI      │   │     Audio     │        │
│  │   Loading     │   │   Inference   │   │    Thread     │        │
│  │   Thread      │   │   Thread      │   │               │        │
│  └───────────────┘   └───────────────┘   └───────────────┘        │
│                                                                   │
│  Thread Synchronization                                          │
│  ┌──────────────────────────────────────────────────────────────┐│
│  │  Mutex    ──▶ Protected data access                         ││
│  │  Signal   ──▶ Thread event signaling                        ││
│  │  Lock-Free ──▶ Atomic operations for counters               ││
│  └──────────────────────────────────────────────────────────────┘│
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

---

## Extension Points

### Custom Component

```cpp
class MyCustomComponent : public SparkLabs::Component {
    DECLARE_RTTI

public:
    void OnAttached() override {
        // Initialization
    }

    void Update(SparkLabs::float32 deltaTime) override {
        // Per-frame logic
    }
};
```

### Custom AI Node

```cpp
class MyAINode : public SparkLabs::AINode {
public:
    NodeStatus Process(float32 deltaTime) override {
        // Custom AI logic
        return NodeStatus::Success;
    }
};
```

### Custom Behavior Tree Node

```cpp
class MyActionNode : public SparkLabs::BehaviorTreeNode {
public:
    NodeStatus Execute(Agent* agent, float32 deltaTime) override {
        // Custom action logic
        return NodeStatus::Running;
    }
};
```

---

For detailed AI system documentation, see [AI_SYSTEM.md](AI_SYSTEM.md).
