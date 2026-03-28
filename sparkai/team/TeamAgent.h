#ifndef SPARKLABS_TEAM_TEAMAGENT_H
#define SPARKLABS_TEAM_TEAMAGENT_H

#include "../core/Types.h"
#include "../core/string/String.h"
#include "../core/memory/SmartPtr.h"

namespace SparkLabs {
namespace Team {

enum class AgentRole {
    CreativeDirector,
    TechnicalDirector,
    Producer,
    GameDesigner,
    LeadProgrammer,
    ArtDirector,
    AudioDirector,
    NarrativeDirector,
    QALead,
    ReleaseManager,
    GameplayProgrammer,
    EngineProgrammer,
    AIProgrammer,
    NetworkProgrammer,
    ToolsProgrammer,
    UIProgrammer,
    SystemsDesigner,
    LevelDesigner,
    EconomyDesigner,
    TechnicalArtist,
    SoundDesigner,
    Writer,
    WorldBuilder,
    UXDesigner,
    Prototyper,
    PerformanceAnalyst,
    DevOpsEngineer,
    AnalyticsEngineer,
    SecurityEngineer,
    QATester,
    AccessibilitySpecialist,
    LiveOpsDesigner,
    CommunityManager
};

enum class AgentTier {
    Director,
    Lead,
    Specialist
};

enum class TaskStatus {
    Pending,
    InProgress,
    Review,
    Completed,
    Blocked
};

struct Task {
    String id;
    String title;
    String description;
    AgentRole assignedTo;
    TaskStatus status;
    float32 progress;
    String createdBy;
};

class TeamAgent {
public:
    TeamAgent(AgentRole role, AgentTier tier, const String& name);
    virtual ~TeamAgent() = default;

    virtual void Initialize();
    virtual void Update(float32 deltaTime);
    virtual void Shutdown();

    virtual String GenerateFeedback(const String& context) = 0;
    virtual bool ValidateTask(const Task& task) = 0;

    void AssignTask(const Task& task);
    void CompleteCurrentTask();
    Task* GetCurrentTask();

    AgentRole GetRole() const { return m_Role; }
    AgentTier GetTier() const { return m_Tier; }
    const String& GetName() const { return m_Name; }

protected:
    AgentRole m_Role;
    AgentTier m_Tier;
    String m_Name;
    SmartPtr<Task> m_CurrentTask;
    Vector<Task> m_TaskHistory;
};

} // namespace Team
} // namespace SparkLabs

#endif
