# bresenham
##Drawing circles with Bresenham's algorithm
This is a program to illustrate the Bresenham's algorithm to draw circles.

It uses horizontal line drawing primitive to fill a circle and a dot plot
to draw the circunference (in case of no fill)

The program supports options -v (verbose, trace the algorithm variables at
each iteration, don't draw) or -f (fill the circle's interior) and accepts
radii as command line parameters, as in

    bresenham -v 10

	bresenham -f 10

	bresenham 10 20 30

are examples of program execution.
The program tries to {help with a question in StackOverflow}[http://stackoverflow.com/questions/41524497/c-sdl2-rendering-a-circle/41570334#41570334]
