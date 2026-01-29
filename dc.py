"""
BLDC 모터 시뮬레이션 및 자동 파라미터 최적화
===========================================
기능:
  1. BLDC 모터 물리 모델 시뮬레이션 (dq 좌표계 기반)
  2. FOC (Field Oriented Control) 제어 시뮬레이션
  3. 정상상태 분석으로 필요 전압/파라미터 자동 계산
  4. scipy.optimize를 이용한 PID 게인 자동 튜닝
  5. 3상 전류 및 토크 리플 시각화

BLDC vs DC 모터 차이점:
  - DC: 브러시+정류자로 기계적 정류
  - BLDC: 전자 정류 (6-step 또는 FOC)
  - BLDC: 3상 구동 (A, B, C)
  - BLDC: 사다리꼴/사인파 역기전력
"""
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from scipy.optimize import minimize, differential_evolution

# =============================================================================
# 1. BLDC 모터 파라미터 (예시)
# =============================================================================
Rs = 0.5              # 상 저항 (Phase Resistance) [Ω]
Ls = 0.001            # 상 인덕턴스 (Phase Inductance) [H]
J = 0.001             # 관성 모멘트 [kg·m²]
B = 0.001             # 마찰 계수 [N·m·s/rad]
Kt = 0.1              # 토크 상수 [N·m/A]
Ke = 0.1              # 역기전력 상수 [V·s/rad]
P = 1                 # 극 쌍 수 (Pole Pairs)

# 등가 파라미터 (dq 좌표계용)
R = Rs                # dq 좌표계 저항
L = Ls                # dq 좌표계 인덕턴스

# =============================================================================
# 2. BLDC Plant 모델 (dq 좌표계 - FOC 제어 기반)
# =============================================================================
def motor_dynamics(t, x, Vq, params):
    """
    BLDC 모터 동역학 모델 (dq 좌표계 - q축만 사용하는 단순화 모델)
    
    dq 모델 방정식:
      Vq = Rs*iq + Ls*diq/dt + Ke*omega_e
      Te = (3/2)*P*Kt*iq
      J*domega_m/dt = Te - B*omega_m - TL
      
    여기서:
      omega_e = P * omega_m (전기각속도 = 극쌍수 × 기계각속도)
      iq: q축 전류 (토크 생성)
      id: d축 전류 (자속 약화, 여기서는 0으로 제어)
    """
    Rs, Ls, J, B, Kt, Ke, P = params
    iq, omega_m = x  # q축 전류, 기계 각속도
    
    omega_e = P * omega_m  # 전기 각속도
    
    # q축 전류 동역학
    diq_dt = (Vq - Rs*iq - Ke*omega_e) / Ls
    
    # 토크 및 기계 동역학
    Te = 1.5 * P * Kt * iq  # BLDC 토크 (3상 기준)
    domega_m_dt = (Te - B*omega_m) / J
    
    return [diq_dt, domega_m_dt]

def get_phase_currents(iq, theta_e):
    """
    dq 전류를 3상 전류로 역변환 (Park 역변환)
    id = 0 가정 (토크 최대화 제어)
    """
    ia = iq * np.cos(theta_e - np.pi/2)
    ib = iq * np.cos(theta_e - np.pi/2 - 2*np.pi/3)
    ic = iq * np.cos(theta_e - np.pi/2 + 2*np.pi/3)
    return ia, ib, ic

# =============================================================================
# 3. 시뮬레이션 함수
# =============================================================================
def simulate_motor(kp, ki, target_speed, V_max, params, t_end=0.5, fs=10000):
    """BLDC 모터 시뮬레이션 실행 (FOC 제어)"""
    dt = 1/fs
    t_eval = np.arange(0, t_end, dt)
    
    Rs, Ls, J, B, Kt, Ke, P = params
    
    x = [0.0, 0.0]  # [iq (q축 전류), omega_m (기계 각속도)]
    history = []
    integral_error = 0.0
    theta_e = 0.0  # 전기 각도 (3상 전류 계산용)
    
    for t in t_eval:
        # Controller (PI 속도 제어 -> Vq 출력)
        error = target_speed - x[1]
        integral_error += error * dt
        Vq_out = kp * error + ki * integral_error
        Vq_out = np.clip(Vq_out, -V_max, V_max)
        
        # Plant (물리 시뮬레이션)
        sol = solve_ivp(motor_dynamics, [t, t+dt], x, 
                        args=(Vq_out, params), method='RK45')
        x = sol.y[:, -1]
        
        # 전기 각도 업데이트
        theta_e += P * x[1] * dt
        theta_e = theta_e % (2*np.pi)
        
        # 3상 전류 계산
        ia, ib, ic = get_phase_currents(x[0], theta_e)
        
        # 토크 계산
        Te = 1.5 * P * Kt * x[0]
        
        history.append([t, x[0], x[1], Vq_out, ia, ib, ic, Te, theta_e])
    
    return np.array(history)

# =============================================================================
# 4. 정상상태 분석 (물리적 한계 계산)
# =============================================================================
def analyze_steady_state(target_speed, params, V_max):
    """BLDC 정상상태에서 필요한 전압과 달성 가능한 최대 속도 계산"""
    Rs, Ls, J, B, Kt, Ke, P = params
    
    # 정상상태 (dq 좌표계):
    # Te = 1.5*P*Kt*iq = B*omega_m  =>  iq = B*omega_m / (1.5*P*Kt)
    # Vq = Rs*iq + Ke*P*omega_m
    
    iq_steady = B * target_speed / (1.5 * P * Kt)
    V_per_omega = Rs * B / (1.5 * P * Kt) + Ke * P
    
    V_required = V_per_omega * target_speed
    omega_max = V_max / V_per_omega
    Te_steady = 1.5 * P * Kt * iq_steady
    
    return {
        'V_required': V_required,
        'omega_max': omega_max,
        'iq_steady': iq_steady,
        'Te_steady': Te_steady,
        'V_per_omega': V_per_omega,
        'achievable': V_required <= V_max
    }

# =============================================================================
# 5. 자동 파라미터 탐색 (최적화)
# =============================================================================
# 전역 카운터 (진행 상황 표시용)
eval_counter = [0]

def cost_function(gains, target_speed, V_max, params):
    """최적화 비용 함수: 정착시간 + 오버슈트 + 정상상태 오차"""
    kp, ki = gains
    eval_counter[0] += 1
    
    if kp <= 0 or ki <= 0:
        return 1e6
    
    try:
        # 최적화 시에는 낮은 샘플링 주파수로 빠르게 계산
        history = simulate_motor(kp, ki, target_speed, V_max, params, t_end=0.3, fs=1000)
        speed = history[:, 2]
        
        # 성능 지표 계산
        steady_state_error = abs(target_speed - np.mean(speed[-50:]))
        overshoot = max(0, (np.max(speed) - target_speed) / target_speed * 100)
        
        # 정착시간 (2% 이내)
        tolerance = 0.02 * target_speed
        settled = np.abs(speed - target_speed) < tolerance
        if np.any(settled):
            settling_idx = np.where(settled)[0]
            if len(settling_idx) > 0:
                settling_time = history[settling_idx[0], 0]
            else:
                settling_time = 0.3
        else:
            settling_time = 0.3
        
        # 비용: 정상상태 오차 + 오버슈트 + 정착시간
        cost = steady_state_error * 10 + overshoot * 0.5 + settling_time * 100
        
        # 매 10회마다 진행 상황 출력
        if eval_counter[0] % 10 == 0:
            print(f"    Eval #{eval_counter[0]:4d}: Kp={kp:.3f}, Ki={ki:.1f}, Cost={cost:.1f}")
        
        return cost
    except:
        return 1e6

def auto_tune_pid(target_speed, V_max, params):
    """PID 게인 자동 튜닝 (Differential Evolution)"""
    print("  Searching for optimal gains...")
    
    bounds = [(0.01, 10), (1, 500)]  # kp, ki 범위
    iteration_count = [0]  # 리스트로 감싸서 클로저에서 수정 가능하게
    
    def callback(xk, convergence):
        iteration_count[0] += 1
        kp, ki = xk
        cost = cost_function(xk, target_speed, V_max, params)
        print(f"    Iter {iteration_count[0]:3d}: Kp={kp:.4f}, Ki={ki:.4f}, Cost={cost:.2f}, Conv={convergence:.4f}")
        return False  # False를 반환하면 계속 진행
    
    result = differential_evolution(
        cost_function, bounds,
        args=(target_speed, V_max, params),
        maxiter=50, popsize=10, seed=42, workers=1,
        disp=False, callback=callback
    )
    
    return result.x[0], result.x[1], result.fun

def find_optimal_voltage(target_speed, params):
    """목표 속도 달성에 필요한 최소 전압 탐색"""
    analysis = analyze_steady_state(target_speed, params, 1000)
    return analysis['V_required'] * 1.2  # 20% 여유

# =============================================================================
# 6. 메인 실행
# =============================================================================
if __name__ == "__main__":
    # BLDC 파라미터 튜플
    params = (Rs, Ls, J, B, Kt, Ke, P)
    target_speed = 300.0  # BLDC는 일반적으로 고속
    V_max_original = 24.0
    
    print("=" * 60)
    print("BLDC Motor Simulation - FOC Control & Auto Optimization")
    print("=" * 60)
    print(f"  Pole Pairs: {P}")
    print(f"  Rs={Rs} Ohm, Ls={Ls*1000:.1f} mH")
    print(f"  Kt={Kt} N.m/A, Ke={Ke} V.s/rad")
    
    # Step 1: 정상상태 분석
    print("\n[Step 1] Steady-State Analysis")
    analysis = analyze_steady_state(target_speed, params, V_max_original)
    print(f"  Target Speed: {target_speed} rad/s ({target_speed*60/(2*np.pi):.0f} RPM)")
    print(f"  Current V_max: {V_max_original} V")
    print(f"  Required Vq: {analysis['V_required']:.2f} V")
    print(f"  Achievable Max Speed: {analysis['omega_max']:.2f} rad/s")
    print(f"  Steady-State iq: {analysis['iq_steady']:.3f} A")
    print(f"  Target Achievable: {'YES' if analysis['achievable'] else 'NO'}")
    
    # Step 2: 필요시 전압 조정
    if not analysis['achievable']:
        V_max_new = find_optimal_voltage(target_speed, params)
        print(f"\n[Step 2] Voltage Auto-Adjust: {V_max_original}V -> {V_max_new:.1f}V")
        V_max = V_max_new
    else:
        V_max = V_max_original
        print("\n[Step 2] Voltage OK - No adjustment needed")
    
    # Step 3: PID 자동 튜닝
    print("\n[Step 3] PID Gain Auto-Tuning (Speed Loop)")
    kp_opt, ki_opt, cost = auto_tune_pid(target_speed, V_max, params)
    print(f"  Optimal Kp: {kp_opt:.4f}")
    print(f"  Optimal Ki: {ki_opt:.4f}")
    print(f"  Cost Function: {cost:.4f}")
    
    # Step 4: 최적 파라미터로 시뮬레이션
    print("\n[Step 4] Running Simulation with Optimal Parameters")
    history = simulate_motor(kp_opt, ki_opt, target_speed, V_max, params)
    
    # 성능 지표 계산
    speed = history[:, 2]
    steady_state = np.mean(speed[-100:])
    overshoot = (np.max(speed) - target_speed) / target_speed * 100
    ss_error = abs(target_speed - steady_state) / target_speed * 100
    
    print(f"\n[Result] Performance Metrics")
    print(f"  Steady-State Speed: {steady_state:.2f} rad/s ({steady_state*60/(2*np.pi):.0f} RPM)")
    print(f"  Steady-State Error: {ss_error:.2f}%")
    print(f"  Overshoot: {max(0, overshoot):.2f}%")
    
    # Step 5: 결과 시각화 (BLDC 전용)
    fig, axes = plt.subplots(4, 1, figsize=(12, 10))
    
    # 속도 응답
    ax1 = axes[0]
    ax1.plot(history[:, 0], history[:, 2], 'b-', linewidth=2, label='Speed [rad/s]')
    ax1.axhline(target_speed, color='r', linestyle='--', linewidth=2, label='Target')
    ax1.fill_between(history[:, 0], target_speed*0.98, target_speed*1.02, 
                     alpha=0.2, color='green', label='+-2% Band')
    ax1.set_ylabel('Speed [rad/s]')
    ax1.legend(loc='lower right')
    ax1.set_title(f'BLDC Motor FOC Control - Kp={kp_opt:.3f}, Ki={ki_opt:.3f}, V_max={V_max:.1f}V')
    ax1.grid(True, alpha=0.3)
    
    # 3상 전류
    ax2 = axes[1]
    ax2.plot(history[:, 0], history[:, 4], 'r-', linewidth=1, alpha=0.8, label='Ia')
    ax2.plot(history[:, 0], history[:, 5], 'g-', linewidth=1, alpha=0.8, label='Ib')
    ax2.plot(history[:, 0], history[:, 6], 'b-', linewidth=1, alpha=0.8, label='Ic')
    ax2.set_ylabel('Phase Current [A]')
    ax2.legend(loc='upper right')
    ax2.set_title('3-Phase Currents (ABC)')
    ax2.grid(True, alpha=0.3)
    
    # q축 전류 및 토크
    ax3 = axes[2]
    ax3_twin = ax3.twinx()
    line1, = ax3.plot(history[:, 0], history[:, 1], 'b-', linewidth=2, label='iq [A]')
    line2, = ax3_twin.plot(history[:, 0], history[:, 7], 'orange', linewidth=2, label='Torque [N.m]')
    ax3.set_ylabel('q-axis Current [A]', color='b')
    ax3_twin.set_ylabel('Torque [N.m]', color='orange')
    ax3.legend(handles=[line1, line2], loc='lower right')
    ax3.grid(True, alpha=0.3)
    
    # Vq 전압
    ax4 = axes[3]
    ax4.plot(history[:, 0], history[:, 3], 'g-', linewidth=2, label='Vq [V]')
    ax4.axhline(V_max, color='r', linestyle='--', alpha=0.5, label=f'V_max={V_max:.1f}V')
    ax4.axhline(-V_max, color='r', linestyle='--', alpha=0.5)
    ax4.set_xlabel('Time [s]')
    ax4.set_ylabel('Voltage [V]')
    ax4.legend(loc='lower right')
    ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()