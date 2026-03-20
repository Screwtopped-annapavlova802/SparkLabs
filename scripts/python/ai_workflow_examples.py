"""
SparkLabs Engine - AI Workflow Quick Start
============================================
This example demonstrates how to create an AI image generation workflow
using the SparkLabs Python API.
"""

import sparklabs

def basic_image_generation():
    """Basic image generation workflow example"""
    print("Creating workflow graph...")
    graph = sparklabs.WorkflowGraph()
    graph.set_name("Image Generation")

    print("Creating nodes...")
    text_prompt = sparklabs.create_text_prompt_node()
    text_prompt.set_id("prompt_1")
    text_prompt.set_name("Text Prompt")
    text_prompt.set_category("Prompt")
    text_prompt.set_position(100.0, 100.0)
    text_prompt.set_prompt("A beautiful sunset over mountains, digital art, 4K")

    image_gen = sparklabs.create_image_generation_node()
    image_gen.set_id("image_gen_1")
    image_gen.set_name("Image Generation")
    image_gen.set_category("AI/Image")
    image_gen.set_position(400.0, 100.0)
    image_gen.set_model("models/sd_xl.safetensors")
    image_gen.set_width(1024)
    image_gen.set_height(1024)
    image_gen.set_steps(30)
    image_gen.set_guidance_scale(7.5)
    image_gen.set_seed(42)

    save_image = sparklabs.create_save_image_node()
    save_image.set_id("save_1")
    save_image.set_name("Save Image")
    save_image.set_category("Output")
    save_image.set_position(700.0, 100.0)
    save_image.set_output_path("output/sunset.png")

    print("Adding nodes to graph...")
    graph.add_node(text_prompt)
    graph.add_node(image_gen)
    graph.add_node(save_image)

    print("Connecting nodes...")
    graph.connect("prompt_1", 0, "image_gen_1", 0)
    graph.connect("image_gen_1", 0, "save_1", 0)

    print("Executing workflow...")
    result = graph.execute()

    if result:
        print("Workflow completed successfully!")
    else:
        print("Workflow execution failed!")

    return result


def advanced_workflow():
    """Advanced workflow with multiple nodes"""
    print("\n" + "="*50)
    print("Creating advanced workflow...")
    print("="*50)

    graph = sparklabs.WorkflowGraph()
    graph.set_name("Advanced Image Workflow")

    prompt = sparklabs.create_text_prompt_node()
    prompt.set_id("main_prompt")
    prompt.set_prompt("A futuristic cityscape at night, cyberpunk style")
    prompt.set_position(50.0, 50.0)
    graph.add_node(prompt)

    neg_prompt = sparklabs.create_text_prompt_node()
    neg_prompt.set_id("neg_prompt")
    neg_prompt.set_prompt("blurry, low quality, distorted")
    neg_prompt.set_position(50.0, 200.0)
    graph.add_node(neg_prompt)

    latent = sparklabs.create_latent_node()
    latent.set_id("latent")
    latent.set_width(1024)
    latent.set_height(1024)
    latent.set_batch_size(1)
    latent.set_position(350.0, 50.0)
    graph.add_node(latent)

    sampler = sparklabs.create_ksampler_node()
    sampler.set_id("sampler")
    sampler.set_steps(25)
    sampler.set_cfg(8.0)
    sampler.set_seed(12345)
    sampler.set_sampler_name("euler_a")
    sampler.set_position(650.0, 50.0)
    graph.add_node(sampler)

    vae_decode = sparklabs.create_vae_decode_node()
    vae_decode.set_id("vae_decode")
    vae_decode.set_position(950.0, 50.0)
    graph.add_node(vae_decode)

    save = sparklabs.create_save_image_node()
    save.set_id("save")
    save.set_output_path("output/cyberpunk_city.png")
    save.set_position(1250.0, 50.0)
    graph.add_node(save)

    print("Connecting workflow nodes...")
    graph.connect("main_prompt", 0, "sampler", 0)
    graph.connect("latent", 0, "sampler", 1)
    graph.connect("sampler", 0, "vae_decode", 0)
    graph.connect("vae_decode", 0, "save", 0)

    print("Executing advanced workflow...")
    result = graph.execute()

    print(f"Result: {'Success' if result else 'Failed'}")
    return result


def text_to_video_workflow():
    """Text to video generation workflow"""
    print("\n" + "="*50)
    print("Creating text-to-video workflow...")
    print("="*50)

    graph = sparklabs.WorkflowGraph()
    graph.set_name("Text to Video")

    text_prompt = sparklabs.create_text_prompt_node()
    text_prompt.set_id("video_prompt")
    text_prompt.set_prompt("A flowing river through a forest, peaceful nature scene")
    text_prompt.set_position(100.0, 100.0)
    graph.add_node(text_prompt)

    video_gen = sparklabs.VideoGenerationNode()
    video_gen.set_id("video_gen")
    video_gen.set_model("models/video_model.safetensors")
    video_gen.set_duration(120)
    video_gen.set_fps(30)
    video_gen.set_position(400.0, 100.0)
    graph.add_node(video_gen)

    print("Executing video generation workflow...")
    result = graph.execute()
    print(f"Result: {'Success' if result else 'Failed'}")
    return result


def text_to_speech_workflow():
    """Text to speech audio generation workflow"""
    print("\n" + "="*50)
    print("Creating text-to-speech workflow...")
    print("="*50)

    graph = sparklabs.WorkflowGraph()
    graph.set_name("Text to Speech")

    text_prompt = sparklabs.create_text_prompt_node()
    text_prompt.set_id("tts_prompt")
    text_prompt.set_prompt("Welcome to the future of game development with SparkLabs!")
    text_prompt.set_position(100.0, 100.0)
    graph.add_node(text_prompt)

    audio_gen = sparklabs.AudioGenerationNode()
    audio_gen.set_id("audio_gen")
    audio_gen.set_model("models/tts_model.safetensors")
    audio_gen.set_duration(10.0)
    audio_gen.set_sample_rate(44100)
    audio_gen.set_position(400.0, 100.0)
    graph.add_node(audio_gen)

    print("Executing audio generation workflow...")
    result = graph.execute()
    print(f"Result: {'Success' if result else 'Failed'}")
    return result


def image_upscaling_workflow():
    """Image upscaling workflow"""
    print("\n" + "="*50)
    print("Creating image upscaling workflow...")
    print("="*50)

    graph = sparklabs.WorkflowGraph()
    graph.set_name("Image Upscaling")

    load_img = sparklabs.create_load_image_node()
    load_img.set_id("load")
    load_img.set_file_path("input/low_res_image.png")
    load_img.set_position(100.0, 100.0)
    graph.add_node(load_img)

    upscale = sparklabs.UpscaleNode()
    upscale.set_id("upscale")
    upscale.set_method("nearest")
    upscale.set_scale(4.0)
    upscale.set_position(400.0, 100.0)
    graph.add_node(upscale)

    save = sparklabs.create_save_image_node()
    save.set_id("save")
    save.set_output_path("output/high_res_image.png")
    save.set_position(700.0, 100.0)
    graph.add_node(save)

    print("Executing upscaling workflow...")
    result = graph.execute()
    print(f"Result: {'Success' if result else 'Failed'}")
    return result


def main():
    """Run all examples"""
    print("="*60)
    print("SparkLabs Engine - AI Workflow Python Examples")
    print("="*60)

    try:
        basic_image_generation()
        advanced_workflow()
        image_upscaling_workflow()

        print("\n" + "="*60)
        print("All examples completed!")
        print("="*60)

    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()
