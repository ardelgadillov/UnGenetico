from ungenetico import *
import random
from dataclasses import dataclass, field


class MutationIntUniform(Mutation):
    def mutate(self, gen: Gene, ag: GeneticAlgorithm):
        """

        :param gen:
        :param ag:
        :return:
        """
        gen.value = random.randint(gen.min_val, gen.max_val)


@dataclass
class MutationIntNotUniform(Mutation):
    b: float = 0.5

    def mutate(self, gen: Gene, ag: GeneticAlgorithm):
        """

        Parameters
        ----------
        gen
        ag

        Returns
        -------

        """
        t = ag.generation
        tmax = ag.generation_max
        beta = random.randint(0, 1)
        r = random.uniform(0, 1)
        delta = (gen.max_val-gen.value) * (1 - r**(1-t/tmax)**self.b)
        if beta == 0:
            value = gen.value + delta
        else:
            value = gen.value - delta
        print(f'min: {gen.min_val}  max: {gen.max_val}   val: {gen.value}')
        gen.value = value
        print(f'new: {gen.value}')


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
    value: int
    mutation_operator: Mutation

    _name: str = field(init=False, repr=False)
    _min: int = field(init=False, repr=False)
    _max: int = field(init=False, repr=False)
    _val: int = field(init=False, repr=False)
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
            self._val = random.randint(self.min_val, self.max_val)
        else:
            if val < self.min_val:
                val = self.min_val
            elif val > self.max_val:
                val = self.max_val
            self._val = int(val)

    @property
    def min_val(self):
        return self._min

    @min_val.setter
    def min_val(self, min_val):
        self._min = int(min_val)

    @property
    def max_val(self):
        return self._max

    @max_val.setter
    def max_val(self, max_val):
        self._max = int(max_val)

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
            self.mutation_operator = MutationIntUniform()
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



