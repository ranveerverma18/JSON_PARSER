#!/usr/bin/env python3
"""
Generate test JSON files of various sizes and complexities for benchmarking
"""

import json
import random
import string
import sys

def random_string(length=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def generate_nested_object(depth, max_depth=10, breadth=5):
    """Generate deeply nested JSON object"""
    if depth >= max_depth:
        return random.choice([
            random_string(20),
            random.randint(0, 1000),
            random.random() * 1000,
            True,
            False,
            None
        ])
    
    obj_type = random.choice(['object', 'array', 'value'])
    
    if obj_type == 'object':
        return {
            random_string(8): generate_nested_object(depth + 1, max_depth, breadth)
            for _ in range(random.randint(1, breadth))
        }
    elif obj_type == 'array':
        return [
            generate_nested_object(depth + 1, max_depth, breadth)
            for _ in range(random.randint(1, breadth))
        ]
    else:
        return random.choice([
            random_string(20),
            random.randint(0, 1000),
            random.random() * 1000,
            True,
            False,
            None
        ])

def generate_wide_object(num_keys=1000):
    """Generate wide flat object with many keys"""
    return {
        f"key_{i}": {
            "id": i,
            "name": random_string(15),
            "value": random.random() * 1000,
            "active": random.choice([True, False]),
            "tags": [random_string(5) for _ in range(random.randint(1, 5))]
        }
        for i in range(num_keys)
    }

def generate_array_heavy(num_items=10000):
    """Generate JSON with large arrays"""
    return {
        "metadata": {
            "count": num_items,
            "generated": "benchmark"
        },
        "data": [
            {
                "id": i,
                "value": random.random() * 1000,
                "label": random_string(10)
            }
            for i in range(num_items)
        ]
    }

def generate_string_heavy(num_strings=1000, string_length=100):
    """Generate JSON with many long strings"""
    return {
        "strings": [
            random_string(string_length)
            for _ in range(num_strings)
        ],
        "metadata": {
            "count": num_strings,
            "length": string_length
        }
    }

def generate_realistic_config():
    """Generate realistic configuration file"""
    return {
        "version": "1.0.0",
        "application": {
            "name": "MyApp",
            "port": 8080,
            "host": "localhost",
            "debug": False
        },
        "database": {
            "host": "db.example.com",
            "port": 5432,
            "name": "mydb",
            "credentials": {
                "username": "admin",
                "password": "secret123"
            },
            "pool": {
                "min": 5,
                "max": 20,
                "timeout": 30000
            }
        },
        "logging": {
            "level": "info",
            "format": "json",
            "outputs": ["console", "file"],
            "file": {
                "path": "/var/log/app.log",
                "maxSize": "100MB",
                "maxFiles": 10
            }
        },
        "features": {
            "authentication": True,
            "caching": True,
            "compression": True,
            "rateLimit": {
                "enabled": True,
                "maxRequests": 100,
                "windowMs": 60000
            }
        }
    }

def save_json(data, filename, minify=False):
    """Save JSON to file"""
    with open(filename, 'w') as f:
        if minify:
            json.dump(data, f, separators=(',', ':'))
        else:
            json.dump(data, f, indent=2)
    
    import os
    size = os.path.getsize(filename)
    print(f"Generated {filename}: {size / 1024:.2f} KB")

if __name__ == "__main__":
    print("Generating benchmark test files...\n")
    
    # Small config file (typical use case)
    save_json(generate_realistic_config(), "bench_small_10kb.json")
    
    # Medium nested structure
    save_json(generate_nested_object(0, max_depth=8, breadth=4), "bench_medium_nested.json")
    
    # Wide object (many keys)
    save_json(generate_wide_object(500), "bench_wide_500keys.json")
    
    # Large array
    save_json(generate_array_heavy(5000), "bench_large_array_5k.json")
    
    # String heavy
    save_json(generate_string_heavy(1000, 200), "bench_string_heavy.json")
    
    # Very deep nesting (stress test)
    print("Generating deep nesting test (this may take a moment)...")
    save_json(generate_nested_object(0, max_depth=100, breadth=2), "bench_deep_nesting_100.json")
    
    # Large file (minified)
    save_json(generate_array_heavy(50000), "bench_huge_50k_items.json", minify=True)
    
    print("\n✓ All benchmark files generated!")
    print("\nTo run benchmarks:")
    print("  g++ -std=c++17 -O3 -Iinclude src/benchmark.cpp src/*.cpp -o benchmark.exe")
    print("  benchmark.exe bench_*.json")
