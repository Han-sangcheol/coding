# 헤더 파일 분리 학습 가이드

## 개요

이 문서는 현재 프로젝트의 단일 파일 구조(.c, .cpp, .py)를 실제 프로젝트에서 사용하는 헤더 파일과 소스 파일로 분리하는 방법을 설명합니다.

**왜 학습용은 단일 파일로 작성했나?**
- 한 파일에서 전체 흐름을 파악하기 쉬움
- 예제 실행과 학습에 집중
- 컴파일 간편 (단일 명령어)

**실무에서는 왜 분리하나?**
- 재사용성 (다른 프로젝트에서 헤더만 include)
- 컴파일 시간 최적화 (변경된 .c/.cpp만 재컴파일)
- 인터페이스와 구현 분리 (API 공개/숨김)
- 팀 협업 용이 (헤더 = 계약, 소스 = 구현)
- 라이브러리 배포 (.h + .a/.lib 형태)

---

## 목차

1. [C 언어 - 헤더/소스 파일 분리](#1-c-언어---헤더소스-파일-분리)
2. [C++ 언어 - 헤더/소스 파일 분리](#2-c-언어---헤더소스-파일-분리)
3. [Python - 모듈/패키지 구조](#3-python---모듈패키지-구조)
4. [언어별 비교표](#4-언어별-비교표)
5. [실무 예제](#5-실무-예제)
6. [주의사항](#6-주의사항)

---

## 1. C 언어 - 헤더/소스 파일 분리

### 1.1 기본 원칙

#### 헤더 파일 (.h)에 작성할 내容

```c
/* ============================================================================
 * sensor_factory.h - 센서 팩토리 헤더 파일
 * ============================================================================ */

#ifndef SENSOR_FACTORY_H    // Include Guard 시작
#define SENSOR_FACTORY_H

#include <stdint.h>

/* ============================================================================
 * 1. 매크로 정의
 * ============================================================================ */
#define MAX_SENSOR_NAME 32
#define SENSOR_VERSION 1

/* ============================================================================
 * 2. 타입 정의 (typedef, enum, struct)
 * ============================================================================ */

// 센서 타입 열거형
typedef enum SensorType {
    SENSOR_TYPE_TEMPERATURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_PRESSURE
} SensorType;

// 센서 구조체
typedef struct Sensor {
    SensorType type;
    int sensor_id;
    float (*read)(struct Sensor* self);
    void (*init)(struct Sensor* self);
    void (*destroy)(struct Sensor* self);
    char name[MAX_SENSOR_NAME];
} Sensor;

/* ============================================================================
 * 3. 함수 선언 (프로토타입만)
 * ============================================================================ */

// Factory 함수 선언
Sensor* SensorFactory_Create(SensorType type, int sensor_id);

// 유틸리티 함수 선언
void SensorFactory_PrintInfo(const Sensor* sensor);

/* ============================================================================
 * 4. 전역 변수 extern 선언 (정의는 .c에)
 * ============================================================================ */
extern int g_sensor_count;

#endif  // SENSOR_FACTORY_H    // Include Guard 끝
```

#### 소스 파일 (.c)에 작성할 내용

```c
/* ============================================================================
 * sensor_factory.c - 센서 팩토리 구현 파일
 * ============================================================================ */

#include "sensor_factory.h"  // 자신의 헤더 파일
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * 1. static 변수 (이 파일 내부에서만 사용)
 * ============================================================================ */
static int s_factory_initialized = 0;

/* ============================================================================
 * 2. 전역 변수 정의 (헤더에서 extern 선언된 것)
 * ============================================================================ */
int g_sensor_count = 0;

/* ============================================================================
 * 3. static 함수 (내부 구현 함수, 외부에서 접근 불가)
 * ============================================================================ */
static void Internal_InitFactory(void) {
    if (!s_factory_initialized) {
        printf("[Factory] 내부 초기화\n");
        s_factory_initialized = 1;
    }
}

/* ============================================================================
 * 4. 각 센서 타입별 구현 함수들
 * ============================================================================ */

// 온도 센서 함수들
static float Temperature_Read(Sensor* self) {
    float raw = 20.0f + (rand() % 100) / 10.0f;
    return raw;
}

static void Temperature_Init(Sensor* self) {
    printf("[Factory] 온도 센서 %d 초기화\n", self->sensor_id);
}

static void Temperature_Destroy(Sensor* self) {
    printf("[Factory] 온도 센서 %d 해제\n", self->sensor_id);
    free(self);
}

/* ============================================================================
 * 5. public 함수 구현 (헤더에 선언된 함수)
 * ============================================================================ */

// Factory 함수 구현
Sensor* SensorFactory_Create(SensorType type, int sensor_id) {
    Internal_InitFactory();  // static 함수 호출
    
    Sensor* sensor = NULL;
    
    switch (type) {
        case SENSOR_TYPE_TEMPERATURE: {
            sensor = (Sensor*)malloc(sizeof(Sensor));
            if (sensor) {
                sensor->type = type;
                sensor->sensor_id = sensor_id;
                sensor->read = Temperature_Read;
                sensor->init = Temperature_Init;
                sensor->destroy = Temperature_Destroy;
                snprintf(sensor->name, MAX_SENSOR_NAME, "Temperature_%d", sensor_id);
                g_sensor_count++;  // 전역 변수 사용
            }
            break;
        }
        // ... 다른 타입들
    }
    
    return sensor;
}

// 유틸리티 함수 구현
void SensorFactory_PrintInfo(const Sensor* sensor) {
    if (sensor) {
        printf("센서 [%s] ID=%d, 타입=%d\n", 
               sensor->name, sensor->sensor_id, sensor->type);
    }
}
```

#### 사용자 파일 (main.c)

```c
/* ============================================================================
 * main.c - 사용자 코드
 * ============================================================================ */

#include "sensor_factory.h"  // 헤더만 include
#include <stdio.h>

int main(void) {
    // Factory 사용
    Sensor* temp = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 101);
    
    if (temp) {
        temp->init(temp);
        float value = temp->read(temp);
        printf("온도: %.2f\n", value);
        temp->destroy(temp);
    }
    
    printf("총 생성된 센서 수: %d\n", g_sensor_count);
    
    return 0;
}
```

#### 컴파일 방법

```bash
# 방법 1: 한 번에 컴파일
gcc main.c sensor_factory.c -o sensor_app.exe

# 방법 2: 분리 컴파일 (권장)
gcc -c sensor_factory.c -o sensor_factory.o   # .o 파일 생성
gcc -c main.c -o main.o
gcc sensor_factory.o main.o -o sensor_app.exe # 링크

# 방법 3: Makefile 사용 (대규모 프로젝트)
make
```

### 1.2 Include Guard 상세

**왜 필요한가?**
- 같은 헤더를 여러 번 include하는 것 방지
- 컴파일 오류 방지 (중복 정의)

```c
// sensor_factory.h
#ifndef SENSOR_FACTORY_H  // 첫 include 시: 정의 안 됨 → 내부 실행
#define SENSOR_FACTORY_H  // 이제 정의됨

// ... 헤더 내용 ...

#endif  // 두 번째 include 시: 이미 정의됨 → 내부 스킵
```

**명명 규칙**:
- 파일명을 대문자로 변환
- `.`을 `_`로 변환
- 예: `sensor_factory.h` → `SENSOR_FACTORY_H`

### 1.3 현재 프로젝트 파일 분리 예시

현재 `06_factory_pattern.c` (약 600줄)를 분리한다면:

```
06_factory_pattern.c (단일 파일)
    ↓
sensor_factory.h          (약 80줄)  - 공개 API
temperature_sensor.c      (약 100줄) - 온도 센서 구현
humidity_sensor.c         (약 100줄) - 습도 센서 구현
pressure_sensor.c         (약 100줄) - 압력 센서 구현
sensor_factory.c          (약 150줄) - Factory 구현
comm_factory.h            (약 50줄)  - 통신 Factory API
comm_factory.c            (약 120줄) - 통신 Factory 구현
main.c                    (약 50줄)  - 사용 예제
```

---

## 2. C++ 언어 - 헤더/소스 파일 분리

### 2.1 기본 원칙

#### 헤더 파일 (.hpp 또는 .h)에 작성할 내용

```cpp
/* ============================================================================
 * CallbackManager.hpp - 콜백 관리자 헤더 파일
 * ============================================================================ */

#pragma once  // Modern C++ (또는 include guard 사용)
// #ifndef CALLBACK_MANAGER_HPP
// #define CALLBACK_MANAGER_HPP

#include <functional>
#include <vector>
#include <string>

namespace patterns {

/* ============================================================================
 * 1. 클래스 선언
 * ============================================================================ */

class CallbackManager {
public:
    // 타입 별칭 (public)
    using Callback = std::function<void(int)>;
    using EventCallback = std::function<void(const std::string&)>;
    
    // 생성자/소멸자 선언
    CallbackManager();
    ~CallbackManager();
    
    // public 멤버 함수 선언
    void RegisterCallback(Callback cb);
    void TriggerCallbacks(int value);
    int GetCallbackCount() const;
    
    // inline 함수 (헤더에 구현 가능)
    inline bool IsEmpty() const {
        return callbacks_.empty();
    }
    
private:
    // private 멤버 변수
    std::vector<Callback> callbacks_;
    int callback_count_;
    
    // private 멤버 함수 선언
    void InternalInit();
};

/* ============================================================================
 * 2. 템플릿 클래스 (전체를 헤더에 작성)
 * ============================================================================ */

template<typename T>
class EventDispatcher {
public:
    void Register(std::function<void(T)> handler) {
        handlers_.push_back(handler);
    }
    
    void Dispatch(T event) {
        for (auto& handler : handlers_) {
            handler(event);
        }
    }
    
private:
    std::vector<std::function<void(T)>> handlers_;
};

/* ============================================================================
 * 3. 유틸리티 함수 선언
 * ============================================================================ */

void PrintCallbackInfo(const CallbackManager& manager);

// inline 전역 함수
inline std::string GetVersion() {
    return "1.0.0";
}

}  // namespace patterns

// #endif  // CALLBACK_MANAGER_HPP
```

#### 소스 파일 (.cpp)에 작성할 내용

```cpp
/* ============================================================================
 * CallbackManager.cpp - 콜백 관리자 구현 파일
 * ============================================================================ */

#include "CallbackManager.hpp"
#include <iostream>

namespace patterns {

/* ============================================================================
 * 1. static 변수 (파일 스코프)
 * ============================================================================ */
static int s_total_managers_created = 0;

/* ============================================================================
 * 2. 익명 namespace (내부 구현 함수, C의 static과 유사)
 * ============================================================================ */
namespace {
    void InternalLog(const std::string& msg) {
        std::cout << "[Internal] " << msg << std::endl;
    }
}

/* ============================================================================
 * 3. 클래스 멤버 함수 구현
 * ============================================================================ */

// 생성자 구현
CallbackManager::CallbackManager() 
    : callback_count_(0) {
    InternalInit();
    s_total_managers_created++;
    InternalLog("CallbackManager 생성");
}

// 소멸자 구현
CallbackManager::~CallbackManager() {
    InternalLog("CallbackManager 소멸");
}

// private 멤버 함수 구현
void CallbackManager::InternalInit() {
    callbacks_.reserve(10);  // 미리 메모리 할당
}

// public 멤버 함수 구현
void CallbackManager::RegisterCallback(Callback cb) {
    callbacks_.push_back(cb);
    callback_count_++;
    std::cout << "콜백 등록됨 (총 " << callback_count_ << "개)" << std::endl;
}

void CallbackManager::TriggerCallbacks(int value) {
    std::cout << "콜백 실행: value=" << value << std::endl;
    for (auto& cb : callbacks_) {
        cb(value);
    }
}

int CallbackManager::GetCallbackCount() const {
    return callback_count_;
}

/* ============================================================================
 * 4. 전역 함수 구현
 * ============================================================================ */

void PrintCallbackInfo(const CallbackManager& manager) {
    std::cout << "CallbackManager 정보:" << std::endl;
    std::cout << "  등록된 콜백 수: " << manager.GetCallbackCount() << std::endl;
    std::cout << "  비어있음: " << (manager.IsEmpty() ? "예" : "아니오") << std::endl;
}

}  // namespace patterns
```

#### 사용자 파일 (main.cpp)

```cpp
#include "CallbackManager.hpp"
#include <iostream>

using namespace patterns;

int main() {
    // CallbackManager 사용
    CallbackManager manager;
    
    // 람다 등록
    manager.RegisterCallback([](int x) {
        std::cout << "콜백1: " << x << std::endl;
    });
    
    manager.RegisterCallback([](int x) {
        std::cout << "콜백2: " << x * 2 << std::endl;
    });
    
    // 실행
    manager.TriggerCallbacks(10);
    
    // 정보 출력
    PrintCallbackInfo(manager);
    
    // 템플릿 사용
    EventDispatcher<std::string> dispatcher;
    dispatcher.Register([](const std::string& s) {
        std::cout << "이벤트: " << s << std::endl;
    });
    dispatcher.Dispatch("테스트 이벤트");
    
    return 0;
}
```

#### 컴파일 방법

```bash
# 방법 1: 한 번에 컴파일
g++ -std=c++17 main.cpp CallbackManager.cpp -o callback_app.exe

# 방법 2: 분리 컴파일 (권장)
g++ -std=c++17 -c CallbackManager.cpp -o CallbackManager.o
g++ -std=c++17 -c main.cpp -o main.o
g++ CallbackManager.o main.o -o callback_app.exe

# 방법 3: CMake 사용 (대규모 프로젝트)
cmake . && make
```

### 2.2 #pragma once vs Include Guard

```cpp
// 방법 1: #pragma once (Modern C++, 권장)
#pragma once
// ... 헤더 내용 ...

// 방법 2: Include Guard (전통적 방식, 이식성 높음)
#ifndef CALLBACK_MANAGER_HPP
#define CALLBACK_MANAGER_HPP
// ... 헤더 내용 ...
#endif
```

**비교**:
| 항목 | #pragma once | Include Guard |
|------|--------------|---------------|
| 간결성 | 매우 간결 (1줄) | 3줄 필요 |
| 이식성 | 대부분 컴파일러 지원 | 100% 호환 |
| 성능 | 약간 빠름 | 표준적 |
| 오타 가능성 | 없음 | Guard 이름 중복 가능 |

**권장**: 모던 C++ 프로젝트는 `#pragma once`, 레거시 호환은 Include Guard

### 2.3 템플릿은 왜 헤더에 전체 구현?

```cpp
// ❌ 잘못된 예 (컴파일 에러!)
// EventDispatcher.hpp
template<typename T>
class EventDispatcher {
    void Dispatch(T event);  // 선언만
};

// EventDispatcher.cpp
template<typename T>
void EventDispatcher<T>::Dispatch(T event) {  // 구현
    // ...
}
// → 링크 에러! 템플릿은 사용 시점에 인스턴스화되어야 함

// ✅ 올바른 예
// EventDispatcher.hpp
template<typename T>
class EventDispatcher {
    void Dispatch(T event) {  // 헤더에 구현
        // ...
    }
};
```

### 2.4 현재 프로젝트 파일 분리 예시

현재 `00_function_pointer_basics.cpp` (약 680줄)를 분리한다면:

```
00_function_pointer_basics.cpp (단일 파일)
    ↓
FunctionPointer.hpp        (약 120줄) - std::function 래퍼
Lambda.hpp                 (약 80줄)  - 람다 유틸리티
Functor.hpp                (약 60줄)  - 함수 객체 클래스
CallbackManager.hpp        (약 80줄)  - 콜백 관리자
CallbackManager.cpp        (약 150줄) - 콜백 구현
ObserverPattern.hpp        (약 70줄)  - 옵저버 패턴
ObserverPattern.cpp        (약 100줄) - 옵저버 구현
main.cpp                   (약 100줄) - 사용 예제
```

---

## 3. Python - 모듈/패키지 구조

### 3.1 단일 파일 vs 패키지 구조

#### 단일 파일 (현재 학습용)

```python
# sensor_factory.py (단일 파일, 약 300줄)

from enum import Enum
from typing import Protocol, Callable
import random

class SensorType(Enum):
    TEMPERATURE = 1
    HUMIDITY = 2
    PRESSURE = 3

class Sensor(Protocol):
    """센서 인터페이스"""
    def read(self) -> float: ...
    def init(self) -> None: ...

class TemperatureSensor:
    def __init__(self, sensor_id: int):
        self.sensor_id = sensor_id
        self.name = f"Temperature_{sensor_id}"
    
    def read(self) -> float:
        return 20.0 + random.random() * 10.0
    
    def init(self) -> None:
        print(f"[Factory] 온도 센서 {self.sensor_id} 초기화")

class SensorFactory:
    @staticmethod
    def create(sensor_type: SensorType, sensor_id: int) -> Sensor:
        if sensor_type == SensorType.TEMPERATURE:
            return TemperatureSensor(sensor_id)
        # ...

# 사용 예제
if __name__ == "__main__":
    temp = SensorFactory.create(SensorType.TEMPERATURE, 101)
    temp.init()
    print(f"온도: {temp.read():.2f}")
```

#### 패키지 구조 (실무)

```
sensor_package/
├── __init__.py              # 패키지 초기화
├── sensor_types.py          # 타입 정의
├── base.py                  # 기본 클래스/프로토콜
├── temperature_sensor.py    # 온도 센서
├── humidity_sensor.py       # 습도 센서
├── pressure_sensor.py       # 압력 센서
├── factory.py               # Factory 클래스
└── utils.py                 # 유틸리티

examples/
└── main.py                  # 사용 예제

tests/
└── test_factory.py          # 테스트
```

### 3.2 각 파일 내용

#### `__init__.py` (패키지 진입점)

```python
"""
sensor_package - 센서 팩토리 패키지

사용 예:
    from sensor_package import SensorFactory, SensorType
    
    sensor = SensorFactory.create(SensorType.TEMPERATURE, 101)
"""

__version__ = "1.0.0"
__author__ = "Your Name"

# 공개 API 정의
__all__ = [
    "SensorFactory",
    "SensorType",
    "Sensor",
    "TemperatureSensor",
    "HumiditySensor",
    "PressureSensor",
]

# 하위 모듈에서 import
from .sensor_types import SensorType
from .base import Sensor
from .temperature_sensor import TemperatureSensor
from .humidity_sensor import HumiditySensor
from .pressure_sensor import PressureSensor
from .factory import SensorFactory

# 패키지 레벨 함수
def get_version() -> str:
    """패키지 버전 반환"""
    return __version__
```

#### `sensor_types.py` (타입 정의)

```python
"""센서 타입 정의"""

from enum import Enum, auto

class SensorType(Enum):
    """센서 타입 열거형"""
    TEMPERATURE = auto()
    HUMIDITY = auto()
    PRESSURE = auto()

# 상수 정의
MAX_SENSOR_NAME_LENGTH = 32
DEFAULT_SAMPLE_RATE = 1.0  # Hz
```

#### `base.py` (기본 클래스)

```python
"""센서 기본 인터페이스"""

from typing import Protocol
from abc import ABC, abstractmethod

# 방법 1: Protocol (Duck Typing, Python 3.8+)
class Sensor(Protocol):
    """센서 인터페이스 (Protocol)"""
    sensor_id: int
    name: str
    
    def read(self) -> float:
        """센서 값 읽기"""
        ...
    
    def init(self) -> None:
        """센서 초기화"""
        ...

# 방법 2: ABC (Abstract Base Class, 전통적 방식)
class SensorABC(ABC):
    """센서 추상 클래스"""
    
    def __init__(self, sensor_id: int):
        self.sensor_id = sensor_id
        self.name = ""
    
    @abstractmethod
    def read(self) -> float:
        """센서 값 읽기 (구현 필수)"""
        pass
    
    @abstractmethod
    def init(self) -> None:
        """센서 초기화 (구현 필수)"""
        pass
```

#### `temperature_sensor.py` (온도 센서)

```python
"""온도 센서 구현"""

import random
from .base import Sensor

class TemperatureSensor:
    """온도 센서"""
    
    def __init__(self, sensor_id: int):
        self.sensor_id = sensor_id
        self.name = f"Temperature_{sensor_id}"
        self._offset = 0.0
        self._scale = 1.0
    
    def read(self) -> float:
        """온도 읽기 (°C)"""
        raw = 20.0 + random.random() * 10.0
        return raw * self._scale + self._offset
    
    def init(self) -> None:
        """센서 초기화"""
        self._offset = 0.0
        self._scale = 1.0
        print(f"[Factory] 온도 센서 {self.sensor_id} 초기화")
    
    def calibrate(self, offset: float, scale: float) -> None:
        """캘리브레이션"""
        self._offset = offset
        self._scale = scale

# Private 함수 (언더스코어로 시작)
def _internal_helper():
    """내부 헬퍼 함수 (외부에서 사용 X)"""
    pass
```

#### `factory.py` (Factory)

```python
"""센서 팩토리"""

from typing import Union
from .sensor_types import SensorType
from .base import Sensor
from .temperature_sensor import TemperatureSensor
from .humidity_sensor import HumiditySensor
from .pressure_sensor import PressureSensor

class SensorFactory:
    """센서 생성 팩토리"""
    
    # 클래스 변수 (모든 인스턴스가 공유)
    _sensor_count = 0
    
    @staticmethod
    def create(sensor_type: SensorType, sensor_id: int) -> Sensor:
        """센서 생성
        
        Args:
            sensor_type: 센서 타입
            sensor_id: 센서 ID
            
        Returns:
            생성된 센서 인스턴스
            
        Raises:
            ValueError: 지원하지 않는 센서 타입
        """
        SensorFactory._sensor_count += 1
        
        if sensor_type == SensorType.TEMPERATURE:
            return TemperatureSensor(sensor_id)
        elif sensor_type == SensorType.HUMIDITY:
            return HumiditySensor(sensor_id)
        elif sensor_type == SensorType.PRESSURE:
            return PressureSensor(sensor_id)
        else:
            raise ValueError(f"지원하지 않는 센서 타입: {sensor_type}")
    
    @classmethod
    def get_sensor_count(cls) -> int:
        """생성된 센서 총 개수"""
        return cls._sensor_count
    
    @classmethod
    def reset_count(cls) -> None:
        """카운터 리셋"""
        cls._sensor_count = 0
```

#### `utils.py` (유틸리티)

```python
"""유틸리티 함수"""

from typing import TYPE_CHECKING

# 순환 import 방지
if TYPE_CHECKING:
    from .base import Sensor

def print_sensor_info(sensor: 'Sensor') -> None:
    """센서 정보 출력"""
    print(f"센서 [{sensor.name}] ID={sensor.sensor_id}")

def format_sensor_value(value: float, unit: str = "°C") -> str:
    """센서 값 포맷팅"""
    return f"{value:.2f} {unit}"
```

#### `main.py` (사용 예제)

```python
"""사용 예제"""

# 방법 1: 전체 패키지 import
import sensor_package

sensor = sensor_package.SensorFactory.create(
    sensor_package.SensorType.TEMPERATURE, 
    101
)

# 방법 2: 필요한 것만 import (권장)
from sensor_package import SensorFactory, SensorType
from sensor_package.utils import print_sensor_info

def main():
    # 센서 생성
    temp = SensorFactory.create(SensorType.TEMPERATURE, 101)
    
    # 초기화 및 사용
    temp.init()
    value = temp.read()
    print(f"온도: {value:.2f}°C")
    
    # 유틸리티 사용
    print_sensor_info(temp)
    
    # Factory 정보
    print(f"생성된 센서 수: {SensorFactory.get_sensor_count()}")

if __name__ == "__main__":
    main()
```

### 3.3 Import 방식

```python
# 절대 import (권장)
from sensor_package import SensorFactory
from sensor_package.temperature_sensor import TemperatureSensor

# 상대 import (패키지 내부에서)
# factory.py 내부에서
from .sensor_types import SensorType        # 같은 패키지
from .base import Sensor                    # 같은 패키지
from ..utils import helper_function         # 상위 패키지

# 별칭 사용
from sensor_package import SensorFactory as SF
sensor = SF.create(...)

# 전체 import (비권장, namespace 오염)
from sensor_package import *  # __all__에 정의된 것만
```

### 3.4 Public vs Private 관례

```python
# Public (외부에서 사용 가능)
class SensorFactory:        # 클래스
    def create(self):       # 메서드
        pass

def print_info():           # 함수
    pass

# Private (내부 사용, 언더스코어 시작)
class _InternalHelper:      # Private 클래스
    pass

def _internal_function():   # Private 함수
    pass

class Sensor:
    def __init__(self):
        self.id = 1         # Public 속성
        self._offset = 0.0  # Protected (관례상)
        self.__secret = 0   # Name mangling (진짜 private)
```

### 3.5 `if __name__ == "__main__":` 의미

```python
# sensor_factory.py

class SensorFactory:
    # ... 클래스 정의 ...
    pass

# 이 부분은 직접 실행할 때만 실행됨
if __name__ == "__main__":
    print("직접 실행됨!")
    # 테스트 코드
    sensor = SensorFactory.create(...)
    
# 다른 파일에서 import할 때는 실행 안 됨
# from sensor_factory import SensorFactory  # 위 print 안 나옴
```

### 3.6 현재 프로젝트 파일 분리 예시

현재 학습용 단일 `.py` 파일을 분리한다면:

```
factory_pattern.py (단일 파일)
    ↓
sensor_package/
├── __init__.py
├── sensor_types.py         # Enum, 상수
├── base.py                 # Protocol, ABC
├── temperature_sensor.py
├── humidity_sensor.py
├── pressure_sensor.py
├── factory.py
└── utils.py

examples/
└── main.py

tests/
└── test_factory.py
```

---

## 4. 언어별 비교표

### 4.1 파일 분리 개념 비교

| 항목 | C | C++ | Python |
|------|---|-----|--------|
| **헤더 파일** | `.h` | `.hpp`, `.h` | 없음 |
| **소스 파일** | `.c` | `.cpp` | `.py` |
| **중복 방지** | Include Guard | Include Guard / `#pragma once` | Import 시스템 (자동) |
| **가시성 제어** | `static` (파일 스코프) | `namespace`, `private`, `anonymous namespace` | `_`, `__`, `__all__` |
| **모듈 시스템** | 없음 (헤더 include) | 없음 (C++20 modules는 신기술) | 내장 (import) |
| **템플릿/제네릭** | 매크로 | 헤더에 전체 구현 | 타입 힌트 (런타임 무관) |

### 4.2 무엇을 어디에 작성하는가?

| 내용 | C 헤더 | C 소스 | C++ 헤더 | C++ 소스 | Python |
|------|--------|--------|----------|----------|--------|
| 타입 정의 (struct/class) | ✅ | ❌ | ✅ | ❌ | ✅ (.py) |
| 함수 선언 | ✅ | ❌ | ✅ | ❌ | ❌ (불필요) |
| 함수 구현 | ❌ | ✅ | ❌ | ✅ | ✅ (.py) |
| inline 함수 | ✅ | ❌ | ✅ | ❌ | N/A |
| 템플릿 | N/A | N/A | ✅ (전체) | ❌ | N/A |
| 매크로 | ✅ | 가능 | ✅ | 가능 | N/A |
| static 변수 | ❌ (extern 선언만) | ✅ | ❌ (extern 선언만) | ✅ | ✅ (모듈 레벨) |
| 내부 함수 | ❌ | ✅ (static) | ❌ | ✅ (anonymous ns) | ✅ (`_`로 시작) |

### 4.3 컴파일/실행 비교

| 단계 | C | C++ | Python |
|------|---|-----|--------|
| **컴파일** | `gcc -c file.c` | `g++ -c file.cpp` | 없음 (인터프리터) |
| **링크** | `gcc *.o -o app` | `g++ *.o -o app` | 없음 |
| **실행** | `./app` | `./app` | `python main.py` |
| **증분 빌드** | .o 파일로 가능 | .o 파일로 가능 | 필요 없음 |
| **배포** | 헤더 + 라이브러리 | 헤더 + 라이브러리 | 소스 또는 wheel |

---

## 5. 실무 예제

### 5.1 C 언어 - 임베디드 HAL 구조

실제 STM32 프로젝트 구조:

```
project/
├── Inc/                    # 헤더 파일
│   ├── main.h
│   ├── gpio_wrapper.h
│   ├── uart_wrapper.h
│   └── sensor_manager.h
├── Src/                    # 소스 파일
│   ├── main.c
│   ├── gpio_wrapper.c
│   ├── uart_wrapper.c
│   └── sensor_manager.c
├── Drivers/                # HAL 드라이버
│   └── STM32F4xx_HAL_Driver/
└── Makefile
```

**gpio_wrapper.h** (공개 API):
```c
#ifndef GPIO_WRAPPER_H
#define GPIO_WRAPPER_H

#include <stdint.h>

// 공개 함수
void GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
uint8_t Button_IsPressed(void);

#endif
```

**gpio_wrapper.c** (구현):
```c
#include "gpio_wrapper.h"
#include "stm32f4xx_hal.h"  // HAL

// 내부 함수 (static)
static void Internal_ConfigurePin(void) {
    // HAL 호출
    HAL_GPIO_Init(...);
}

// 공개 함수 구현
void LED_On(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}
```

**main.c** (사용):
```c
#include "gpio_wrapper.h"  // HAL을 직접 include하지 않음!

int main(void) {
    GPIO_Init();
    LED_On();
    while (1) {
        if (Button_IsPressed()) {
            LED_Off();
        }
    }
}
```

### 5.2 C++ 언어 - 게임 엔진 구조

실제 게임 엔진 프로젝트:

```
GameEngine/
├── include/
│   └── Engine/
│       ├── Core/
│       │   ├── GameObject.hpp
│       │   └── Component.hpp
│       ├── Rendering/
│       │   ├── Renderer.hpp
│       │   └── Camera.hpp
│       └── Physics/
│           └── RigidBody.hpp
├── src/
│   ├── Core/
│   │   ├── GameObject.cpp
│   │   └── Component.cpp
│   ├── Rendering/
│   │   ├── Renderer.cpp
│   │   └── Camera.cpp
│   └── Physics/
│       └── RigidBody.cpp
├── examples/
│   └── main.cpp
└── CMakeLists.txt
```

**GameObject.hpp**:
```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Engine {

class Component;  // 전방 선언

class GameObject {
public:
    GameObject(const std::string& name);
    ~GameObject();
    
    void Update(float deltaTime);
    void AddComponent(std::shared_ptr<Component> comp);
    
    template<typename T>
    std::shared_ptr<T> GetComponent();  // 템플릿은 헤더에
    
private:
    std::string name_;
    std::vector<std::shared_ptr<Component>> components_;
};

// 템플릿 구현 (헤더에)
template<typename T>
std::shared_ptr<T> GameObject::GetComponent() {
    for (auto& comp : components_) {
        if (auto result = std::dynamic_pointer_cast<T>(comp)) {
            return result;
        }
    }
    return nullptr;
}

}  // namespace Engine
```

### 5.3 Python - 웹 API 패키지 구조

Flask 기반 API 서버:

```
myapi/
├── __init__.py
├── app.py                  # Flask 앱
├── config.py               # 설정
├── models/                 # 데이터 모델
│   ├── __init__.py
│   ├── user.py
│   └── sensor.py
├── routes/                 # API 라우트
│   ├── __init__.py
│   ├── users.py
│   └── sensors.py
├── services/               # 비즈니스 로직
│   ├── __init__.py
│   └── sensor_service.py
└── utils/                  # 유틸리티
    ├── __init__.py
    └── validators.py

run.py                      # 실행 진입점
requirements.txt
```

**models/sensor.py**:
```python
"""센서 데이터 모델"""

from dataclasses import dataclass
from datetime import datetime
from typing import Optional

@dataclass
class Sensor:
    """센서 모델"""
    id: int
    name: str
    sensor_type: str
    value: float
    timestamp: datetime
    
    def to_dict(self) -> dict:
        """딕셔너리로 변환 (JSON 응답용)"""
        return {
            "id": self.id,
            "name": self.name,
            "type": self.sensor_type,
            "value": self.value,
            "timestamp": self.timestamp.isoformat()
        }
```

**services/sensor_service.py**:
```python
"""센서 비즈니스 로직"""

from typing import List, Optional
from ..models.sensor import Sensor

class SensorService:
    """센서 관리 서비스"""
    
    def __init__(self):
        self._sensors: List[Sensor] = []
    
    def create_sensor(self, name: str, sensor_type: str) -> Sensor:
        """센서 생성"""
        sensor = Sensor(
            id=len(self._sensors) + 1,
            name=name,
            sensor_type=sensor_type,
            value=0.0,
            timestamp=datetime.now()
        )
        self._sensors.append(sensor)
        return sensor
    
    def get_sensor(self, sensor_id: int) -> Optional[Sensor]:
        """ID로 센서 조회"""
        for sensor in self._sensors:
            if sensor.id == sensor_id:
                return sensor
        return None
```

**routes/sensors.py**:
```python
"""센서 API 라우트"""

from flask import Blueprint, jsonify, request
from ..services.sensor_service import SensorService

# Blueprint 생성
sensors_bp = Blueprint('sensors', __name__, url_prefix='/api/sensors')
sensor_service = SensorService()

@sensors_bp.route('/', methods=['GET'])
def get_sensors():
    """모든 센서 조회"""
    # ... 구현 ...
    return jsonify({"sensors": []})

@sensors_bp.route('/<int:sensor_id>', methods=['GET'])
def get_sensor(sensor_id: int):
    """특정 센서 조회"""
    sensor = sensor_service.get_sensor(sensor_id)
    if sensor:
        return jsonify(sensor.to_dict())
    return jsonify({"error": "Not found"}), 404
```

**run.py** (진입점):
```python
"""API 서버 실행"""

from myapi import create_app

if __name__ == "__main__":
    app = create_app()
    app.run(debug=True, host='0.0.0.0', port=5000)
```

---

## 6. 주의사항

### 6.1 C/C++ 공통 주의사항

#### 1. 순환 참조 (Circular Dependency)

**문제**:
```c
// sensor.h
#include "factory.h"
typedef struct Sensor { ... } Sensor;

// factory.h
#include "sensor.h"  // ← 순환!
Sensor* Create(void);
```

**해결책 - 전방 선언 (Forward Declaration)**:
```c
// sensor.h
// #include "factory.h"  ← 제거
typedef struct Sensor Sensor;  // 전방 선언

// factory.h
typedef struct Sensor Sensor;  // 전방 선언 (include 불필요)
Sensor* Create(void);
```

#### 2. 헤더에 구현 넣지 않기

**잘못된 예**:
```c
// sensor.h
void Print(void) {  // ← 구현을 헤더에!
    printf("test");
}
// → 여러 .c 파일에서 include 시 중복 정의 에러
```

**올바른 예**:
```c
// sensor.h
void Print(void);  // 선언만

// sensor.c
void Print(void) {  // 구현
    printf("test");
}
```

**예외 - inline 함수**:
```c
// C99+
static inline int Add(int a, int b) {  // inline은 헤더 OK
    return a + b;
}

// C++
inline int Add(int a, int b) {
    return a + b;
}
```

#### 3. Include Guard 중복 이름

**잘못된 예**:
```c
// sensor.h
#ifndef SENSOR_H  // ← 너무 흔한 이름
#define SENSOR_H
// ...
#endif

// 다른 프로젝트의 sensor.h도 SENSOR_H 사용
// → 한쪽이 무시됨!
```

**올바른 예**:
```c
// sensor.h
#ifndef PROJECT_SENSOR_FACTORY_SENSOR_H  // 고유한 이름
#define PROJECT_SENSOR_FACTORY_SENSOR_H
// ...
#endif

// 또는 GUID 사용
#ifndef SENSOR_H_A3F8B2C1_4D9E_5F6A_7B8C_9D0E1F2A3B4C
#define SENSOR_H_A3F8B2C1_4D9E_5F6A_7B8C_9D0E1F2A3B4C
// ...
#endif
```

### 6.2 C++ 특화 주의사항

#### 1. 템플릿 분리 불가

```cpp
// ❌ 안 됨
// Array.hpp
template<typename T>
class Array {
    void resize(size_t n);  // 선언만
};

// Array.cpp
template<typename T>
void Array<T>::resize(size_t n) {  // 구현
    // ...
}
// → 링크 에러!

// ✅ 해결책 1: 헤더에 전부 구현
template<typename T>
class Array {
    void resize(size_t n) {  // 헤더에 구현
        // ...
    }
};

// ✅ 해결책 2: 명시적 인스턴스화 (특정 타입만 지원)
// Array.cpp
template class Array<int>;
template class Array<double>;
```

#### 2. namespace 오남용

```cpp
// ❌ 헤더에서 using namespace
// utils.hpp
#include <iostream>
using namespace std;  // ← 나쁨! 헤더를 include한 모든 파일에 영향

// ✅ 소스 파일에서만 using
// utils.cpp
#include "utils.hpp"
using namespace std;  // OK, 이 파일에만 영향
```

### 6.3 Python 특화 주의사항

#### 1. 순환 import

**문제**:
```python
# sensor.py
from factory import SensorFactory  # factory를 import

class Sensor:
    pass

# factory.py
from sensor import Sensor  # sensor를 import
# → ImportError: cannot import name 'Sensor'
```

**해결책**:
```python
# 방법 1: 지연 import (함수 내부)
# factory.py
def create_sensor():
    from sensor import Sensor  # 함수 내부에서 import
    return Sensor()

# 방법 2: TYPE_CHECKING (타입 힌트용)
# factory.py
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from sensor import Sensor  # 타입 체크 시에만

def create_sensor() -> 'Sensor':  # 문자열로 타입 힌트
    from sensor import Sensor
    return Sensor()
```

#### 2. `__all__` 미사용

```python
# module.py
def public_func():
    pass

def _internal_func():
    pass

SECRET_KEY = "123"

# 다른 파일에서
from module import *  # SECRET_KEY까지 import됨!

# ✅ 해결책: __all__ 정의
__all__ = ['public_func']  # 이것만 export

from module import *  # public_func만 import됨
```

---

## 7. 컴파일 자동화

### 7.1 C - Makefile

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include

# 소스 파일
SRCS = src/sensor_factory.c src/temperature_sensor.c src/main.c
OBJS = $(SRCS:.c=.o)

# 타겟
TARGET = sensor_app

# 규칙
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
```

### 7.2 C++ - CMake

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(SensorFactory)

set(CMAKE_CXX_STANDARD 17)

# 헤더 경로
include_directories(include)

# 소스 파일
set(SOURCES
    src/CallbackManager.cpp
    src/EventDispatcher.cpp
    src/main.cpp
)

# 실행 파일 생성
add_executable(sensor_app ${SOURCES})

# 테스트
enable_testing()
add_test(NAME sensor_test COMMAND sensor_app)
```

### 7.3 Python - setup.py / pyproject.toml

```python
# setup.py
from setuptools import setup, find_packages

setup(
    name="sensor_package",
    version="1.0.0",
    packages=find_packages(),
    install_requires=[
        "numpy>=1.20.0",
    ],
    python_requires=">=3.8",
)
```

```toml
# pyproject.toml (Modern Python)
[build-system]
requires = ["setuptools>=45", "wheel"]
build-backend = "setuptools.build_meta"

[project]
name = "sensor_package"
version = "1.0.0"
requires-python = ">=3.8"
dependencies = [
    "numpy>=1.20.0",
]

[tool.pytest.ini_options]
testpaths = ["tests"]
```

---

## 8. 요약 및 권장 사항

### 8.1 학습 단계별 권장

| 단계 | 권장 구조 | 이유 |
|------|-----------|------|
| **입문** | 단일 파일 | 전체 흐름 파악 용이 |
| **초급** | 헤더 + 소스 분리 | 기본 구조 학습 |
| **중급** | 모듈별 분리 | 실무 구조 이해 |
| **고급** | 라이브러리화 | 재사용 가능한 코드 작성 |

### 8.2 실무 프로젝트 권장 구조

#### C 임베디드

```
project/
├── Inc/           # 헤더
├── Src/           # 소스
├── Drivers/       # HAL
├── Makefile
└── README.md
```

#### C++ 애플리케이션

```
project/
├── include/       # 공개 헤더
├── src/           # 구현
├── examples/      # 예제
├── tests/         # 테스트
├── CMakeLists.txt
└── README.md
```

#### Python 패키지

```
project/
├── package_name/  # 패키지
│   └── __init__.py
├── examples/
├── tests/
├── setup.py
└── README.md
```

### 8.3 핵심 원칙

1. **인터페이스와 구현 분리**
   - C/C++: 헤더에는 선언만, 소스에 구현
   - Python: `__all__`로 공개 API 명시

2. **중복 방지**
   - C/C++: Include Guard 또는 `#pragma once`
   - Python: Import 시스템이 자동 처리

3. **가시성 제어**
   - C: `static` 키워드
   - C++: `namespace`, `private`, anonymous namespace
   - Python: `_` prefix, `__all__`

4. **의존성 최소화**
   - 전방 선언 활용
   - 필요한 것만 include/import

---

## 참고 자료

- C: ISO C11 Standard
- C++: ISO C++17 Standard, [cppreference.com](https://en.cppreference.com)
- Python: [Python.org](https://docs.python.org/3/), PEP 8 Style Guide

---

**작성일**: 2026-02-03
**버전**: 1.0.0
