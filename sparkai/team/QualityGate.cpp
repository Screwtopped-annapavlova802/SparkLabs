#include "QualityGate.h"

namespace SparkLabs {
namespace Team {

QualityGate* QualityGate::s_Instance = nullptr;

QualityGate::QualityGate()
    : m_Standard(QualityStandard::High)
    , m_Initialized(false) {
    m_CurrentMetrics.codeQuality = 0.0f;
    m_CurrentMetrics.performanceScore = 0.0f;
    m_CurrentMetrics.documentationCoverage = 0.0f;
    m_CurrentMetrics.testCoverage = 0.0f;
    m_CurrentMetrics.accessibilityScore = 0.0f;
}

QualityGate::~QualityGate() {
    if (m_Initialized) {
        Shutdown();
    }
}

QualityGate* QualityGate::GetInstance() {
    if (!s_Instance) {
        s_Instance = new QualityGate();
    }
    return s_Instance;
}

void QualityGate::Initialize() {
    if (m_Initialized) {
        return;
    }
    m_Initialized = true;
}

void QualityGate::Shutdown() {
    if (!m_Initialized) {
        return;
    }
    m_Initialized = false;
}

void QualityGate::SetQualityStandard(QualityStandard standard) {
    m_Standard = standard;
}

bool QualityGate::CheckCodeQuality(const String& code, const String& fileType) {
    float32 threshold = 0.7f;
    switch (m_Standard) {
        case QualityStandard::Low: threshold = 0.5f; break;
        case QualityStandard::Medium: threshold = 0.65f; break;
        case QualityStandard::High: threshold = 0.8f; break;
        case QualityStandard::Production: threshold = 0.9f; break;
    }
    return !code.IsEmpty();
}

bool QualityGate::CheckPerformance(const String& report) {
    return !report.IsEmpty();
}

bool QualityGate::CheckDocumentation(const String& docs) {
    return !docs.IsEmpty();
}

bool QualityGate::CheckAccessibility(const String& assessment) {
    return !assessment.IsEmpty();
}

void QualityGate::UpdateMetrics(const QualityMetrics& metrics) {
    m_CurrentMetrics = metrics;
}

void QualityGate::GenerateQualityReport() {
    m_LastReport = String("Quality Report:\n") +
        "Code Quality: " + String::FromFloat(m_CurrentMetrics.codeQuality) + "\n" +
        "Performance: " + String::FromFloat(m_CurrentMetrics.performanceScore) + "\n" +
        "Documentation: " + String::FromFloat(m_CurrentMetrics.documentationCoverage) + "\n" +
        "Test Coverage: " + String::FromFloat(m_CurrentMetrics.testCoverage) + "\n" +
        "Accessibility: " + String::FromFloat(m_CurrentMetrics.accessibilityScore);
}

} // namespace Team
} // namespace SparkLabs
