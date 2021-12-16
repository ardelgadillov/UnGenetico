from dataclasses import dataclass, field
from typing import List
from ungenetico.individual import Individual


@dataclass
class Population:
    """
    Class Population stores a set of Individuals
    """
    generation: List[Individual]

    _generation: List[Individual] = field(init=False, repr=False)

    @property
    def generation(self):
        return self._generation

    @generation.setter
    def generation(self, generation):
        if isinstance(generation, property):
            self._generation = []
        else:
            self._generation = generation

    def get_individual(self, pos):
        return self.generation[pos]

    def append_individual(self, ind: Individual):
        self.generation.append(ind)

    def replace_individual(self, new_ind: Individual, pos):
        self.generation[pos] = new_ind

    # def new_generation(self, size):
    #     self.generation = [Individual() for _ in range(size)]

    def calculate_objective_function(self, objective_function):
        # For each individual
        for ind in self.generation:
            ind.calculate_objective_function(objective_function)

    def mutate(self, ag):
        for ind in self.generation:
            ind.mutate(ag)
