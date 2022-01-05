from ungenetico import Gene, Mutation, MutationUniform
import random
from dataclasses import dataclass, field
from typing import List


@dataclass
class GeneInt(Gene):
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
    min_val: int
    max_val: int
    length: int
    value: List[int]
    mutation_operator: Mutation

    _min: int = field(init=False, repr=False)
    _max: int = field(init=False, repr=False)
    _val: List[int] = field(init=False, repr=False)
    _mutation_operator:  Mutation = field(init=False, repr=False)

    @property
    def value(self):
        return self._val

    @value.setter
    def value(self, val):
        if isinstance(val, property):
            self._val = [random.randint(self.min_val, self.max_val) for _ in range(self.length)]
        else:
            for i in range(self.length):
                if val[i] < self.min_val:
                    val[i] = self.min_val
                elif val[i] > self.max_val:
                    val[i] = self.max_val
                self._val[i] = int(round(val[i]))

    @property
    def min_val(self):
        return self._min

    @min_val.setter
    def min_val(self, min_val):
        self._min = int(round(min_val))

    @property
    def max_val(self):
        return self._max

    @max_val.setter
    def max_val(self, max_val):
        self._max = int(round(max_val))

    @property
    def mutation_operator(self):
        return self._mutation_operator

    @mutation_operator.setter
    def mutation_operator(self, mo: Mutation):
        if isinstance(mo, property):
            self._mutation_operator = MutationUniform()
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



