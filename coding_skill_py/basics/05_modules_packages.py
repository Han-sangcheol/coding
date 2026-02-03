"""
============================================================================
파일명: 05_modules_packages.py
주제: Python 모듈과 패키지
실행: python 05_modules_packages.py
============================================================================
"""

def example_01_import():
    """예제 1: import"""
    print("\n=== 예제 1: import ===\n")
    
    # C 방식: #include
    # #include <stdio.h>
    # #include "mylib.h"
    
    # Python 방식: import
    import math
    print(f"원주율: {math.pi}")
    print(f"제곱근: {math.sqrt(16)}")
    
    # from ... import
    from math import sqrt, pi
    print(f"pi: {pi}")
    
    # as로 별명
    import math as m
    print(f"sin(90): {m.sin(m.pi/2)}")

def example_02_module_structure():
    """예제 2: 모듈 구조"""
    print("\n=== 예제 2: 모듈 구조 ===\n")
    
    print("""
    [파일 구조]
    myproject/
    ├── main.py
    ├── utils.py          # 모듈
    └── mypackage/        # 패키지
        ├── __init__.py
        ├── module1.py
        └── module2.py
    
    [사용]
    import utils
    from mypackage import module1
    """)

def example_03_if_name_main():
    """예제 3: if __name__ == '__main__'"""
    print("\n=== 예제 3: if __name__ == '__main__' ===\n")
    
    print("""
    # mymodule.py
    def my_function():
        print("함수 실행")
    
    if __name__ == "__main__":
        # 이 파일을 직접 실행할 때만 실행됨
        my_function()
    
    # 다른 파일에서 import할 때는 실행 안 됨
    """)

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식]")
    print("""
    // mylib.h
    #ifndef MYLIB_H
    #define MYLIB_H
    void my_function();
    #endif
    
    // mylib.c
    #include "mylib.h"
    void my_function() { ... }
    
    // main.c
    #include "mylib.h"
    """)
    
    print("\n[Python 방식]")
    print("""
    # mymodule.py
    def my_function():
        pass
    
    # main.py
    import mymodule
    mymodule.my_function()
    """)

def main():
    print("=" * 40)
    print("Python 기초: 05. 모듈과 패키지")
    print("=" * 40)
    
    example_01_import()
    example_02_module_structure()
    example_03_if_name_main()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 06_file_io.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

import: import math
from: from math import sqrt
as: import math as m
__name__: if __name__ == "__main__":
패키지: __init__.py

============================================================================
"""
