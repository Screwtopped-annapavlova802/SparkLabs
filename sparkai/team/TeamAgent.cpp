#include "TeamAgent.h"

namespace SparkLabs {
namespace Team {

TeamAgent::TeamAgent(AgentRole role, AgentTier tier, const String& name)
    : m_Role(role)
    , m_Tier(tier)
    , m_Name(name)
    , m_CurrentTask(nullptr) {
}

void TeamAgent::Initialize() {
}

void TeamAgent::Update(float32 deltaTime) {
    if (m_CurrentTask && m_CurrentTask->status == TaskStatus::InProgress) {
    }
}

void TeamAgent::Shutdown() {
    m_CurrentTask = nullptr;
    m_TaskHistory.Clear();
}

void TeamAgent::AssignTask(const Task& task) {
    m_CurrentTask = SmartPtr<Task>(new Task(task));
    m_CurrentTask->status = TaskStatus::InProgress;
}

void TeamAgent::CompleteCurrentTask() {
    if (m_CurrentTask) {
        m_CurrentTask->status = TaskStatus::Completed;
        m_CurrentTask->progress = 1.0f;
        m_TaskHistory.PushBack(*m_CurrentTask);
        m_CurrentTask = nullptr;
    }
}

Task* TeamAgent::GetCurrentTask() {
    return m_CurrentTask.Get();
}

} // namespace Team
} // namespace SparkLabs
