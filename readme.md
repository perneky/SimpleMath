[![Build Status](https://travis-ci.org/perneky/SimpleMath.svg?branch=master)](https://travis-ci.org/perneky/SimpleMath/builds)

# Simple Math

Simple Math is a small and easy math scripting library. It is lightweight, embeddable and expandable by 
the host application without changing the library.

It works with 1-4 dimension values, so it is suitable for being the source of 2D-3D animation.

It evaluates expresions like "sin( time * 0.01 ) * 0.5 + 0.5" into a simple C++ object which can be 
evaluated later. Animation can be done by using external variables is the script, which can be provided 
at evaluation time. In the example above, "time" is such a variable.

A sample project is provided, which is a simple command line evaluator.