import ungenetico as ung
import math
from statistics import mean


def obj_expression(z, x, y, m):
    return x * y * z + m


a = ung.GeneInt('x', min_val=1, max_val=11, length=10)
print(a)
a.name = 'y'
print(a)
a2 = ung.GeneInt('y', min_val=1.1, max_val=10.9, mutation_operator=ung.MutationUniform())
a3 = ung.GeneBool('z')
a4 = ung.GeneFloat('m', min_val=1.1, max_val=10.9, mutation_operator=ung.MutationNotUniform())
print(a2)
print(a3)
print(a3.min_val)
print(a3.max_val)
print(a4)



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

# -------------------
# def obj_expression(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9,
#                    x10, x11, x12, x13, x14, x15, x16, x17, x18, x19,
#                    x20, x21, x22, x23, x24):
#     of = 0
#     x = [x0, x1, x2, x3, x4, x5, x6, x7, x8, x9,
#          x10, x11, x12, x13, x14, x15, x16, x17, x18, x19,
#          x20, x21, x22, x23, x24]
#     for xi in x:
#         of += xi
#     return of
#
#
# ga = ung.GeneticAlgorithm(objective_function=obj_expression, optimization='minimization')
# for i in range(25):
#     ga.add_gen(ung.GeneFloat(f'x{i}', -10, 10))
#
# ga.create_population(10)
# ga.mutate()
# # print(ga)
# for _ in range(3):
#     print(f'----------{_}')
#     ga.calculate_objective_function()
#     #print(ga.objective_mean)
#     ga.assign_probability()
#     print(ga)
#     # of = [ind.objective_value for ind in ga.actual_generation.population]
#     # print(of)
#     # of = [ind.survival_probability for ind in ga.actual_generation.population]
#     # print(of)
#     ga.sort_population('descending', 'survival_probability')
#     ga.select()
#     ga.match()
#     ga.reproduce()
#     ga.calculate_objective_function()
#     print(ga.objective_mean)
#     # ga.mutate()
#     # ga.calculate_objective_function()
#     # #of = [ind.objective_value for ind in ga.actual_generation.population]
#     # #print(of)
#     # print(ga.objective_mean)
# #print(ga)
