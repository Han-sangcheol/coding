"""Python Factory - classmethod와 딕셔너리"""
from abc import ABC, abstractmethod
from typing import Dict, Type

class Product(ABC):
    @abstractmethod
    def use(self):
        pass

class ProductA(Product):
    def use(self):
        print("  → ProductA 사용")

class ProductB(Product):
    def use(self):
        print("  → ProductB 사용")

class Factory:
    _products: Dict[str, Type[Product]] = {
        'A': ProductA,
        'B': ProductB
    }
    
    @classmethod
    def create(cls, product_type: str) -> Product:
        if product_type in cls._products:
            return cls._products[product_type]()
        raise ValueError(f"Unknown type: {product_type}")
    
    @classmethod
    def register(cls, name: str, product_class: Type[Product]):
        cls._products[name] = product_class

def main():
    print("\n=== Python Factory Pattern ===")
    
    productA = Factory.create('A')
    productB = Factory.create('B')
    
    productA.use()
    productB.use()

if __name__ == "__main__":
    main()
