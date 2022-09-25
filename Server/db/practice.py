from matplotlib.pyplot import plot
from math import atan, pi

class MyList(list):
    def __str__(self):
        out = ''
        for val in self:
            out += str(val) + ' '
        return out.strip()


years = MyList([2014, 2019, 2022, 2022])
events = MyList(['Крым', 'Ковид', 'Спецоперация', 'Мобилизация'])
massege = f'Текущие события: {years} - {events}'
print(massege)
print('---')
for i in range(len(years)):
    print(f'Текущие события: {years[i]} - {events[i]}')

print(years.reverse())
print(sorted(years))
print(years)

def func(a, b, c, d=0):
    print(a, b, c, d)
    return a+b+c+d

f = func
print(func(1,2,3))
print(years)


a = lambda x: 250000/((x**2+250**2)*(9*x**2+3500**2))**0.5
fi = lambda w: -atan(4250/(875000-3*w**2))*180/pi

W = [w for w in range(0, 10000, 100)]

A = [a(w) for w in W]
Fi = [fi(w) for w in W]

for i in range(len(W)):
    print(f'W = {W[i]};  A = {round(A[i],4)};  Fi = {round(Fi[i], 2)}')