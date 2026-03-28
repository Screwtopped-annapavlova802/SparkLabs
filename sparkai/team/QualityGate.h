#ifndef SPARKLABS_TEAM_QUALITYGATE_H
#define SPARKLABS_TEAM_QUALITYGATE_H

#include "../core/Types.h"
#include "../core/string/String.h"

namespace SparkLabs {
namespace Team {

enum class QualityStandard {
    Low,
    Medium,
    High,
    Production
};

struct QualityMetrics {
    float32 codeQuality;
    float32 performanceScore;
    float32 documentationCoverage;
    float32 testCoverage;
    float32 accessibilityScore;
};

class QualityGate {
public:
    static QualityGate* GetInstance();

    void Initialize();
    void Shutdown();

    void SetQualityStandard(QualityStandard standard);
    QualityStandard GetQualityStandard() const { return m_Standard; }

    bool CheckCodeQuality(const String& code, const String& fileType);
    bool CheckPerformance(const String& report);
    bool CheckDocumentation(const String& docs);
    bool CheckAccessibility(const String& assessment);

    QualityMetrics GetCurrentMetrics() const { return m_CurrentMetrics; }
    void UpdateMetrics(const QualityMetrics& metrics);

    void GenerateQualityReport();
    String GetLastReport() const { return m_LastReport; }

private:
    QualityGate();
    ~QualityGate();

    static QualityGate* s_Instance;

    QualityStandard m_Standard;
    QualityMetrics m_CurrentMetrics;
    String m_LastReport;
    bool m_Initialized;
};

} // namespace Team
} // namespace SparkLabs

#endif
