#include "PyBindings.h"
#include "../../core/math/Vector3.h"
#include "../../core/math/Quaternion.h"
#include "../../core/math/Matrix4x4.h"
#include "../../engine/scene/Scene.h"
#include "../../engine/scene/GameObject.h"
#include "../../engine/scene/Node.h"
#include "../../sparkai/workflow/WorkflowGraph.h"
#include "../../sparkai/workflow/nodes/AIGenerationNodes.h"

namespace SparkLabs {

void PythonBindings::RegisterAll(py::module& m) {
    RegisterCoreTypes(m);
    RegisterMathTypes(m);
    RegisterEngineTypes(m);
    RegisterWorkflowTypes(m);
}

void PythonBindings::RegisterCoreTypes(py::module& m) {
    py::class_<Object>(m, "Object")
        .def("get_id", &Object::GetId, py::return_value_policy::reference)
        .def("add_ref", &Object::AddRef)
        .def("release", &Object::Release)
        .def("is_valid", &Object::IsValid);

    m.def("print_message", [](const String& msg) {
        py::print(msg);
    });
}

void PythonBindings::RegisterMathTypes(py::module& m) {
    py::class_<Vector3>(m, "Vector3")
        .def(py::init<>())
        .def(py::init<float32, float32, float32>())
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z)
        .def("length", &Vector3::Length)
        .def("normalized", &Vector3::Normalized)
        .def("dot", &Vector3::Dot)
        .def("cross", &Vector3::Cross)
        .def_static("zero", &Vector3::Zero)
        .def_static("one", &Vector3::One)
        .def_static("up", &Vector3::Up)
        .def_static("forward", &Vector3::Forward)
        .def("__repr__", [](const Vector3& v) {
            return py::str("Vector3({}, {}, {})").format(v.x, v.y, v.z);
        })
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * float32())
        .def(float32() * py::self);

    py::class_<Quaternion>(m, "Quaternion")
        .def(py::init<>())
        .def(py::init<float32, float32, float32, float32>())
        .def_readwrite("x", &Quaternion::x)
        .def_readwrite("y", &Quaternion::y)
        .def_readwrite("z", &Quaternion::z)
        .def_readwrite("w", &Quaternion::w)
        .def_static("identity", &Quaternion::Identity)
        .def_static("from_euler", &Quaternion::FromEuler)
        .def("to_matrix", &Quaternion::ToMatrix)
        .def("normalized", &Quaternion::Normalized)
        .def("__repr__", [](const Quaternion& q) {
            return py::str("Quaternion({}, {}, {}, {})").format(q.x, q.y, q.z, q.w);
        });

    py::class_<Matrix4x4>(m, "Matrix4x4")
        .def(py::init<>())
        .def_static("identity", &Matrix4x4::Identity)
        .def_static("perspective", &Matrix4x4::Perspective)
        .def_static("look_at", &Matrix4x4::LookAt)
        .def("transform_point", &Matrix4x4::TransformPoint)
        .def("inverse", &Matrix4x4::Inverse)
        .def("transpose", &Matrix4x4::Transpose);
}

void PythonBindings::RegisterEngineTypes(py::module& m) {
    py::class_<Node, SmartPtr<Node>>(m, "Node")
        .def(py::init<>())
        .def("set_name", &Node::SetName)
        .def("get_name", &Node::GetName)
        .def("set_position", &Node::SetPosition)
        .def("get_position", &Node::GetPosition)
        .def("set_rotation", &Node::SetRotation)
        .def("get_rotation", &Node::GetRotation)
        .def("set_scale", &Node::SetScale)
        .def("get_scale", &Node::GetScale)
        .def("set_local_position", &Node::SetLocalPosition)
        .def("get_local_position", &Node::GetLocalPosition)
        .def("get_forward", &Node::GetForward)
        .def("get_right", &Node::GetRight)
        .def("get_up", &Node::GetUp)
        .def("look_at", static_cast<void (Node::*)(const Vector3&)>(&Node::LookAt));

    py::class_<GameObject, Node, SmartPtr<GameObject>>(m, "GameObject")
        .def(py::init<>())
        .def("set_tag", &GameObject::SetTag)
        .def("get_tag", &GameObject::GetTag)
        .def("set_layer", &GameObject::SetLayer)
        .def("get_layer", &GameObject::GetLayer)
        .def("set_active", &GameObject::SetActive)
        .def("is_active", &GameObject::IsActive)
        .def("find_child_by_name", &GameObject::FindChildByName, py::return_value_policy::reference)
        .def("find_by_tag", &GameObject::FindByTag, py::return_value_policy::reference)
        .def("get_child_count", &GameObject::GetChildCount);

    py::class_<Scene, SmartPtr<Scene>>(m, "Scene")
        .def(py::init<>())
        .def("set_name", &Scene::SetName)
        .def("get_name", &Scene::GetName)
        .def("get_root", &Scene::GetRoot, py::return_value_policy::reference)
        .def("create_entity", &Scene::CreateEntity, py::return_value_policy::reference)
        .def("find_entity_by_name", &Scene::FindEntityByName, py::return_value_policy::reference)
        .def("add_entity", &Scene::AddEntity)
        .def("remove_entity", &Scene::RemoveEntity)
        .def("update", &Scene::Update);
}

void PythonBindings::RegisterWorkflowTypes(py::module& m) {
    py::class_<WorkflowNode, SmartPtr<WorkflowNode>>(m, "WorkflowNode")
        .def(py::init<>())
        .def("set_id", &WorkflowNode::SetId)
        .def("get_id", &WorkflowNode::GetId)
        .def("set_name", &WorkflowNode::SetName)
        .def("get_name", &WorkflowNode::GetName)
        .def("set_category", &WorkflowNode::SetCategory)
        .def("get_category", &WorkflowNode::GetCategory)
        .def("set_position", &WorkflowNode::SetPosition)
        .def("get_pos_x", &WorkflowNode::GetPosX)
        .def("get_pos_y", &WorkflowNode::GetPosY)
        .def("set_size", &WorkflowNode::SetSize)
        .def("get_width", &WorkflowNode::GetWidth)
        .def("get_height", &WorkflowNode::GetHeight)
        .def("is_muted", &WorkflowNode::IsMuted)
        .def("set_muted", &WorkflowNode::SetMuted)
        .def("is_bypassed", &WorkflowNode::IsBypassed)
        .def("set_bypassed", &WorkflowNode::SetBypassed)
        .def_property_readonly("input_pins", &WorkflowNode::GetInputPins)
        .def_property_readonly("output_pins", &WorkflowNode::GetOutputPins);

    py::class_<WorkflowPin, SmartPtr<WorkflowPin>>(m, "WorkflowPin")
        .def(py::init<>())
        .def("set_name", &WorkflowPin::SetName)
        .def("get_name", &WorkflowPin::GetName)
        .def("is_input", &WorkflowPin::IsInput)
        .def("is_connected", &WorkflowPin::IsConnected)
        .def("set_value", [](WorkflowPin& self, const py::object& value) {
            if (py::isinstance<py::str>(value)) {
                self.SetValue(String(py::cast<py::str>(value)));
            } else if (py::isinstance<py::int_>(value)) {
                self.SetValue(int64_t(py::cast<py::int_>(value)));
            } else if (py::isinstance<py::float_>(value)) {
                self.SetValue(double(py::cast<py::float_>(value)));
            } else if (py::isinstance<py::bool_>(value)) {
                self.SetValue(bool(py::cast<py::bool_>(value)));
            }
        })
        .def("get_value", [](WorkflowPin& self) -> py::object {
            const Variant& v = self.GetValue();
            return py::none();
        });

    py::class_<WorkflowGraph, SmartPtr<WorkflowGraph>>(m, "WorkflowGraph")
        .def(py::init<>())
        .def("set_name", &WorkflowGraph::SetName)
        .def("get_name", &WorkflowGraph::GetName)
        .def("add_node", &WorkflowGraph::AddNode)
        .def("remove_node", &WorkflowGraph::RemoveNode)
        .def("get_node", &WorkflowGraph::GetNode, py::return_value_policy::reference)
        .def_property_readonly("all_nodes", &WorkflowGraph::GetAllNodes)
        .def("add_edge", &WorkflowGraph::AddEdge)
        .def("remove_edge", &WorkflowGraph::RemoveEdge)
        .def("can_connect", &WorkflowGraph::CanConnect)
        .def("connect", &WorkflowGraph::Connect)
        .def("execute", &WorkflowGraph::Execute)
        .def("abort", &WorkflowGraph::Abort)
        .def("clear", &WorkflowGraph::Clear);

    py::class_<ImageGenerationNode, WorkflowNode, SmartPtr<ImageGenerationNode>>(m, "ImageGenerationNode")
        .def(py::init<>())
        .def("set_model", &ImageGenerationNode::SetModel)
        .def("get_model", &ImageGenerationNode::GetModel)
        .def("set_width", &ImageGenerationNode::SetWidth)
        .def("get_width", &ImageGenerationNode::GetWidth)
        .def("set_height", &ImageGenerationNode::SetHeight)
        .def("get_height", &ImageGenerationNode::GetHeight)
        .def("set_steps", &ImageGenerationNode::SetSteps)
        .def("get_steps", &ImageGenerationNode::GetSteps)
        .def("set_guidance_scale", &ImageGenerationNode::SetGuidanceScale)
        .def("get_guidance_scale", &ImageGenerationNode::GetGuidanceScale)
        .def("set_seed", &ImageGenerationNode::SetSeed)
        .def("get_seed", &ImageGenerationNode::GetSeed);

    py::class_<TextGenerationNode, WorkflowNode, SmartPtr<TextGenerationNode>>(m, "TextGenerationNode")
        .def(py::init<>())
        .def("set_model", &TextGenerationNode::SetModel)
        .def("get_model", &TextGenerationNode::GetModel)
        .def("set_max_tokens", &TextGenerationNode::SetMaxTokens)
        .def("get_max_tokens", &TextGenerationNode::GetMaxTokens)
        .def("set_temperature", &TextGenerationNode::SetTemperature)
        .def("get_temperature", &TextGenerationNode::GetTemperature);

    py::class_<VideoGenerationNode, WorkflowNode, SmartPtr<VideoGenerationNode>>(m, "VideoGenerationNode")
        .def(py::init<>())
        .def("set_model", &VideoGenerationNode::SetModel)
        .def("get_model", &VideoGenerationNode::GetModel)
        .def("set_duration", &VideoGenerationNode::SetDuration)
        .def("get_duration", &VideoGenerationNode::GetDuration)
        .def("set_fps", &VideoGenerationNode::SetFps)
        .def("get_fps", &VideoGenerationNode::GetFps);

    py::class_<AudioGenerationNode, WorkflowNode, SmartPtr<AudioGenerationNode>>(m, "AudioGenerationNode")
        .def(py::init<>())
        .def("set_model", &AudioGenerationNode::SetModel)
        .def("get_model", &AudioGenerationNode::GetModel)
        .def("set_duration", &AudioGenerationNode::SetDuration)
        .def("get_duration", &AudioGenerationNode::GetDuration)
        .def("set_sample_rate", &AudioGenerationNode::SetSampleRate)
        .def("get_sample_rate", &AudioGenerationNode::GetSampleRate);

    py::class_<TextPromptNode, WorkflowNode, SmartPtr<TextPromptNode>>(m, "TextPromptNode")
        .def(py::init<>())
        .def("set_prompt", &TextPromptNode::SetPrompt)
        .def("get_prompt", &TextPromptNode::GetPrompt)
        .def("set_negative_prompt", &TextPromptNode::SetNegativePrompt)
        .def("get_negative_prompt", &TextPromptNode::GetNegativePrompt);

    py::class_<LoadImageNode, WorkflowNode, SmartPtr<LoadImageNode>>(m, "LoadImageNode")
        .def(py::init<>())
        .def("set_file_path", &LoadImageNode::SetFilePath)
        .def("get_file_path", &LoadImageNode::GetFilePath);

    py::class_<SaveImageNode, WorkflowNode, SmartPtr<SaveImageNode>>(m, "SaveImageNode")
        .def(py::init<>())
        .def("set_output_path", &SaveImageNode::SetOutputPath)
        .def("get_output_path", &SaveImageNode::GetOutputPath);

    py::class_<KSamplerNode, WorkflowNode, SmartPtr<KSamplerNode>>(m, "KSamplerNode")
        .def(py::init<>())
        .def("set_steps", &KSamplerNode::SetSteps)
        .def("get_steps", &KSamplerNode::GetSteps)
        .def("set_cfg", &KSamplerNode::SetCfg)
        .def("get_cfg", &KSamplerNode::GetCfg)
        .def("set_seed", &KSamplerNode::SetSeed)
        .def("get_seed", &KSamplerNode::GetSeed)
        .def("set_sampler_name", &KSamplerNode::SetSamplerName)
        .def("get_sampler_name", &KSamplerNode::GetSamplerName);

    py::class_<LatentNode, WorkflowNode, SmartPtr<LatentNode>>(m, "LatentNode")
        .def(py::init<>())
        .def("set_width", &LatentNode::SetWidth)
        .def("get_width", &LatentNode::GetWidth)
        .def("set_height", &LatentNode::SetHeight)
        .def("get_height", &LatentNode::GetHeight)
        .def("set_batch_size", &LatentNode::SetBatchSize)
        .def("get_batch_size", &LatentNode::GetBatchSize);

    py::class_<VAEDecodeNode, WorkflowNode, SmartPtr<VAEDecodeNode>>(m, "VAEDecodeNode")
        .def(py::init<>());

    py::class_<VAEEncodeNode, WorkflowNode, SmartPtr<VAEEncodeNode>>(m, "VAEEncodeNode")
        .def(py::init<>());

    py::class_<UpscaleNode, WorkflowNode, SmartPtr<UpscaleNode>>(m, "UpscaleNode")
        .def(py::init<>())
        .def("set_method", &UpscaleNode::SetMethod)
        .def("get_method", &UpscaleNode::GetMethod)
        .def("set_scale", &UpscaleNode::SetScale)
        .def("get_scale", &UpscaleNode::GetScale);

    py::class_<InpaintNode, WorkflowNode, SmartPtr<InpaintNode>>(m, "InpaintNode")
        .def(py::init<>())
        .def("set_mask", &InpaintNode::SetMask)
        .def("get_mask", &InpaintNode::GetMask);

    py::class_<ControlNetNode, WorkflowNode, SmartPtr<ControlNetNode>>(m, "ControlNetNode")
        .def(py::init<>())
        .def("set_control_net_path", &ControlNetNode::SetControlNetPath)
        .def("get_control_net_path", &ControlNetNode::GetControlNetPath)
        .def("set_strength", &ControlNetNode::SetStrength)
        .def("get_strength", &ControlNetNode::GetStrength);

    m.def("create_image_generation_node", []() {
        return SmartPtr<WorkflowNode>(new ImageGenerationNode());
    });

    m.def("create_text_generation_node", []() {
        return SmartPtr<WorkflowNode>(new TextGenerationNode());
    });

    m.def("create_video_generation_node", []() {
        return SmartPtr<WorkflowNode>(new VideoGenerationNode());
    });

    m.def("create_audio_generation_node", []() {
        return SmartPtr<WorkflowNode>(new AudioGenerationNode());
    });

    m.def("create_text_prompt_node", []() {
        return SmartPtr<WorkflowNode>(new TextPromptNode());
    });

    m.def("create_load_image_node", []() {
        return SmartPtr<WorkflowNode>(new LoadImageNode());
    });

    m.def("create_save_image_node", []() {
        return SmartPtr<WorkflowNode>(new SaveImageNode());
    });

    m.def("create_ksampler_node", []() {
        return SmartPtr<WorkflowNode>(new KSamplerNode());
    });

    m.def("create_latent_node", []() {
        return SmartPtr<WorkflowNode>(new LatentNode());
    });

    m.def("create_vae_decode_node", []() {
        return SmartPtr<WorkflowNode>(new VAEDecodeNode());
    });

    m.def("create_upscale_node", []() {
        return SmartPtr<WorkflowNode>(new UpscaleNode());
    });

    m.def("create_inpaint_node", []() {
        return SmartPtr<WorkflowNode>(new InpaintNode());
    });
}

void PythonBindings::RegisterNPCTypes(py::module& m) {
}

void PythonBindings::RegisterGameplayTypes(py::module& m) {
}

}
