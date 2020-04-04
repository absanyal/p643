I have used the secant method to solve two equations. The error tolerance is e=0.001 or 0.1%.

f1 = ut + (1/2)at^2

This is a fundamental equation of motion. Using u = 13.0 and a = -9.8, we get two roots at 0.0 and 2.65306. Deoending on our intial guess, the secant method gives both roots.

Guess               Root
------------------------
-1.0, 5.0           0.0
2.0, 10.0           2.65306

f2 = x^3 + 4x^2 -1

This is an example where finding a closed form solution is difficult, which is a case that often arises in physics. Also, we have multiple roots here. We will show that depending on our initial choice, different roots get picked up. Usually, the root is picked up if the initial guess is "fairly close" to the root.

Guess               Root
------------------------
0.0, 1.0            0.472834
-3.0, -2.0          -3.93543
-1.0, 0.0           -0.537402

I have verified that these roots are accurate at least upto the displayed number of digits.