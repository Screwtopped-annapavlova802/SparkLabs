"""
SparkLabs Engine - Python Module
================================

This is the main Python module for SparkLabs Engine, providing seamless
integration between C++ core engine and Python AI/ML capabilities.

Architecture:
- C++ Layer: High-performance core (math, scene, rendering, resource management)
- Python Layer: AI/ML integration, rapid prototyping, external API connections
- PyBind11 Bridge: Seamless communication between layers
"""

from .sparklabs import *

__version__ = "1.0.0"
__author__ = "SparkLabs Team"

# Export commonly used classes for easier access
__all__ = [
    'Engine',
    'Scene',
    'GameObject',
    'Component',
    'Vector2',
    'Vector3',
    'Vector4',
    'Quaternion',
    'Matrix4x4',
    'WorkflowGraph',
    'WorkflowNode',
    'AIBrain',
    'NPCBrain',
]
