#ifndef SPARKLABS_TEAM_TEAMSPECIALIST_H
#define SPARKLABS_TEAM_TEAMSPECIALIST_H

#include "TeamAgent.h"

namespace SparkLabs {
namespace Team {

class TeamSpecialist : public TeamAgent {
public:
    TeamSpecialist(AgentRole role, const String& name);
    virtual ~TeamSpecialist() = default;

    virtual String GenerateFeedback(const String& context) override;
    virtual bool ValidateTask(const Task& task) override;

    void SubmitWork(const String& work);
    void RequestClarification(const String& question);

    void SetDomainExpertise(const String& expertise);
    const String& GetDomainExpertise() const { return m_DomainExpertise; }

protected:
    String m_DomainExpertise;
    Vector<String> m_Submissions;
    Vector<String> m_Questions;
};

} // namespace Team
} // namespace SparkLabs

#endif
