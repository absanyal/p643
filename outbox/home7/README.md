The code uses my own implementation of fixed step-size RK4 to solve a 2nd order ODE. The ODE chosen is a damped, driven oscillator:

y''(t) = -w^2 y(t) -(dc) * y'(t) + F0 * sin(w0 * t)

w: natural frequency of the oscillator
dc: damping constant
F0: amplitude of the driving force
w0: driving frequency

The code also requires as input:

a, b: the intervel in which the ODE is solved
N: number of points to solve for in the interval
y(a) = boundary condition of y at t=a
y'(a) = boundary condition of y' at t=a

For all the output, I have set w=1.0, a = 0.0, b = 100.0, y(a) = 0.0, y'(a) = 1.0.

The following cases have been studies:

1. Natural frequency: dc = 0.0, F0 = 0.0, w0 = 0.0
2. Slightly damped: dc = 0.01, F0 = 0.0, w0 = 0.0
3. Heavily damped: dc = 0.3, F0 = 0.0, w0 = 0.0
4. Critically damped: dc = 2.0, F0 = 0.0, w0 = 0.0
5. Driven at resonant frequency: dc = 0.0, F0 = 0.5, w0 = 1.0
6. Driven at off-resonant frequency: dc = 0.0, F0 = 0.5, w0 = 0.2
7. Damped, driven at resonant frequency: dc = 0.1, F0 = 0.5, w0 = 1.0
8. Damped, driven at off-resonant frequency: dc = 0.1, F0 = 0.5, w0 = 0.2
9. Critically damped, driven: dc = 2.0, F0 = 0.5, w0 = 0.2

For every case, I plotted in coordinate space as well as phase space. More combinations can be done but this suffiently demonstrates the robustness of the solver. There is no perceivable error in the plot; as the step size is small, the error is not large enough to be detected in these examples.