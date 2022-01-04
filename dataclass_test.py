import ungenetico as ung
import math
from statistics import mean


# def obj_expression(z, x, y, m):
#     return x * y * z + m
#
#
# a = ung.GeneInt('x', min_val=1, max_val=11)
# print(a)
# a.name = 'y'
# print(a)
# a2 = ung.GeneInt('y', min_val=1.1, max_val=10.9, mutation_operator=ung.MutationUniform())
# a3 = ung.GeneBool('z')
# a4 = ung.GeneFloat('m', min_val=1.1, max_val=10.9, mutation_operator=ung.MutationNotUniform())
# print(a2)
# print(a3)
# print(a3.min_val)
# print(a3.max_val)
# print(a4)
#
#
#
#
#
# ga = ung.GeneticAlgorithm(
#     objective_function=obj_expression)
#
# print(ga)
# ga.add_gen(a)
# ga.add_gen(a2)
# ga.add_gen(a3)
# ga.add_gen(a4)
# ga.create_population(3)
# for ind in ga.actual_generation.population:
#     print(ind)
#     for gen in ind.genome:
#         print(gen.value)
#
# ga.mutate()
# ga.calculate_objective_function()
# for ind in ga.actual_generation.population:
#     for gen in ind.genome:
#         print(gen.value)
#     print(f'objective: {ind.objective_value}')
#
# for _ in range(10):
#     a3.mutate(ga)
#     print(a3)
#
# print(ga.actual_generation.size)
# ga.assign_probability()
# ga.actual_generation.sort_population()
# for ind in ga.actual_generation.population:
#     print(ind.survival_probability)
#
# ga.select()
# ga.match()
# print(ga.actual_generation)
# ga.reproduce()
# print(ga.actual_generation)
# #print(ga.actual_generation)

def obj_expression(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, y, z):
    of = x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + 4*y + math.sin(15*z)*math.exp(-z)
    if y % 2 == 0:
        of -= 100
    return of


ga = ung.GeneticAlgorithm(objective_function=obj_expression)
for i in range(10):
    ga.add_gen(ung.GeneBool(f'x{i}'))
ga.add_gen(ung.GeneInt('y', -10, 40))
ga.add_gen(ung.GeneFloat('z', 0, 6))

ga.create_population(100)
for _ in range(10):
    #print(f'----------{_}')
    ga.calculate_objective_function()
    ga.assign_probability()
    ga.actual_generation.sort_population()
    ga.select()
    ga.match()
    ga.reproduce()
    ga.mutate()
    of = [ind.objective_value for ind in ga.actual_generation.population]
    #print(of)
    print(max(of))
