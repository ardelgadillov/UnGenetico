from abc import ABC, abstractmethod
from ungenetico.gene import Gene
#from ungenetico.ga import GeneticAlgorithm
from ungenetico.population import Population
import random
from dataclasses import dataclass
from itertools import accumulate
import copy


class Mutation(ABC):
    """Abstract class"""
    @abstractmethod
    def mutate(self, gen: Gene, ag):
        pass


class Crossover(ABC):
    """Abstract class"""
    @abstractmethod
    def exchange(self, gen: Gene, ag):
        pass


class Probability(ABC):
    """Abstract class"""
    @abstractmethod
    def assign_probability(self, pop: Population, ag):
        pass


class Selection(ABC):
    """Abstract class"""
    @abstractmethod
    def select(self, pop: Population, ag):
        pass


class Pairing(ABC):
    """Abstract class"""
    @abstractmethod
    def match(self, pop: Population, ag):
        pass


class Reproduction(ABC):
    """Abstract class"""
    @abstractmethod
    def reproduce(self, pop: Population, ag):
        pass


class MutationUniform(Mutation):
    def mutate(self, gen: Gene, ag):
        """

        :param gen:
        :param ag:
        :return:
        """
        gen.value = random.uniform(gen.min_val, gen.max_val)


@dataclass
class MutationNotUniform(Mutation):
    b: float = 0.5

    def mutate(self, gen: Gene, ag):
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
        gen.value = value


class ProbabilityUniform(Probability):
    def assign_probability(self, pop: Population, ag):
        prob = 1/pop.size
        for ind in pop.population:
            ind.survival_probability = prob


class SelectionStochastic(Selection):
    def select(self, pop: Population, ag):
        prob = [ind.survival_probability for ind in pop.population]
        angle = list(accumulate(prob))
        new_population = Population()
        for i in range(pop.size):
            roulette = random.uniform(0, 1)
            pos = len([1 for jind in angle if roulette >= jind])
            print(f'rou: {roulette}')
            print(pos)
            new_population.append_individual(copy.deepcopy(pop.population[pos]))
        print(pop.population)
        print(new_population.population)
        pop.population = new_population.population


class PairingRandom(Pairing):
    def match(self, pop: Population, ag):
        pop.partners = random.sample(range(pop.size), pop.size)
        print(pop.partners)