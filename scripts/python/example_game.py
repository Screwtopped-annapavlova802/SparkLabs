"""
Example Game - SparkLabs Python Integration
==========================================

This example demonstrates how to use the SparkLabs Engine from Python,
showcasing the C++/Python hybrid architecture.
"""

import sys
import os

# Add the build directory to Python path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', 'build', 'python'))

try:
    import sparklabs
except ImportError:
    print("Error: Could not import sparklabs module.")
    print("Please build the Python bindings first using CMake.")
    sys.exit(1)

from game_loop import (
    PythonGameSystem,
    PythonGameLoop,
    AIDecisionSystem,
    register_system,
    get_game_loop
)
from ai_integration import (
    AIServiceManager,
    OpenAIService,
    setup_ai_services,
    get_ai_manager
)


class ExampleGameSystem(PythonGameSystem):
    """Example game system that runs in Python."""

    def __init__(self):
        super().__init__("ExampleGameSystem")
        self.frame_count = 0
        self.last_log_time = 0.0

    def initialize(self, engine):
        super().initialize(engine)
        print("[ExampleGameSystem] Ready to run!")

    def update(self, delta_time: float):
        self.frame_count += 1
        self.last_log_time += delta_time

        if self.last_log_time >= 1.0:
            self.last_log_time = 0.0
            print(f"[ExampleGameSystem] Frame: {self.frame_count}, Delta: {delta_time:.4f}s")


async def main():
    """Main entry point for the example game."""
    print("=" * 50)
    print("  SparkLabs Engine - Python Example")
    print("=" * 50)
    print()

    # Initialize the C++ engine
    print("Initializing C++ engine...")
    engine = sparklabs.Engine.GetInstance()
    engine.Initialize()
    print("C++ engine initialized!")
    print()

    # Create a scene
    print("Creating scene...")
    scene = sparklabs.Scene()
    scene.SetName("PythonExampleScene")
    engine.SetScene(scene)
    print(f"Scene created: {scene.GetName()}")
    print()

    # Create some game objects
    print("Creating game objects...")
    player = scene.CreateEntity("Player")
    player.SetPosition(sparklabs.Vector3(0.0, 1.0, 0.0))
    player.SetTag("Player")
    print(f"Created Player at {player.GetPosition()}")

    npc1 = scene.CreateEntity("NPC_1")
    npc1.SetPosition(sparklabs.Vector3(5.0, 1.0, 0.0))
    npc1.SetTag("NPC")
    print(f"Created NPC_1 at {npc1.GetPosition()}")

    npc2 = scene.CreateEntity("NPC_2")
    npc2.SetPosition(sparklabs.Vector3(-5.0, 1.0, 0.0))
    npc2.SetTag("NPC")
    print(f"Created NPC_2 at {npc2.GetPosition()}")
    print()

    # Set up Python game systems
    print("Setting up Python game systems...")
    game_loop = get_game_loop()
    game_loop.initialize(engine)

    # Add example game system
    example_system = ExampleGameSystem()
    register_system(example_system)

    # Add AI decision system
    ai_system = AIDecisionSystem()
    register_system(ai_system)

    # Register NPCs with AI system
    ai_system.register_npc({
        'name': 'NPC_1',
        'state': 'idle',
        'health': 100,
        'entity': npc1
    })

    ai_system.register_npc({
        'name': 'NPC_2',
        'state': 'patrol',
        'health': 80,
        'entity': npc2
    })

    print("Python game systems ready!")
    print()

    # Set up AI services (optional, requires API keys)
    print("AI Services (optional):")
    print("  - OpenAI: Text generation, dialogue")
    print("  - HuggingFace: Local model inference")
    print("  - LocalModel: ONNX model inference")
    print()
    print("Note: Configure AI services in config.json to enable.")
    print()

    # Run the engine (commented out to prevent infinite loop)
    # In a real game, you would call engine.Run() here
    print("=" * 50)
    print("  Example setup complete!")
    print("=" * 50)
    print()
    print("To run the full engine, uncomment engine.Run() in this script.")
    print()

    # Update Python systems a few times for demonstration
    print("Demonstrating Python system updates...")
    for i in range(5):
        delta_time = 0.016  # ~60 FPS
        game_loop.update(delta_time)

    print()

    # Cleanup
    print("Shutting down...")
    game_loop.shutdown()
    engine.Shutdown()
    print("Done!")


if __name__ == "__main__":
    import asyncio
    asyncio.run(main())
