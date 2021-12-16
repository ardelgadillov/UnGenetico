from ungenetico import *
import random
from dataclasses import dataclass, field


@dataclass
class GeneFloat(Gene):
    """
    Abstract Class Gen

    Class Gen stores the genetic information

    Attributes
    ----------
    name: str
        Variable name associated with the gen
    value:
        Value of the gen
    min_value

    """

    name: str
    min_val: float
    max_val: float
    value: float
    mutation_operator: Mutation

    _name: str = field(init=False, repr=False)
    _min: float = field(init=False, repr=False)
    _max: float = field(init=False, repr=False)
    _val: float = field(init=False, repr=False)
    _mutation_operator:  Mutation = field(init=False, repr=False)

    # def __post_init__(self):
    #     if hasattr(self, 'name'):
    #         print(self.name)
    #     print(self)
    #     # no value was passed
    #     if isinstance(self.mutation_operator, property):
    #         self.mutation_operator = MutationOperatorIntUniform()

        # # no value was passed
        # if isinstance(self.value, property):
        #     self.value = -99

    @property
    def value(self):
        return self._val

    @value.setter
    def value(self, val):
        if isinstance(val, property):
            self._val = random.uniform(self.min_val, self.max_val)
        else:
            if val < self.min_val:
                val = self.min_val
            elif val > self.max_val:
                val = self.max_val
            self._val = float(val)

    @property
    def min_val(self):
        return self._min

    @min_val.setter
    def min_val(self, min_val):
        self._min = float(min_val)

    @property
    def max_val(self):
        return self._max

    @max_val.setter
    def max_val(self, max_val):
        self._max = float(max_val)

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
    def mutation_operator(self):
        return self._mutation_operator

    @mutation_operator.setter
    def mutation_operator(self, mo: Mutation):
        if isinstance(mo, property):
            self.mutation_operator = MutationUniform()
        else:
            self._mutation_operator = mo

    #
    # @property
    # def crossover_operator(self):
    #     print('crossover')
    #     return self._crossover_operator
    #
    # @crossover_operator.setter
    # def crossover_operator(self, value):
    #     self._crossover_operator = value



