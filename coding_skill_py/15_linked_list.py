"""Python Linked List - collections.deque 또는 커스텀"""
from collections import deque
from typing import Optional, Generic, TypeVar

T = TypeVar('T')

class Node(Generic[T]):
    def __init__(self, data: T):
        self.data = data
        self.next: Optional[Node[T]] = None

class LinkedList(Generic[T]):
    def __init__(self):
        self.head: Optional[Node[T]] = None
    
    def push(self, data: T):
        node = Node(data)
        node.next = self.head
        self.head = node
    
    def __iter__(self):
        current = self.head
        while current:
            yield current.data
            current = current.next

def main():
    print("=== Python Linked List ===")
    
    # deque 사용 (권장)
    dq = deque([10, 20, 30])
    dq.appendleft(5)
    print(f"deque: {list(dq)}")
    
    # 커스텀 구현
    ll = LinkedList[int]()
    ll.push(10)
    ll.push(20)
    ll.push(5)
    print(f"custom: {list(ll)}")

if __name__ == "__main__":
    main()
