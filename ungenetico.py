from abc import ABC, abstractmethod
from dataclasses import dataclass, field
import copy
import inspect
import types


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

    def mutate(self, ag):
        """Mutate gen using the mutation operator"""
        self.mutation_operator.mutate(self, ag)


@dataclass
class Individual:
    """Class that represents an individual of the Genetic Algorithm"""
    genome: list[Gene]
    survival_probability: float
    objective_value: float

    _genome: list[Gene] = field(init=False, repr=False)
    _survival_probability: float = field(init=False, repr=False)
    _objective_value: float = field(init=False, repr=False)

    @property
    def genome(self):
        """List of genes"""
        return self._genome

    @genome.setter
    def genome(self, genome):
        self._genome = genome

    @property
    def objective_value(self):
        """Value of the objective function for the individual"""
        return self._objective_value

    @objective_value.setter
    def objective_value(self, objective):
        if isinstance(objective, property):
            self._objective_value = 0
        else:
            self._objective_value = objective

    @property
    def survival_probability(self):
        """Value of the survival probability for the individual"""
        return self._survival_probability

    @survival_probability.setter
    def survival_probability(self, value):
        if isinstance(value, property):
            self._survival_probability = 0
        else:
            if value < 0:
                value = 0
            elif value > 1:
                value = 1
            self._survival_probability = value

    def mutate(self, ag):
        """Mutate the genes in genome"""
        for gen in self.genome:
            gen.mutate(ag)

    def calculate_objective_function(self, objective_function):
        # get name of the genes
        names = [gen.name for gen in self.genome]
        # get values for each gen
        values = [gen.value for gen in self.genome]
        # get variables for objective_function
        variables = inspect.getfullargspec(objective_function).args
        # get position of names in variables
        pos = [names.index(i) for i in variables]
        # sort values corresponding with variables
        values_list = [values[i] for i in pos]
        # calculate objective function for individual
        self.objective_value = objective_function(*values_list)


@dataclass
class Population:
    """
    Class Population stores a set of Individuals
    """
    generation: list[Individual]

    _generation: list[Individual] = field(init=False, repr=False)

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

    _genome_base: list[Gene] = field(init=False, repr=False)

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


class MutationOperator(ABC):
    """Abstract class"""
    @abstractmethod
    def mutate(self, gen: Gene, ag: GeneticAlgorithm):
        pass
