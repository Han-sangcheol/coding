#!/usr/bin/env python3
"""
Python 디자인 패턴 전체 실행 스크립트
"""

import glob
import subprocess
import sys
from pathlib import Path

def run_all_patterns():
    """모든 패턴 파일을 순차적으로 실행"""
    
    # 00-31번 패턴 파일 찾기
    pattern_files = []
    for i in range(32):
        pattern = f"{i:02d}_*.py"
        files = glob.glob(pattern)
        pattern_files.extend(files)
    
    pattern_files = sorted(set(pattern_files))
    
    if not pattern_files:
        print("패턴 파일을 찾을 수 없습니다.")
        return
    
    print(f"\n찾은 패턴 파일: {len(pattern_files)}개\n")
    
    # 실행 모드 선택
    print("실행 모드:")
    print("1. 전체 실행")
    print("2. 특정 패턴만")
    choice = input("선택 (1-2): ").strip()
    
    if choice == "2":
        print(f"\n사용 가능한 패턴:")
        for i, file in enumerate(pattern_files):
            print(f"  {i}: {file}")
        idx = int(input("\n패턴 번호: "))
        if 0 <= idx < len(pattern_files):
            pattern_files = [pattern_files[idx]]
    
    # 실행
    success = 0
    failed = 0
    
    for pattern in pattern_files:
        print("\n" + "=" * 60)
        print(f"Running: {pattern}")
        print("=" * 60)
        
        try:
            result = subprocess.run(
                [sys.executable, pattern],
                check=True,
                capture_output=False,
                text=True
            )
            success += 1
        except subprocess.CalledProcessError as e:
            print(f"[ERROR] {pattern} 실행 실패")
            failed += 1
        except KeyboardInterrupt:
            print("\n\n중단됨")
            break
    
    # 결과 요약
    print("\n" + "=" * 60)
    print("실행 완료")
    print("=" * 60)
    print(f"성공: {success}개")
    print(f"실패: {failed}개")
    print()

if __name__ == "__main__":
    run_all_patterns()
