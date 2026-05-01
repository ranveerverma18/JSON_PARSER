@echo off
echo ========================================
echo JSON Parser - Feature Showcase Demo
echo ========================================
echo.
echo This demo shows all 6 CLI commands:
echo   1. VALIDATE - Check JSON syntax
echo   2. PRETTY   - Format with indentation
echo   3. MINIFY   - Compact JSON
echo   4. SHOW     - Display parsed tree
echo   5. GET      - Extract values by path
echo   6. SET      - Update values by path
echo.
pause
cls

REM ============================================
echo ========================================
echo 1. VALIDATE Command
echo ========================================
echo.
echo Purpose: Validate JSON syntax
echo.
echo Command: json_parser.exe validate tests\test.json
echo.
json_parser.exe validate tests\test.json
echo.
echo Testing with invalid JSON...
echo {invalid json} > tests\temp_invalid.json
json_parser.exe validate tests\temp_invalid.json 2>nul || echo [Expected] Validation failed - syntax error detected
del tests\temp_invalid.json 2>nul
echo.
pause
cls

REM ============================================
echo ========================================
echo 2. PRETTY Command
echo ========================================
echo.
echo Purpose: Format JSON with indentation
echo.
echo Original file (tests\test.json):
type tests\test.json
echo.
echo.
echo Command: json_parser.exe pretty tests\test.json stdout
echo.
echo Formatted output:
json_parser.exe pretty tests\test.json stdout
echo.
echo Note: Without 'stdout', it writes to test_pretty.json
echo.
pause
cls

REM ============================================
echo ========================================
echo 3. MINIFY Command
echo ========================================
echo.
echo Purpose: Compact JSON (remove whitespace)
echo.
echo Command: json_parser.exe minify tests\test.json stdout
echo.
echo Minified output:
json_parser.exe minify tests\test.json stdout
echo.
echo Note: Without 'stdout', it writes to test_minified.json
echo.
pause
cls

REM ============================================
echo ========================================
echo 4. SHOW Command
echo ========================================
echo.
echo Purpose: Display parsed JSON tree structure
echo.
echo Command: json_parser.exe show tests\test.json
echo.
echo Parsed tree:
json_parser.exe show tests\test.json
echo.
echo Note: Useful for debugging and understanding structure
echo.
pause
cls

REM ============================================
echo ========================================
echo 5. GET Command
echo ========================================
echo.
echo Purpose: Extract values using path notation
echo.
echo Original JSON:
json_parser.exe show tests\test.json
echo.
echo.
echo Example 1: Get top-level value
echo Command: json_parser.exe get tests\test.json name
echo Result:
json_parser.exe get tests\test.json name
echo.
echo.
echo Example 2: Get array element
echo Command: json_parser.exe get tests\test.json skills[0]
echo Result:
json_parser.exe get tests\test.json skills[0]
echo.
echo.
echo Example 3: Get array element at index 2
echo Command: json_parser.exe get tests\test.json skills[2]
echo Result:
json_parser.exe get tests\test.json skills[2]
echo.
pause
cls

REM ============================================
echo ========================================
echo 6. SET Command
echo ========================================
echo.
echo Purpose: Update values in JSON
echo.
echo Original JSON:
json_parser.exe show tests\test.json
echo.
echo.
echo Example 1: Set boolean value
echo Command: json_parser.exe set tests\test.json active false stdout
echo Result:
json_parser.exe set tests\test.json active false stdout
echo.
echo.
echo Example 2: Set string value
echo Command: json_parser.exe set tests\test.json name \"John\" stdout
echo Result:
json_parser.exe set tests\test.json name \"John\" stdout
echo.
echo.
echo Example 3: Set number value
echo Command: json_parser.exe set tests\test.json age 25 stdout
echo Result:
json_parser.exe set tests\test.json age 25 stdout
echo.
echo Note: Without 'stdout', changes are saved to the file
echo.
pause
cls

REM ============================================
echo ========================================
echo Performance Benchmarks
echo ========================================
echo.
echo Running quick performance test...
echo.
benchmark.exe benchmarks\bench_string_heavy.json benchmarks\bench_deep_500.json
echo.
pause
cls

REM ============================================
echo ========================================
echo Demo Complete!
echo ========================================
echo.
echo You've seen all 6 commands:
echo   ✓ VALIDATE - Syntax checking
echo   ✓ PRETTY   - Formatting
echo   ✓ MINIFY   - Compacting
echo   ✓ SHOW     - Tree display
echo   ✓ GET      - Value extraction
echo   ✓ SET      - Value mutation
echo.
echo Performance:
echo   - Peak throughput: 79+ MB/s
echo   - Deep nesting: 500 levels
echo   - Memory efficiency: 1.3x overhead
echo.
echo For more info:
echo   - README.md
echo   - docs\METRICS_SUMMARY.md
echo   - PROJECT_COMPLETE.md
echo.
pause
