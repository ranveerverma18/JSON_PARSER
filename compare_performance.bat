@echo off
echo ========================================
echo Performance Comparison Script
echo ========================================
echo.

echo Running BASELINE (without optimizations)...
echo.

REM Build without optimizations
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/parser.cpp src/JSONSerializer.cpp src/JSONprinter.cpp src/JSONNavigator.cpp src/JSONPath.cpp src/FileUtils.cpp src/JSONUtils.cpp -o json_parser_baseline.exe
g++ -std=c++17 -O2 -Iinclude src/benchmark.cpp src/parser.cpp src/JSONSerializer.cpp src/FileUtils.cpp src/JSONUtils.cpp -o benchmark_baseline.exe

echo Running baseline benchmarks...
benchmark_baseline.exe bench_*.json > baseline_results.txt
echo ✓ Baseline results saved to baseline_results.txt
echo.

echo ========================================
echo.

echo Running OPTIMIZED (with -O3 -march=native -flto + from_chars)...
echo.

REM Build with optimizations
build.bat > nul 2>&1

echo Running optimized benchmarks...
benchmark.exe bench_*.json > optimized_results.txt
echo ✓ Optimized results saved to optimized_results.txt
echo.

echo ========================================
echo COMPARISON
echo ========================================
echo.
echo Baseline results:
findstr /C:"Average Throughput" baseline_results.txt
echo.
echo Optimized results:
findstr /C:"Average Throughput" optimized_results.txt
echo.
echo ========================================
echo.
echo Full results saved in:
echo   - baseline_results.txt
echo   - optimized_results.txt
echo.
