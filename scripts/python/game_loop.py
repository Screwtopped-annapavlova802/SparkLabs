"""
Game Loop Integration
======================

Demonstrates how to integrate Python code with the C++ engine's game loop.
This allows Python to handle AI decision-making, gameplay logic, and
external API calls while the C++ engine handles rendering and physics.
"""

import asyncio
from typing import Dict, List, Any, Optional, Callable
from . import sparklabs


class PythonGameSystem:
    """
    Base class for Python-based game systems.
    These systems run alongside the C++ engine and can respond to engine events.
    """

    def __init__(self, name: str):
        self.name = name
        self.enabled = True
        self._engine = None

    def initialize(self, engine):
        """Initialize the system with the engine instance."""
        self._engine = engine
        print(f"[PythonSystem] {self.name} initialized")

    def update(self, delta_time: float):
        """Called every frame by the engine."""
        pass

    def fixed_update(self, delta_time: float):
        """Called at fixed intervals for physics updates."""
        pass

    def late_update(self, delta_time: float):
        """Called after all other updates."""
        pass

    def render(self):
        """Called during rendering phase."""
        pass

    def shutdown(self):
        """Cleanup when the system is shut down."""
        print(f"[PythonSystem] {self.name} shutdown")


class AIDecisionSystem(PythonGameSystem):
    """
    AI decision-making system that runs in Python.
    Makes NPC decisions, generates dialogue, and handles AI behaviors.
    """

    def __init__(self):
        super().__init__("AIDecisionSystem")
        self.npcs: List[Dict[str, Any]] = []
        self.decision_cooldown = 0.1
        self.last_decision_time = 0.0

    def register_npc(self, npc_data: Dict[str, Any]):
        """Register an NPC with the AI system."""
        self.npcs.append(npc_data)
        print(f"[AIDecisionSystem] Registered NPC: {npc_data.get('name', 'Unknown')}")

    def update(self, delta_time: float):
        """Update AI decisions each frame."""
        if not self.enabled:
            return

        self.last_decision_time += delta_time

        if self.last_decision_time >= self.decision_cooldown:
            self.last_decision_time = 0.0
            self._make_decisions()

    def _make_decisions(self):
        """Make decisions for all registered NPCs."""
        for npc in self.npcs:
            self._process_npc_decision(npc)

    def _process_npc_decision(self, npc: Dict[str, Any]):
        """Process decision for a single NPC."""
        npc_name = npc.get('name', 'Unknown')
        state = npc.get('state', 'idle')

        if state == 'idle':
            new_state = self._decide_idle_behavior(npc)
        elif state == 'patrol':
            new_state = self._decide_patrol_behavior(npc)
        elif state == 'combat':
            new_state = self._decide_combat_behavior(npc)
        else:
            new_state = state

        if new_state != state:
            npc['state'] = new_state
            print(f"[AIDecisionSystem] {npc_name} changed state: {state} -> {new_state}")

    def _decide_idle_behavior(self, npc: Dict[str, Any]) -> str:
        """Decide what to do when idle."""
        import random
        roll = random.random()
        if roll < 0.3:
            return 'patrol'
        elif roll < 0.5:
            return 'interact'
        return 'idle'

    def _decide_patrol_behavior(self, npc: Dict[str, Any]) -> str:
        """Decide what to do when patrolling."""
        import random
        if random.random() < 0.1:
            return 'idle'
        return 'patrol'

    def _decide_combat_behavior(self, npc: Dict[str, Any]) -> str:
        """Decide what to do in combat."""
        health = npc.get('health', 100)
        if health < 20:
            return 'flee'
        return 'combat'


class PythonGameLoop:
    """
    Manages the integration between Python systems and the C++ engine.
    Provides hooks for Python code to run during the engine's update cycle.
    """

    def __init__(self):
        self.systems: List[PythonGameSystem] = []
        self._engine = None
        self._running = False

    def initialize(self, engine):
        """Initialize the game loop with the engine."""
        self._engine = engine
        for system in self.systems:
            system.initialize(engine)
        self._running = True
        print("[PythonGameLoop] Initialized")

    def add_system(self, system: PythonGameSystem):
        """Add a Python game system to the loop."""
        self.systems.append(system)
        if self._engine:
            system.initialize(self._engine)

    def remove_system(self, system: PythonGameSystem):
        """Remove a Python game system from the loop."""
        if system in self.systems:
            self.systems.remove(system)
            system.shutdown()

    def update(self, delta_time: float):
        """Update all registered systems."""
        if not self._running:
            return

        for system in self.systems:
            if system.enabled:
                system.update(delta_time)

    def fixed_update(self, delta_time: float):
        """Fixed update for all registered systems."""
        if not self._running:
            return

        for system in self.systems:
            if system.enabled:
                system.fixed_update(delta_time)

    def late_update(self, delta_time: float):
        """Late update for all registered systems."""
        if not self._running:
            return

        for system in self.systems:
            if system.enabled:
                system.late_update(delta_time)

    def render(self):
        """Render hook for all registered systems."""
        if not self._running:
            return

        for system in self.systems:
            if system.enabled:
                system.render()

    def shutdown(self):
        """Shutdown the game loop and all systems."""
        self._running = False
        for system in self.systems:
            system.shutdown()
        print("[PythonGameLoop] Shutdown")


# Global game loop instance
_game_loop: Optional[PythonGameLoop] = None


def get_game_loop() -> PythonGameLoop:
    """Get or create the global Python game loop."""
    global _game_loop
    if _game_loop is None:
        _game_loop = PythonGameLoop()
    return _game_loop


def register_system(system: PythonGameSystem):
    """Register a Python game system with the global loop."""
    get_game_loop().add_system(system)


def update_systems(delta_time: float):
    """Update all registered systems (called from C++)."""
    loop = get_game_loop()
    loop.update(delta_time)


def shutdown_game_loop():
    """Shutdown the Python game loop."""
    global _game_loop
    if _game_loop:
        _game_loop.shutdown()
        _game_loop = None
