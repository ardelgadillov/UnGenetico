from abc import ABC, abstractmethod
from dataclasses import dataclass, field
# from typing import List, Any
# from ungenetico import *


@dataclass
class Gene(ABC):
    _name: str = field(init=False, repr=False)
    _length: int = field(init=False, repr=False)

    """Abstract class that stores the genetic information"""
    @property
    def name(self):
        """
        name
        """
        return self._name

    @name.setter
    def name(self, name):
        # it only set the name the first time
        if not hasattr(self, 'name'):
            self._name = name

    @property
    def length(self):
        return self._length

    @length.setter
    def length(self, length):
        if isinstance(length, property):
            self._length = 1
        else:
            self._length = int(length)

    @property
    @abstractmethod
    def value(self):
        """Value of the gen"""
        pass

    @value.setter
    def value(self, value):
        pass

    @property
    @abstractmethod
    def min_val(self):
        """Minimum value of gen"""
        pass

    @property
    @abstractmethod
    def max_val(self):
        """Maximum value of gen"""
        pass

    @property
    @abstractmethod
    def mutation_operator(self):
        """Mutation operator"""
        pass

    @property
    @abstractmethod
    def crossover_operator(self):
        pass

    def mutate(self, genetic_algorithm):
        """Mutate gen using the mutation operator"""
        self.mutation_operator.mutate(self, genetic_algorithm)

    def exchange(self, gene2, genetic_algorithm):
        """Exchange genes"""
        children = self.crossover_operator.exchange(self, gene2, genetic_algorithm)
        return children
