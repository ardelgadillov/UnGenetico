from abc import ABC, abstractmethod
from ungenetico.gene import Gene
from ungenetico.ga import GeneticAlgorithm


class Mutation(ABC):
    """Abstract class"""
    @abstractmethod
    def mutate(self, gen: Gene, ag: GeneticAlgorithm):
        pass
