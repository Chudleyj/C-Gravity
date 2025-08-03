# C-Gravity:
Gravity visualization simulation in C using OpenGL

This simulation implements the *Runge-Kutta-Fhelberg method* for approximating solutions to Ordinary Differential Equations (Newton's Law of Graviational Attraction being an ODE)
Implenetation is dervivied from *Low-Order Classical Runge-Kutta Formulas with Stepsize Control and Their Application to Some Heat Transfer Problems* by Erwin Fhelberg (NASA TR R-315, 1969) and can be found at:

https://ntrs.nasa.gov/api/citations/19690021375/downloads/19690021375.pdf

Coefficients used in the algorithm can also be found in the above, though one may find it easier to locate these references at https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method

Work in progress, gods speed if you try to build/run this on your PC in its current state. 

<br>


# Examples: 

Please excuse the choppy lighting in the below examples for now, it is much smoother when I am running it versus how it comes out in the recordings, but I plan to redo lighting and add textures in the future which will *hopefully* make better videos.

https://github.com/user-attachments/assets/f81072e6-05d7-4fb8-a190-0f096e87dc14

Overhead shot:

https://github.com/user-attachments/assets/92780b8d-4c4d-4419-abcf-5e280bca903c

Zoom in to show there are moon orbits:

https://github.com/user-attachments/assets/a5d7e700-452b-484b-ac79-9cc453a9ac17

<br>


# A note on *Low-Order Classical Runge-Kutta Formulas with Stepsize Control and Their Application to Some Heat Transfer Problems*:

For those who are curious, a draft of the below is presented which is part of a larger work related to this project:


## A Correction to the Truncation Error Coefficient for the Runge-Kutta-Fehlberg (RKF45) Method in *Low-Order Classical Runge-Kutta Formulas with Stepsize Control and their Application to Some Heat Transfer Problems* (NASA TR R-315, 1969)

**Author:** chudleyj 

**Date:** August 2025

## Abstract

In his 1969 NASA Technical Report R-315, Erwin Fehlberg presented the now widely used Runge-Kutta-Fehlberg (RKF45) method for solving ordinary differential equations (ODE). A typographical error in the leading term of the local truncation error is identified and clarified here. While this error does not propagate to other numerical results or alter the reliability of the method in practice, its documentation is important for pedagogical clarity and historical accuracy.

## Introduction

The RKF45 method remains a cornerstone of numerical ODE solvers. Fehlberg's 1969 NASA report, *Low-Order Classical Runge-Kutta Formulas with Stepsize Control and their Application to Some Heat Transfer Problems*, is often referenced in both software implementations and academic texts. On page 12 of the report, Fehlberg explains that the leading term in the local truncation error can be found by subtracting the coefficients in the last two columns of Table III. These correspond to the weights of the 5th- and 4th-order Runge-Kutta methods, respectively. However, on page 14, a small but significant error appears in the result of that subtraction.

## The Reported Error

Fehlberg's Table III (page 13) presents the coefficients for the 4th- and 5th-order formulas. The last two columns in that table are:

- $C_K = \frac{2197}{4104}$
- $C_K^{\hat{}} = \frac{28561}{56430}$

Fehlberg then states (page 14) that the difference between these values—used to compute the coefficient of the leading term in the local truncation error—is:

$$C_{T,K} = C_K - C_K^{\hat{}} = \frac{2187}{75240}$$

**This result is incorrect.** The correct difference is:

$$\frac{2197}{4104} - \frac{28561}{56430} = \frac{2197}{75240}$$

not $\frac{2187}{75240}$ as printed. This appears to be a typographical error where "2197" was mistyped as "2187".

## Significance of the Error

This coefficient appears in an illustrative formula for the local truncation error and does not affect the main numerical experiments or the design of the RKF45 algorithm. Nonetheless, this value has occasionally been repeated in derivative materials, and its correction may assist students and developers in verifying the RKF45 coefficients themselves.

## Has the Error Propagated?

We have not found evidence that this typographical error propagated into software implementations. Widely used sources such as Hairer's *Solving Ordinary Differential Equations I* list the correct coefficients for both the 4th- and 5th-order formulas. Additionally, established Fortran libraries such as those on Netlib (e.g., `rkf45.f`) implement the correct Fehlberg pair and do not include the subtraction error.

## Impact

This error:

- **Does not affect** the core RKF45 method, which uses the original coefficients in Table III
- **Is not propagated** into the later sections of the report, which rely on numerical experiments, not symbolic expressions
- **May cause confusion** for readers attempting to independently verify the derivation of the local truncation error term or re-derive the RK pair
- **Prompted debate** on the [Wikipedia article on the Runge–Kutta–Fehlberg method](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method), where contributors disagreed about whether the stated coefficient was a typo or a valid form. Clarifying this helps stabilize open knowledge sources and prevents future re-edits based on the original misprint

## Verification

To verify this correction yourself, calculate:

```
2197/4104 = 0.535331384015594...
28561/56430 = 0.506097560975610...
Difference = 0.029233823039984... = 2197/75240
```

The incorrect value would give:
```
2187/75240 = 0.029100478468900...
```

## Conclusion

Although minor, this error in the original RKF45 paper deserves correction for historical accuracy and pedagogical clarity. It offers a useful example in courses or documentation that involve deriving or verifying Runge–Kutta coefficients. By clearly presenting the origin and nature of the error, we aim to provide a definitive reference—one that can help resolve future disputes in open knowledge sources and assist students and developers in confirming the correct coefficients with confidence.

## References

1. Fehlberg, E. (1969). *Low-order classical Runge-Kutta formulas with step-size control and their application to some heat transfer problems*. NASA Technical Report R-315. [Link](https://ntrs.nasa.gov/citations/19690021375)

2. Hairer, E., Nørsett, S. P., & Wanner, G. (1993). *Solving Ordinary Differential Equations I: Nonstiff Problems*. Springer.

3. Dormand, J. R., & Prince, P. J. (1980). A family of embedded Runge-Kutta formulae. *Journal of Computational and Applied Mathematics*, 6(1), 19–26.

4. SLATEC Common Mathematical Library (1993). *Subroutine RKF45*. Netlib Repository. [Link](https://netlib.org/slatec/src/rkf45.f)

---

*This correction is provided in the interest of mathematical accuracy and educational clarity.*
