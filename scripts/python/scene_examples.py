"""
SparkLabs Engine - Scene Creation Examples
==========================================
This example demonstrates how to create game scenes and objects
using the SparkLabs Python API.
"""

import sparklabs


def create_basic_scene():
    """Create a basic game scene with entities"""
    print("Creating scene...")
    scene = sparklabs.Scene()
    scene.set_name("MyGameScene")

    print("Creating player entity...")
    player = scene.create_entity("Player")
    player.set_position(sparklabs.Vector3(0.0, 1.0, 0.0))
    player.set_tag("Player")

    print("Creating NPC entities...")
    for i in range(3):
        npc = scene.create_entity(f"NPC_{i}")
        npc.set_position(sparklabs.Vector3(float(i) * 2.0, 1.0, 0.0))
        npc.set_tag("NPC")

    print("Creating environment objects...")
    ground = scene.create_entity("Ground")
    ground.set_position(sparklabs.Vector3(0.0, 0.0, 0.0))
    ground.set_tag("Environment")

    sky = scene.create_entity("Sky")
    sky.set_position(sparklabs.Vector3(0.0, 50.0, 0.0))
    sky.set_tag("Environment")

    print(f"Scene '{scene.get_name()}' created with {player.get_child_count() + 3 + 2} entities")
    return scene


def scene_hierarchy():
    """Demonstrate scene hierarchy and transforms"""
    print("\n" + "="*50)
    print("Creating scene with hierarchy...")
    print("="*50)

    scene = sparklabs.Scene()
    scene.set_name("HierarchyDemo")

    root = scene.create_entity("Root")
    root.set_position(sparklabs.Vector3(0.0, 0.0, 0.0))

    child1 = sparklabs.GameObject()
    child1.set_name("Child1")
    child1.set_parent(root)
    child1.set_position(sparklabs.Vector3(10.0, 0.0, 0.0))

    child2 = sparklabs.GameObject()
    child2.set_name("Child2")
    child2.set_parent(root)
    child2.set_position(sparklabs.Vector3(20.0, 0.0, 0.0))

    grandchild = sparklabs.GameObject()
    grandchild.set_name("GrandChild")
    grandchild.set_parent(child1)
    grandchild.set_position(sparklabs.Vector3(5.0, 0.0, 0.0))

    scene.add_entity(root)

    print(f"Root entity: {root.get_name()}")
    print(f"  Child count: {root.get_child_count()}")
    print(f"  World position: {root.get_position()}")
    print(f"  Forward vector: {root.get_forward()}")

    return scene


def transform_operations():
    """Demonstrate transform operations"""
    print("\n" + "="*50)
    print("Transform operations demo...")
    print("="*50)

    entity = sparklabs.GameObject()
    entity.set_name("TransformDemo")
    entity.set_position(sparklabs.Vector3(10.0, 5.0, 0.0))

    print(f"Initial position: {entity.get_position()}")

    entity.set_position(sparklabs.Vector3(20.0, 10.0, 0.0))
    print(f"New position: {entity.get_position()}")

    forward = entity.get_forward()
    right = entity.get_right()
    up = entity.get_up()

    print(f"Forward: ({forward.x}, {forward.y}, {forward.z})")
    print(f"Right: ({right.x}, {right.y}, {right.z})")
    print(f"Up: ({up.x}, {up.y}, {up.z})")

    entity.look_at(sparklabs.Vector3(0.0, 0.0, 0.0))
    print(f"After look_at: Forward = {entity.get_forward()}")

    return entity


def entity_finding():
    """Demonstrate finding entities in scene"""
    print("\n" + "="*50)
    print("Entity finding demo...")
    print("="*50)

    scene = sparklabs.Scene()
    scene.set_name("FindingDemo")

    player = scene.create_entity("Player")
    player.set_tag("Player")

    enemy1 = scene.create_entity("Enemy1")
    enemy1.set_tag("Enemy")

    enemy2 = scene.create_entity("Enemy2")
    enemy2.set_tag("Enemy")

    treasure = scene.create_entity("GoldTreasure")
    treasure.set_tag("Collectible")

    print("Finding by name:")
    found = scene.find_entity_by_name("Enemy1")
    if found:
        print(f"  Found: {found.get_name()}")

    print("Finding by tag:")
    enemies = []
    for i in range(10):
        entity = scene.create_entity(f"Enemy_{i}")
        entity.set_tag("Enemy")
        if i < 3:
            enemies.append(entity)

    print(f"  Created {len(enemies)} enemies with 'Enemy' tag")

    return scene


def main():
    """Run all scene examples"""
    print("="*60)
    print("SparkLabs Engine - Scene Creation Python Examples")
    print("="*60)

    try:
        create_basic_scene()
        scene_hierarchy()
        transform_operations()
        entity_finding()

        print("\n" + "="*60)
        print("All scene examples completed!")
        print("="*60)

    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()
