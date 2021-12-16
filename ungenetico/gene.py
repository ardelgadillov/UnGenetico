from abc import ABC, abstractmethod
from ungenetico import *


class Gene(ABC):
    """Abstract class that stores the genetic information"""
    @property
    @abstractmethod
    def name(self):
        """Variable name associated with the gen"""
        pass

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

    #
    # @property
    # @abstractmethod
    # def crossover_operator(self):
    #     pass

    def mutate(self, genetic_algorithm):
        """Mutate gen using the mutation operator"""
        self.mutation_operator.mutate(self, genetic_algorithm)

