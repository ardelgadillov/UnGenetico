import ungenetico as ung
import random
from dataclasses import dataclass, field


class MutationOperatorIntUniform(ung.MutationOperator):
    def mutate(self, gen: ung.Gene, ag: ung.GeneticAlgorithm):
        """

        :param gen:
        :param ag:
        :return:
        """
        gen.value = random.randint(gen.min_val, gen.max_val)


class MutationOperatorIntNoUniform(ung.MutationOperator):
    def mutate(self, gen: ung.Gene, ag: ung.GeneticAlgorithm):
        """

        :param gen:
        :param ag:
        :return:
        """
        gen.value = random.randint(gen.min_val, gen.max_val)


@dataclass
class GeneInt(ung.Gene):
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
    mutation_operator: ung.MutationOperator

    _name: str = field(init=False, repr=False)
    _min: int = field(init=False, repr=False)
    _max: int = field(init=False, repr=False)
    _val: int = field(init=False, repr=False)
    _mutation_operator:  ung.MutationOperator = field(init=False, repr=False)

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
    def mutation_operator(self, mo: ung.MutationOperator):
        if isinstance(mo, property):
            self.mutation_operator = MutationOperatorIntUniform()
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


a = GeneInt('x', min_val=1, max_val=11)#, mutation_operator=12)
print(a)
a.name = 'y'
print(a)
a2 = GeneInt('y', min_val=1.1, max_val=10.9)
a3 = GeneInt('z', min_val=1.1, max_val=10.9)
a4 = GeneInt('m', min_val=1.1, max_val=10.9)
print(a2)
print(a3)
print(a4)


def obj_expression(z, x, y, m):
    return x * y * z + m


ga = ung.GeneticAlgorithm(
    objective_function=obj_expression)

print(ga)
ga.add_gen(a)
ga.add_gen(a2)
ga.add_gen(a3)
ga.add_gen(a4)
ga.create_population(3)
for ind in ga.actual_generation.generation:
    print(ind)
    for gen in ind.genome:
        print(gen.value)

ga.mutate()
ga.calculate_objective_function()
for ind in ga.actual_generation.generation:
    for gen in ind.genome:
        print(gen.value)
    print(f'objective: {ind.objective_value}')

