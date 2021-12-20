from dataclasses import dataclass, field
from typing import List
from ungenetico.individual import Individual


@dataclass
class Population:
    """
    Class Population stores a set of Individuals
    """
    population: List[Individual]
    size: int
    partners: List[int] = field(init=False, repr=False)

    _population: List[Individual] = field(init=False, repr=False)

    @property
    def population(self):
        return self._population

    @population.setter
    def population(self, population):
        if isinstance(population, property):
            self._population = []
        else:
            self._population = population

    @property
    def size(self):
        return len(self.population)

    @size.setter
    def size(self, size):
        pass

    def get_individual(self, pos):
        return self.population[pos]

    def append_individual(self, ind: Individual):
        self.population.append(ind)

    def replace_individual(self, new_ind: Individual, pos):
        self.population[pos] = new_ind

    # def new_generation(self, size):
    #     self.generation = [Individual() for _ in range(size)]

    def calculate_objective_function(self, objective_function):
        # For each individual
        for ind in self.population:
            ind.calculate_objective_function(objective_function)

    def mutate(self, ag):
        for ind in self.population:
            ind.mutate(ag)

    @staticmethod
    def survival_probability(ind: Individual):
        return ind.survival_probability

    def sort_population(self):
        self.population.sort(reverse=True, key=self.survival_probability)


