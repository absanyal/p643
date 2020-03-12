I have solved the Poisson equation using the successive over-relaxation method.

The Poisson equation is an important equation in electrodynamics. It is essentially Laplace's equation with a non-zero source term, and can be used for solving problems where a field is created by some sources, such as solving for the potential created by some charge density.

I have used a charge distribution on a lattice of (LxL) given as

rho(i,j) = q * Sin((2 * pi * (i+j)/L))

where q is a point charge of magnitude 10.0. The boundary is always fixed to a potential of 0.

This gives rise to a sinosoidally varying potential.