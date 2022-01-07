from abc import ABC, abstractmethod
from ungenetico.gene import Gene
#from ungenetico.ga import GeneticAlgorithm
from ungenetico.population import Population
import random
from dataclasses import dataclass
from itertools import accumulate
import copy
import numpy as np


class Mutation(ABC):
    """Abstract class"""
    @abstractmethod
    def mutate(self, gen: Gene, ag):
        pass


class Crossover(ABC):
    """Abstract class"""
    @abstractmethod
    def exchange(self, gen1: Gene, gen2: Gene, ag):
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
        gen.value = [random.uniform(gen.min_val, gen.max_val) for _ in range(gen.length)]


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
        values = [None]*gen.length
        for i in range(gen.length):
            beta = random.randint(0, 1)
            r = random.uniform(0, 1)
            delta = (gen.max_val-gen.value[i]) * (1 - r**(1-t/tmax)**self.b)
            if beta == 0:
                value = gen.value[i] + delta
            else:
                value = gen.value[i] - delta
            values[i] = value
        gen.value = values


class CrossoverSimple(Crossover):
    def exchange(self, gen1: Gene, gen2: Gene, ag):
        return gen2.value, gen1.value


@dataclass
class CrossoverArithmetic(Crossover):
    alpha: float = 0.7

    def exchange(self, gen1: Gene, gen2: Gene, ag):
        child1 = [None]*gen1.length
        child2 = [None]*gen1.length
        for i in range(gen1.length):
            child1[i] = self.alpha*gen1.value[i] + (1-self.alpha)*gen2.value[i]
            child2[i] = self.alpha*gen2.value[i] + (1-self.alpha)*gen1.value[i]
        return child1, child2


class ProbabilityUniform(Probability):
    def assign_probability(self, pop: Population, ag):
        prob = 1/pop.size
        for ind in pop.population:
            ind.survival_probability = prob


class ProbabilityProportional(Probability):
    def assign_probability(self, pop: Population, ag):
        ga_min = ag.objective_min
        sov_max = sum([ind.objective_value + (1-ga_min) for ind in pop.population])
        sov_min = sum([1/(ind.objective_value + (1 - ga_min)) for ind in pop.population])
        if ag.optimization == 'maximization':
            for ind in pop.population:
                ind.survival_probability = (ind.objective_value + (1-ga_min)) / sov_max
        else:
            for ind in pop.population:
                ind.survival_probability = (1/(ind.objective_value + (1-ga_min))) / sov_min


class ProbabilityLineal(Probability):
    def assign_probability(self, pop: Population, ag):
        n = pop.size
        if ag.optimization == 'maximization':
            pop.sort_population('descending', 'objective_value')
        else:
            pop.sort_population('ascending', 'objective_value')
        for i in range(n):
            pop.population[i].survival_probability = 2*(n-i) / (n*(n+1))


class SelectionStochastic(Selection):
    def select(self, pop: Population, ag):
        pop.sort_population('descending', 'survival_probability')
        prob = [ind.survival_probability for ind in pop.population]
        angle = list(accumulate(prob))
        new_population = Population()
        for i in range(pop.size):
            roulette = random.uniform(0, 1)
            pos = len([1 for jind in angle if roulette >= jind])
            new_population.append_individual(copy.deepcopy(pop.population[pos]))
        pop.population = new_population.population


class PairingRandom(Pairing):
    def match(self, pop: Population, ag):
        pop.parents = random.sample(range(pop.size), pop.size)


class PairingAdjacent(Pairing):
    def match(self, pop: Population, ag):
        n = list(range(1, pop.size))
        n.append(pop.size-1)
        pop.partners = n


class PairingExtremes(Pairing):
    def match(self, pop: Population, ag):
        pop.partners = list(range(pop.size-1, -1, -1))


class ReproductionSimple(Reproduction):
    def reproduce(self, pop: Population, ag):
        for ind in pop.population:
            ind.paired = False

        for index in range(pop.size):
            parent1 = pop.population[index]
            parent2 = pop.population[pop.partners[index]]
            if not parent1.paired and not parent2.paired:
                parent1.paired = True
                parent2.paired = True
                exchange_point = random.randint(0, len(parent1.genome))
                for i in range(exchange_point, len(parent1.genome)):
                    parent1.genome[i].value, parent2.genome[i].value = parent1.genome[i].exchange(parent2.genome[i], ag)


class ReproductionTwoParentsTwoChildren(Reproduction):
    def reproduce(self, pop: Population, ag):
        for ind in pop.population:
            ind.paired = False

        for index in range(pop.size):
            parent1 = pop.population[index]
            parent2 = pop.population[pop.partners[index]]
            if not parent1.paired and not parent2.paired:
                parent1.paired = True
                parent2.paired = True
                for i in range(len(parent1.genome)):
                    parent1.genome[i].value, parent2.genome[i].value = parent1.genome[i].exchange(parent2.genome[i], ag)


class ReproductionBestParentBestChild(Reproduction):
    def reproduce(self, pop: Population, ag):
        for ind in pop.population:
            ind.paired = False

        for index in range(pop.size):
            parent1 = pop.population[index]
            parent2 = pop.population[pop.partners[index]]
            if not parent1.paired and not parent2.paired:
                parent1.paired = True
                parent2.paired = True

                child1 = copy.deepcopy(parent1)
                child2 = copy.deepcopy(parent2)

                for i in range(len(parent1.genome)):
                    child1.genome[i].value, child2.genome[i].value = parent1.genome[i].exchange(parent2.genome[i], ag)

                parent1.calculate_objective_function(ag.objective_function)
                parent2.calculate_objective_function(ag.objective_function)
                child1.calculate_objective_function(ag.objective_function)
                child2.calculate_objective_function(ag.objective_function)

                if ag.optimization == 'maximization':
                    if parent2.objective_value > parent1.objective_value:
                        #pop.population[index] = copy.deepcopy(parent2)
                        pop.population[index] = parent2

                    if child1.objective_value > child2.objective_value:
                        pop.population[pop.partners[index]] = child1
                    else:
                        pop.population[pop.partners[index]] = child2
                else:
                    if parent2.objective_value < parent1.objective_value:
                        # pop.population[index] = copy.deepcopy(parent2)
                        pop.population[index] = parent2

                    if child1.objective_value < child2.objective_value:
                        pop.population[pop.partners[index]] = child1
                    else:
                        pop.population[pop.partners[index]] = child2


class ReproductionBestBetweenParentsChildren(Reproduction):
    def reproduce(self, pop: Population, ag):
        for ind in pop.population:
            ind.paired = False

        for index in range(pop.size):
            parent1 = pop.population[index]
            parent2 = pop.population[pop.partners[index]]
            if not parent1.paired and not parent2.paired:
                parent1.paired = True
                parent2.paired = True

                child1 = copy.deepcopy(parent1)
                child2 = copy.deepcopy(parent2)

                for i in range(len(parent1.genome)):
                    child1.genome[i].value, child2.genome[i].value = parent1.genome[i].exchange(parent2.genome[i], ag)

                parent1.calculate_objective_function(ag.objective_function)
                parent2.calculate_objective_function(ag.objective_function)
                child1.calculate_objective_function(ag.objective_function)
                child2.calculate_objective_function(ag.objective_function)

                group = [parent1, parent2, child1, child2]

                obj_vals = np.array([parent1.objective_value, parent2.objective_value, child1.objective_value, child2.objective_value])
                arg_obj_vals = np.argsort(obj_vals)
                if ag.optimization == 'maximization':
                    pop.population[index] = group[arg_obj_vals[-1]]
                    pop.population[pop.partners[index]] = group[arg_obj_vals[-2]]
                else:
                    pop.population[index] = group[arg_obj_vals[0]]
                    pop.population[pop.partners[index]] = group[arg_obj_vals[1]]
