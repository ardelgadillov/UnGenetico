from dataclasses import dataclass, field
import copy
import types
from typing import List
from ungenetico.gene import Gene
from ungenetico.individual import Individual
from ungenetico.population import Population


@dataclass
class GeneticAlgorithm:
    objective_function: types.FunctionType
    optimization: str
    actual_generation: Population
    generation: int
    generation_size: int
    generation_max: int
    generation_best: int
    objective_mean: float
    objective_median: float
    objective_std: float
    objective_online: float
    objective_offline: float

    _genome_base: List[Gene] = field(init=False, repr=False)

    def __post_init__(self):
        self._genome_base = []

    _objective_function: types.FunctionType = field(init=False, repr=False)
    _optimization: str = field(init=False, repr=False)
    _actual_generation: Population = field(init=False, repr=False)
    _generation: int = field(init=False, repr=False)
    _generation_size: int = field(init=False, repr=False)
    _generation_max: int = field(init=False, repr=False)
    _generation_best: int = field(init=False, repr=False)
    _objective_mean: float = field(init=False, repr=False)
    _objective_median: float = field(init=False, repr=False)
    _objective_std: float = field(init=False, repr=False)
    _objective_online: float = field(init=False, repr=False)
    _objective_offline: float = field(init=False, repr=False)

    @property
    def objective_function(self):
        """Value of the objective function for the individual"""
        return self._objective_function

    @objective_function.setter
    def objective_function(self, objective_function):
        if not isinstance(objective_function, property):
            self._objective_function = objective_function

    @property
    def optimization(self):
        """Value of the objective function for the individual"""
        return self._optimization

    @optimization.setter
    def optimization(self, optimization):
        if isinstance(optimization, property):
            self._optimization = 'maximization'
        else:
            if optimization != 'maximization' or optimization != 'minimization':
                self._optimization = 'maximization'
            else:
                self._optimization = optimization

    @property
    def actual_generation(self):
        """Value of the objective function for the individual"""
        return self._actual_generation

    @actual_generation.setter
    def actual_generation(self, actual_generation):
        if isinstance(actual_generation, property):
            self._actual_generation = Population()
        else:
            self._actual_generation = actual_generation

    @property
    def generation(self):
        """Value of the objective function for the individual"""
        return self._generation

    @generation.setter
    def generation(self, generation):
        if isinstance(generation, property):
            self._generation = 1
        else:
            self._generation = generation

    @property
    def generation_size(self):
        """Value of the objective function for the individual"""
        return self._generation_size

    @generation_size.setter
    def generation_size(self, generation_size):
        if isinstance(generation_size, property):
            self._generation_size = 100
        else:
            self._generation_size = generation_size

    @property
    def generation_max(self):
        """Value of the objective function for the individual"""
        return self._generation_max

    @generation_max.setter
    def generation_max(self, generation_max):
        if isinstance(generation_max, property):
            self._generation_max = 1000
        else:
            self._generation_max = generation_max

    @property
    def generation_best(self):
        """Value of the objective function for the individual"""
        return self._generation_best

    @generation_best.setter
    def generation_best(self, generation_best):
        if isinstance(generation_best, property):
            self._generation_best = 0
        else:
            self._generation_best = generation_best

    @property
    def objective_mean(self):
        """Value of the objective function for the individual"""
        return self._objective_mean

    @objective_mean.setter
    def objective_mean(self, objective_mean):
        if isinstance(objective_mean, property):
            self._objective_mean = 0
        else:
            self._objective_mean = objective_mean

    @property
    def objective_median(self):
        """Value of the objective function for the individual"""
        return self._objective_median

    @objective_median.setter
    def objective_median(self, objective_median):
        if isinstance(objective_median, property):
            self._objective_median = 0
        else:
            self._objective_median = objective_median

    @property
    def objective_std(self):
        """Value of the objective function for the individual"""
        return self._objective_std

    @objective_std.setter
    def objective_std(self, objective_std):
        if isinstance(objective_std, property):
            self._objective_std = 0
        else:
            self._objective_std = objective_std

    @property
    def objective_online(self):
        """Value of the objective function for the individual"""
        return self._objective_online

    @objective_online.setter
    def objective_online(self, objective_online):
        if isinstance(objective_online, property):
            self._objective_online = 0
        else:
            self._objective_online = objective_online

    @property
    def objective_offline(self):
        """Value of the objective function for the individual"""
        return self._objective_offline

    @objective_offline.setter
    def objective_offline(self, objective_offline):
        if isinstance(objective_offline, property):
            self._objective_offline = 0
        else:
            self._objective_offline = objective_offline

    def add_gen(self, gen: Gene):
        self._genome_base.append(gen)

    def create_population(self, size):
        self.generation_size = size
        ind = Individual()
        ind.genome = copy.deepcopy(self._genome_base)
        for _ in range(self.generation_size):
            self.actual_generation.append_individual(copy.deepcopy(ind))

    def mutate(self):
        self.actual_generation.mutate(self)

    def calculate_objective_function(self):
        self.actual_generation.calculate_objective_function(self.objective_function)
