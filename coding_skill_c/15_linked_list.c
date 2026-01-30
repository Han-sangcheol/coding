/*
 * ============================================================================
 * 파일명: 15_linked_list.c
 * 기능: C언어 Linked List(연결 리스트) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Linked List 패턴이란?
 * ============================================================================
 * 
 * 노드들을 포인터로 연결하여 동적 크기 리스트를 구현하는 자료구조
 * 
 * 핵심:
 *   - 동적 크기
 *   - O(1) 삽입/삭제 (위치 알 때)
 *   - 순차 접근
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Linked List 활용
 * ============================================================================
 * 
 * - 태스크 큐
 * - 이벤트 리스트
 * - 타이머 리스트
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 노드 구조체 */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/* Linked List 구조체 */
typedef struct {
    Node* head;
    int count;
} LinkedList;

/* 리스트 초기화 */
void List_Init(LinkedList* list) {
    list->head = NULL;
    list->count = 0;
    printf("[LinkedList] 초기화\n");
}

/* 노드 추가 (맨 앞에) */
void List_Push(LinkedList* list, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->count++;
    
    printf("[LinkedList] Push: %d (count=%d)\n", data, list->count);
}

/* 노드 제거 (맨 앞에서) */
int List_Pop(LinkedList* list, int* data) {
    if (list->head == NULL) {
        printf("[LinkedList] Pop 실패: 리스트 비어있음\n");
        return -1;
    }
    
    Node* node = list->head;
    *data = node->data;
    list->head = node->next;
    free(node);
    list->count--;
    
    printf("[LinkedList] Pop: %d (count=%d)\n", *data, list->count);
    return 0;
}

/* 리스트 출력 */
void List_Print(LinkedList* list) {
    printf("[LinkedList] 내용: ");
    Node* current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("(count=%d)\n", list->count);
}

/* 리스트 정리 */
void List_Clear(LinkedList* list) {
    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->count = 0;
    printf("[LinkedList] 정리 완료\n");
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Linked List 패턴 예제\n");
    printf("========================================\n\n");
    
    LinkedList list;
    List_Init(&list);
    
    // 데이터 추가
    printf("\n=== 데이터 추가 ===\n");
    for (int i = 1; i <= 5; i++) {
        List_Push(&list, i * 10);
    }
    List_Print(&list);
    
    // 데이터 제거
    printf("\n=== 데이터 제거 ===\n");
    int data;
    List_Pop(&list, &data);
    List_Print(&list);
    
    // 정리
    printf("\n=== 리스트 정리 ===\n");
    List_Clear(&list);
    
    printf("\n========================================\n");
    printf("Linked List 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Linked List의 장점
 * ============================================================================
 * 
 * 1. 동적 크기
 * 2. O(1) 삽입/삭제 (위치 알 때)
 * 3. 메모리 효율적 (필요한 만큼만)
 * 
 * 단점:
 * - 순차 접근만 가능
 * - 포인터 오버헤드
 * - 캐시 미스 증가
 * 
 * ============================================================================
 */
