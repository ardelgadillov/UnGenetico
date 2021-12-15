import ungenetico as ung
import random
import copy
import inspect

from dataclasses import dataclass, field


class MutationOperatorIntUniform(ung.MutationOperator):
    def mutate(self, gen: ung.Gene):
        gen.value = random.randint(gen.min_val, gen.max_val)


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
    def __init__(self, name,
                 val: int = None,
                 min_val: int = None,
                 max_val: int = None,
                 mutation_operator: ung.MutationOperator = None):
        """name"""
        self._name = name
        self._min = None
        self._max = None
        self._val = None
        self._mutation_operator = None
        if min_val is not None:
            self.min_val = min_val
        if max_val is not None:
            self.max_val = max_val
        if val is not None:
            self.value = val
        if mutation_operator is None:
            self.mutation_operator = MutationOperatorIntUniform()
        else:
            self.mutation_operator = mutation_operator

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

    # @name.setter
    # def name(self, name):
    #     self._name = name

    @property
    def value(self):
        return self._val

    @value.setter
    def value(self, val):
        if val < self.min_val:
            val = self.min_val
        elif val > self.max_val:
            val = self.max_val
        self._val = int(val)

    @property
    def mutation_operator(self):
        return self._mutation_operator

    @mutation_operator.setter
    def mutation_operator(self, mo: ung.MutationOperator):
        self._mutation_operator = mo

    # @property
    # def mutation_operator(self):
    #     print('get mutation')
    #     return self._mutation_operator
    #
    # @mutation_operator.setter
    # def mutation_operator(self, value):
    #     print(f'set mutation {value}')
    #     self._mutation_operator = value
    #
    # @property
    # def crossover_operator(self):
    #     print('crossover')
    #     return self._crossover_operator
    #
    # @crossover_operator.setter
    # def crossover_operator(self, value):
    #     self._crossover_operator = value


@dataclass
class GeneInt2(ung.Gene):
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
    mutation_operator: ung.MutationOperator
    value: int

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
    def min_val(self):
        return self._min

    @min_val.setter
    def min_val(self, min_val):
        print('min')
        self._min = int(min_val)

    @property
    def max_val(self):
        return self._max

    @max_val.setter
    def max_val(self, max_val):
        print('max')
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
        print('name')
        if not hasattr(self, 'name'):
            self._name = name

    @property
    def mutation_operator(self):
        return self._mutation_operator

    @mutation_operator.setter
    def mutation_operator(self, mo: ung.MutationOperator):
        print('set mutation')
        if isinstance(mo, property):
            self.mutation_operator = MutationOperatorIntUniform()
        else:
            self._mutation_operator = mo

    @property
    def value(self):
        return self._val

    @value.setter
    def value(self, val):
        print('value')
        if isinstance(val, property):
            self._val = random.randint(self.min_val, self.max_val)
        else:
            if val < self.min_val:
                val = self.min_val
            elif val > self.max_val:
                val = self.max_val
            self._val = int(val)

    # @property
    # def mutation_operator(self):
    #     print('get mutation')
    #     return self._mutation_operator
    #
    # @mutation_operator.setter
    # def mutation_operator(self, value):
    #     print(f'set mutation {value}')
    #     self._mutation_operator = value
    #
    # @property
    # def crossover_operator(self):
    #     print('crossover')
    #     return self._crossover_operator
    #
    # @crossover_operator.setter
    # def crossover_operator(self, value):
    #     self._crossover_operator = value


a = GeneInt2('x', min_val=1, max_val=11)#, mutation_operator=12)
print(a)
a.name = 'y'
print(a)
a2 = GeneInt('y', min_val=1.1, max_val=10.9)
a3 = GeneInt('z', min_val=1.1, max_val=10.9)
a4 = GeneInt('m', min_val=1.1, max_val=10.9)  # , mutation_operator=MutationOperatorIntUniform())

print(a._name)


def obj_expression(z, x, y, m):
    return x * y * z + m


# b = ung.Individual(of=obj_expression)
# # print(f'----------q{b.of(*[2,3,4])}')
#
# b.append_gen(a)
# b.append_gen(a2)
# b.append_gen(a3)
# b.append_gen(a4)
#
# for gen in b.genome:
#     gen.mutate()
# for gen in b.genome:
#     print(gen.value)
#     print(f' mo: {gen.mutation_operator}')
#
# c = []
# for i in range(3):
#     c.append(copy.deepcopy(b))
#
# for i in c:
#     for gen in i.genome:
#         gen.mutate()
#
# for i in c:
#     for gen in i.genome:
#         print(gen.value)
#
# bn = [gen.name for gen in b.genome]
# valu = [gen.value for gen in b.genome]
# vari = inspect.getfullargspec(b.of).args
# print(bn)
# print(valu)
# print(vari)
# pos = [bn.index(i) for i in vari]
# valus = [valu[i] for i in pos]
#
# # print(bn.index('x'))
# # print(bn.index('y'))
# # print(bn.index('z'))
# print(b.of(*valus))
#
# print(b)
# print(b.genome)
# print(b.genome[1])
#
# b.genome[1] = GenInt('y2', min_val=1.1, max_val=10.9)
# print(b)
# print(b.genome)
# print(b.genome[1])
##################
ga = ung.GeneticAlgorithm()
ga.objective_function = obj_expression
ga.add_gen(a)
ga.add_gen(a2)
ga.add_gen(a3)
ga.add_gen(a4)
print(ga._genome_base)
ga.create_population(3)
for ind in ga._population.generation:
    print(ind)
    for gen in ind.genome:
         print(gen.value)

ga.mutate()
ga.evaluate_objective_function()
for ind in ga._population.generation:
    for gen in ind.genome:
        print(gen.value)
    print(f'objective: {ind.objective}')

# print(a.__doc__)

