from ungenetico import *
import random
from dataclasses import dataclass, field


@dataclass
class GeneBool(Gene):
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
    min_val: int = field(init=False, repr=False, default=0)
    max_val: int = field(init=False, repr=False, default=1)
    value: int
    mutation_operator: Mutation

    _name: str = field(init=False, repr=False)
    _min: int = field(init=False, repr=False, default=0)
    _max: int = field(init=False, repr=False, default=1)
    _val: int = field(init=False, repr=False)
    _mutation_operator:  Mutation = field(init=False, repr=False)

    @property
    def value(self):
        return self._val

    @value.setter
    def value(self, val):
        if isinstance(val, property):
            self._val = random.randint(self.min_val, self.max_val)
        else:
            if val < self.min_val:
                val = self.min_val
            elif val > self.max_val:
                val = self.max_val
            self._val = int(round(val))

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



