#include "TeamSpecialist.h"

namespace SparkLabs {
namespace Team {

TeamSpecialist::TeamSpecialist(AgentRole role, const String& name)
    : TeamAgent(role, AgentTier::Specialist, name) {
}

String TeamSpecialist::GenerateFeedback(const String& context) {
    return String("Specialist feedback on: ") + context;
}

bool TeamSpecialist::ValidateTask(const Task& task) {
    return !task.title.IsEmpty() && task.assignedTo == m_Role;
}

void TeamSpecialist::SubmitWork(const String& work) {
    m_Submissions.PushBack(work);
}

void TeamSpecialist::RequestClarification(const String& question) {
    m_Questions.PushBack(question);
}

void TeamSpecialist::SetDomainExpertise(const String& expertise) {
    m_DomainExpertise = expertise;
}

} // namespace Team
} // namespace SparkLabs
