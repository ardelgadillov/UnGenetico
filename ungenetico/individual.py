from dataclasses import dataclass, field
import inspect
from typing import List
from ungenetico.gene import Gene


@dataclass
class Individual:
    """Class that represents an individual of the Genetic Algorithm"""
    genome: List[Gene]
    survival_probability: float
    objective_value: float
    paired: bool

    _genome: List[Gene] = field(init=False, repr=False)
    _survival_probability: float = field(init=False, repr=False)
    _objective_value: float = field(init=False, repr=False)
    _paired: bool = field(init=False, repr=False)

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

    @property
    def paired(self):
        """Value of the objective function for the individual"""
        return self._paired

    @paired.setter
    def paired(self, paired):
        if isinstance(paired, property):
            self._paired = False
        else:
            self._paired = paired

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
