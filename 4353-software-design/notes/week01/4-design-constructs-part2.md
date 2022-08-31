# 4353 Software Design Week 01 Videos Part 4 Design Constructs Part II

-> denotes a lambda function, (parameters) -> { body }

Traditional for loops are imperative

Functional programming - Take an application and decompose it into a series of operations with functions

- Higher order functions - functions that perform operations on other functions, taking functions as operands or returning them

- Function composition - applying the results of a function to another function (f o g)

OOP method

for( Vehicle vehicle : vehicles) {<br>
&nbsp;&nbsp;&nbsp;&nbsp;useVehicle(vehicle);<br>
}

Functional style (Higher order function)

vehicles.forEach(vehicle -> useVehicle(vehicle)); or...<br>
vehicles.forEarch(Sample::useVehicle); // Where Sample is the class name

// vehicle -> useVehicle(vehicle) is an anonymous function because it has no name (lambda expression)
<br></br>

Medium:<br>
Functional programming is the form of programming that attempts to avoid changing state and mutable data. In a functional program, the output of a function should always be the same, given the same exact inputs to the function.

using objects as inputs to a function means the output is not deterministic and thus not functional<br></br>

Lazy evaluation (wicked cool)<br>
- when passing a function into another function, you dont have to execute that parameter function immediately or at all 

Incidental vs accidental errors