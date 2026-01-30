@echo off
REM ============================================================================
REM 파일명: compile_all.bat
REM 기능: 모든 C 디자인 패턴 예제 파일을 컴파일하는 배치 스크립트
REM ============================================================================

echo ========================================
echo C 디자인 패턴 예제 컴파일 스크립트
echo 총 31개 패턴 컴파일
echo ========================================
echo.

set TOTAL=31
set SUCCESS=0
set FAILED=0

REM 01-10번: 기본 패턴
echo [1/%TOTAL%] Wrapper 패턴...
gcc -o 01_wrapper.exe 01_wrapper_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [2/%TOTAL%] Adapter 패턴...
gcc -o 02_adapter.exe 02_adapter_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [3/%TOTAL%] Singleton 패턴...
gcc -o 03_singleton.exe 03_singleton_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [4/%TOTAL%] Callback 패턴...
gcc -o 04_callback.exe 04_callback_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [5/%TOTAL%] State Machine 패턴...
gcc -o 05_state_machine.exe 05_state_machine.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [6/%TOTAL%] Factory 패턴...
gcc -o 06_factory.exe 06_factory_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [7/%TOTAL%] Observer 패턴...
gcc -o 07_observer.exe 07_observer_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [8/%TOTAL%] Strategy 패턴...
gcc -o 08_strategy.exe 08_strategy_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [9/%TOTAL%] Command 패턴...
gcc -o 09_command.exe 09_command_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [10/%TOTAL%] Event Queue...
gcc -o 10_event_queue.exe 10_event_queue.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

REM 11-16번: 메모리/데이터 구조
echo [11/%TOTAL%] Memory Pool...
gcc -o 11_memory_pool.exe 11_memory_pool.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [12/%TOTAL%] Object Pool...
gcc -o 12_object_pool.exe 12_object_pool.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [13/%TOTAL%] Reference Counting...
gcc -o 13_reference_counting.exe 13_reference_counting.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [14/%TOTAL%] Ring Buffer...
gcc -o 14_ring_buffer.exe 14_ring_buffer.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [15/%TOTAL%] Linked List...
gcc -o 15_linked_list.exe 15_linked_list.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [16/%TOTAL%] Double Buffer...
gcc -o 16_double_buffer.exe 16_double_buffer.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

REM 17-22번: 리소스/성능
echo [17/%TOTAL%] RAII...
gcc -o 17_raii.exe 17_raii_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [18/%TOTAL%] Semaphore...
gcc -o 18_semaphore.exe 18_semaphore_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [19/%TOTAL%] Mutex Guard...
gcc -o 19_mutex_guard.exe 19_mutex_guard.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [20/%TOTAL%] Lazy Initialization...
gcc -o 20_lazy_init.exe 20_lazy_initialization.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [21/%TOTAL%] Cache...
gcc -o 21_cache.exe 21_cache_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [22/%TOTAL%] Zero-Copy...
gcc -o 22_zero_copy.exe 22_zero_copy.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

REM 23-28번: 안전성/하드웨어
echo [23/%TOTAL%] Watchdog...
gcc -o 23_watchdog.exe 23_watchdog_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [24/%TOTAL%] Retry...
gcc -o 24_retry.exe 24_retry_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [25/%TOTAL%] Failsafe...
gcc -o 25_failsafe.exe 25_failsafe_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [26/%TOTAL%] HAL...
gcc -o 26_hal.exe 26_hal_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [27/%TOTAL%] Driver Interface...
gcc -o 27_driver_interface.exe 27_driver_interface.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [28/%TOTAL%] Interrupt Handler...
gcc -o 28_interrupt_handler.exe 28_interrupt_handler.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

REM 29-31번: 테스트/디버깅
echo [29/%TOTAL%] Mock Object...
gcc -o 29_mock_object.exe 29_mock_object.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [30/%TOTAL%] Assertion...
gcc -o 30_assertion.exe 30_assertion_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo [31/%TOTAL%] Tracing...
gcc -o 31_tracing.exe 31_tracing_pattern.c -Wall && set /A SUCCESS+=1 || set /A FAILED+=1

echo.
echo ========================================
echo 컴파일 완료
echo ========================================
echo 성공: %SUCCESS%개
echo 실패: %FAILED%개
echo.
pause
