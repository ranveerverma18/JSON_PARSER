#!/usr/bin/env python3
"""Generate deeply nested JSON for stress testing"""

import json

def generate_deep_nested(depth):
    """Generate JSON nested to exact depth"""
    result = "value"
    for i in range(depth):
        result = {"level": i, "nested": result}
    return result

# Generate different depths
depths = [10, 50, 100, 500]

for d in depths:
    print(f"Generating {d} levels deep...")
    data = generate_deep_nested(d)
    filename = f"bench_deep_{d}.json"
    
    with open(filename, 'w') as f:
        json.dump(data, f)
    
    import os
    size = os.path.getsize(filename)
    print(f"  Created {filename}: {size / 1024:.2f} KB")

print("\n✓ Deep nesting test files created!")
print("\nTest with:")
print("  benchmark.exe bench_deep_*.json")
