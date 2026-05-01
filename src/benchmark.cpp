#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>

#include "../include/FileUtils.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/JSONSerializer.h"

using namespace std::chrono;

struct BenchmarkResult {
    std::string filename;
    size_t fileSize;
    double parseTimeMs;
    double serializeTimeMs;
    double totalTimeMs;
    double throughputMBps;
    size_t memoryUsed;
    int nestingDepth;
};

// Calculate nesting depth
int calculateDepth(const JSONValue& value, int currentDepth = 0) {
    int maxDepth = currentDepth;
    
    if (value.isArray()) {
        for (const auto& elem : value.asArray()) {
            int depth = calculateDepth(*elem, currentDepth + 1);
            maxDepth = std::max(maxDepth, depth);
        }
    } else if (value.isObject()) {
        for (const auto& [key, val] : value.asObject()) {
            int depth = calculateDepth(*val, currentDepth + 1);
            maxDepth = std::max(maxDepth, depth);
        }
    }
    
    return maxDepth;
}

// Estimate memory usage (rough approximation)
size_t estimateMemory(const JSONValue& value) {
    size_t total = sizeof(JSONValue);
    
    if (value.isString()) {
        total += value.asString().capacity();
    } else if (value.isArray()) {
        const auto& arr = value.asArray();
        total += arr.capacity() * sizeof(std::shared_ptr<JSONValue>);
        for (const auto& elem : arr) {
            total += estimateMemory(*elem);
        }
    } else if (value.isObject()) {
        const auto& obj = value.asObject();
        total += obj.size() * (sizeof(std::string) + sizeof(std::shared_ptr<JSONValue>) + 32); // hash overhead
        for (const auto& [key, val] : obj) {
            total += key.capacity();
            total += estimateMemory(*val);
        }
    }
    
    return total;
}

BenchmarkResult benchmarkFile(const std::string& filepath) {
    BenchmarkResult result;
    result.filename = filepath;
    
    // Read file
    std::string content = FileUtils::readFile(filepath);
    result.fileSize = content.size();
    
    // Benchmark parsing
    auto parseStart = high_resolution_clock::now();
    
    Tokenizer tokenizer(content);
    std::vector<Token> tokens = tokenizer.tokenize();
    
    Parser parser(tokens);
    JSONValue root = parser.parse();
    
    auto parseEnd = high_resolution_clock::now();
    result.parseTimeMs = duration_cast<microseconds>(parseEnd - parseStart).count() / 1000.0;
    
    // Calculate depth and memory
    result.nestingDepth = calculateDepth(root);
    result.memoryUsed = estimateMemory(root);
    
    // Benchmark serialization
    auto serializeStart = high_resolution_clock::now();
    std::string serialized = JSONSerializer::serializeCompact(root);
    auto serializeEnd = high_resolution_clock::now();
    result.serializeTimeMs = duration_cast<microseconds>(serializeEnd - serializeStart).count() / 1000.0;
    
    result.totalTimeMs = result.parseTimeMs + result.serializeTimeMs;
    result.throughputMBps = (result.fileSize / (1024.0 * 1024.0)) / (result.totalTimeMs / 1000.0);
    
    return result;
}

void printResults(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n========================================\n";
    std::cout << "       JSON PARSER BENCHMARK RESULTS\n";
    std::cout << "========================================\n\n";
    
    double totalSize = 0;
    double totalTime = 0;
    
    for (const auto& r : results) {
        std::cout << "File: " << r.filename << "\n";
        std::cout << "  Size: " << std::fixed << std::setprecision(2) 
                  << (r.fileSize / 1024.0) << " KB\n";
        std::cout << "  Parse Time: " << std::setprecision(3) << r.parseTimeMs << " ms\n";
        std::cout << "  Serialize Time: " << r.serializeTimeMs << " ms\n";
        std::cout << "  Total Time: " << r.totalTimeMs << " ms\n";
        std::cout << "  Throughput: " << std::setprecision(2) << r.throughputMBps << " MB/s\n";
        std::cout << "  Memory Used: " << (r.memoryUsed / 1024.0) << " KB\n";
        std::cout << "  Memory Overhead: " << std::setprecision(1) 
                  << (r.memoryUsed / (double)r.fileSize) << "x\n";
        std::cout << "  Nesting Depth: " << r.nestingDepth << "\n\n";
        
        totalSize += r.fileSize;
        totalTime += r.totalTimeMs;
    }
    
    std::cout << "========================================\n";
    std::cout << "AGGREGATE STATISTICS:\n";
    std::cout << "  Total Data Processed: " << std::setprecision(2) 
              << (totalSize / (1024.0 * 1024.0)) << " MB\n";
    std::cout << "  Total Time: " << std::setprecision(3) << totalTime << " ms\n";
    std::cout << "  Average Throughput: " << std::setprecision(2) 
              << (totalSize / (1024.0 * 1024.0)) / (totalTime / 1000.0) << " MB/s\n";
    std::cout << "========================================\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <json_file1> [json_file2] ...\n";
        return 1;
    }
    
    std::vector<BenchmarkResult> results;
    
    for (int i = 1; i < argc; i++) {
        try {
            std::cout << "Benchmarking: " << argv[i] << " ... " << std::flush;
            auto result = benchmarkFile(argv[i]);
            results.push_back(result);
            std::cout << "✓\n";
        } catch (const std::exception& e) {
            std::cerr << "✗ Error: " << e.what() << "\n";
        }
    }
    
    if (!results.empty()) {
        printResults(results);
    }
    
    return 0;
}
