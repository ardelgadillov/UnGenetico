from abc import ABC, abstractmethod
import copy
import inspect


class Gene(ABC):
    """Abstract class that stores the genetic information"""
    @property
    @abstractmethod
    def name(self):
        """Variable name associated with the gen"""
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

    @property
    @abstractmethod
    def value(self):
        """Value of the gen"""
        pass

    # @value.setter
    # @abstractmethod
    # def value(self, val):
    #     pass

    def mutate(self):
        """Mutate gen using the mutation operator"""
        self.mutation_operator.mutate(self)


class Individual:
    """Class that represents an individual of the Genetic Algorithm"""
    def __init__(self):
        """constructor"""
        self._genome = []
        self._probability = 0
        self._objective = 0

    @property
    def genome(self):
        """List of genes"""
        return self._genome

    @genome.setter
    def genome(self, genome):
        self._genome = genome

    @property
    def objective(self):
        """Value of the objective function for the individual"""
        return self._objective

    @objective.setter
    def objective(self, objective):
        self._objective = objective

    @property
    def probability(self):
        """Value of the survival probability for the individual"""
        return self._probability

    @probability.setter
    def probability(self, value):
        if value < 0:
            value = 0
        elif value > 1:
            value = 1
        self._probability = value

    def mutate(self):
        """Mutate the genes in genome"""
        for gen in self.genome:
            gen.mutate()


class Population:
    """
    Class Population stores a set of Individuals
    """
    def __init__(self):
        self._generation = []

    @property
    def generation(self):
        return self._generation

    @generation.setter
    def generation(self, generation):
        self._generation = generation

    def get_individual(self, pos):
        return self._generation[pos]

    def append_individual(self, ind: Individual):
        self._generation.append(ind)

    def replace_individual(self, new_ind: Individual, pos):
        self._generation[pos] = new_ind

    def new_generation(self, size):
        self._generation = [Individual() for _ in range(size)]

    def evaluate_objective_function(self, objective_function):
        for ind in self.generation:
            names = [gen.name for gen in ind.genome]
            values = [gen.value for gen in ind.genome]
            variables = inspect.getfullargspec(objective_function).args
            pos = [names.index(i) for i in variables]
            values_list = [values[i] for i in pos]
            # calculate objective function for individual
            ind.objective = objective_function(*values_list)

    def mutate(self):
        for ind in self.generation:
            ind.mutate()


class GeneticAlgorithm:
    def __init__(self):
        self._genome_base = []
        self._size = 100
        self._population = Population()
        self._objective_function = None

    def add_gen(self, gen: Gene):
        self._genome_base.append(gen)

    def create_population(self, size):
        self._size = size
        ind = Individual()
        ind.genome = copy.deepcopy(self._genome_base)
        for _ in range(self._size):
            self._population.append_individual(copy.deepcopy(ind))

    def mutate(self):
        self._population.mutate()

    def evaluate_objective_function(self):
        self._population.evaluate_objective_function(self.objective_function)

    @property
    def objective_function(self):
        return self._objective_function

    @objective_function.setter
    def objective_function(self, objective_function):
        self._objective_function = objective_function


class MutationOperator(ABC):
    """Abstract class"""
    @abstractmethod
    def mutate(self, gen: Gene):
        pass

