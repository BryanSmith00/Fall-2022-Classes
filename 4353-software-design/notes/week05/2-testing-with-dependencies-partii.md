# 4353 Software Design Week 05 Videos Part 2 Testing with Dependencies: Part II

Working on a hypothetical problem about creating an application to exchange currency

Test examples

- markdown by 2% a value of 100 (profit)
- markdown by 2% a value of 97
- markdown by 2% a value of 50
- markdown by 2% a value of 0
- get rate when there is no vendor (dependencies)
- get rate when there is one vendor
- get rate when there is one vendor with error
- get rate when two vendors with first having higher rate
- get rate when two vendors with second having higher rate
- get rate should log an error when vendor fails

Start with test cases that get you moving before being dragged into dependencies

## Dependencies

In this example vendors are dependencies for the currency exchange application

to test vendors we implement dependency inversion principle

- currency exchange should talk to an interface that the vendor can interface with to decouple them

## Injection

### Constructor based injection

Set the class value directly in the constructor with a mock

- Inflexible
- Difficult to work with multiple values

### Setter based injection

Inject the mock by using setter methods on the object

- Easier to work with and change

### Factory based injection

Used in classes without setters

- Useful when there are many classes that need to access the same type of object and a factory can provide a uniform injection

Goes out to a factory object to get the mock object
