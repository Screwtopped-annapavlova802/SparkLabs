#include "WorkflowFactory.h"

namespace SparkLabs {

WorkflowNodeRegistry& WorkflowNodeRegistry::GetInstance() {
    static WorkflowNodeRegistry instance;
    return instance;
}

void WorkflowNodeRegistry::RegisterNode(const String& category, const String& name, NodeCreator creator) {
    m_Registry[category][name] = creator;
}

WorkflowNode* WorkflowNodeRegistry::CreateNode(const String& category, const String& name) {
    auto catIt = m_Registry.Find(category);
    if (catIt != m_Registry.End()) {
        auto nameIt = catIt->second.Find(name);
        if (nameIt != catIt->second.End()) {
            return nameIt->second();
        }
    }
    return nullptr;
}

Vector<String> WorkflowNodeRegistry::GetCategories() const {
    Vector<String> result;
    for (auto& pair : m_Registry) {
        result.PushBack(pair.first);
    }
    return result;
}

Vector<String> WorkflowNodeRegistry::GetNodeNames(const String& category) const {
    Vector<String> result;
    auto it = m_Registry.Find(category);
    if (it != m_Registry.End()) {
        for (auto& pair : it->second) {
            result.PushBack(pair.first);
        }
    }
    return result;
}

Vector< Pair<String, String> > WorkflowNodeRegistry::GetAllNodes() const {
    Vector< Pair<String, String> > result;
    for (auto& catPair : m_Registry) {
        for (auto& namePair : catPair.second) {
            result.PushBack(MakePair(catPair.first, namePair.first));
        }
    }
    return result;
}

WorkflowExecutor::WorkflowExecutor()
    : m_Graph(nullptr)
    , m_Executing(false)
    , m_Progress(0.0f)
{
}

WorkflowExecutor::~WorkflowExecutor() {
}

void WorkflowExecutor::SetGraph(WorkflowGraph* graph) {
    m_Graph = graph;
}

WorkflowGraph* WorkflowExecutor::GetGraph() const {
    return m_Graph;
}

bool WorkflowExecutor::Execute() {
    if (!m_Graph || m_Executing) return false;

    m_Executing = true;
    m_Progress = 0.0f;

    auto nodes = m_Graph->GetAllNodes();
    int32 totalNodes = nodes.Size();
    int32 executedCount = 0;

    for (auto& node : nodes) {
        if (!m_Executing) break;

        if (ExecuteNode(node.Get())) {
            executedCount++;
            m_Progress = (float32)executedCount / (float32)totalNodes;

            if (m_Callback) {
                m_Callback(node.Get());
            }
        }
    }

    m_Executing = false;
    m_Progress = 1.0f;
    return true;
}

void WorkflowExecutor::Abort() {
    m_Executing = false;
}

bool WorkflowExecutor::IsExecuting() const {
    return m_Executing;
}

float32 WorkflowExecutor::GetProgress() const {
    return m_Progress;
}

void WorkflowExecutor::SetExecutionCallback(std::function<void(WorkflowNode*)> callback) {
    m_Callback = callback;
}

bool WorkflowExecutor::ExecuteNode(WorkflowNode* node) {
    if (!node || node->IsBypassed() || node->IsMuted()) {
        return true;
    }

    Map<String, Variant> inputs = CollectInputs(node);
    Map<String, Variant> outputs;

    bool result = node->Execute(inputs, outputs);

    if (result) {
        node->OnExecuted();
        DistributeOutputs(node, outputs);
    }

    return result;
}

Map<String, Variant> WorkflowExecutor::CollectInputs(WorkflowNode* node) {
    Map<String, Variant> inputs;
    auto inputPins = node->GetInputPins();

    for (auto& pin : inputPins) {
        if (pin->IsConnected()) {
            WorkflowPin* connectedPin = pin->GetConnectedPin();
            if (connectedPin) {
                inputs[pin->GetName()] = connectedPin->GetValue();
            }
        } else {
            inputs[pin->GetName()] = pin->GetDefaultValue();
        }
    }

    return inputs;
}

void WorkflowExecutor::DistributeOutputs(WorkflowNode* node, const Map<String, Variant>& outputs) {
    auto outputPins = node->GetOutputPins();

    for (auto& pin : outputPins) {
        auto it = outputs.Find(pin->GetName());
        if (it != outputs.End()) {
            pin->SetValue(it->second);

            if (pin->IsConnected()) {
                WorkflowPin* connectedPin = pin->GetConnectedPin();
                if (connectedPin) {
                    connectedPin->SetValue(it->second);
                }
            }
        }
    }
}

void WorkflowHistory::SaveState(const WorkflowGraph* graph) {
    m_UndoStack.PushBack(WorkflowSerializer::Serialize(graph));
    m_RedoStack.Clear();
}

bool WorkflowHistory::Undo() {
    if (!CanUndo()) return false;

    m_RedoStack.PushBack(m_UndoStack.Back());
    m_UndoStack.PopBack();
    return true;
}

bool WorkflowHistory::Redo() {
    if (!CanRedo()) return false;

    m_UndoStack.PushBack(m_RedoStack.Back());
    m_RedoStack.PopBack();
    return true;
}

bool WorkflowHistory::CanUndo() const {
    return !m_UndoStack.IsEmpty();
}

bool WorkflowHistory::CanRedo() const {
    return !m_RedoStack.IsEmpty();
}

}
