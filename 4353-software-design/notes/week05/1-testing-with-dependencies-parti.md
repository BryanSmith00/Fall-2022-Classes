# 4353 Software Design Week 05 Videos Part 1 Testing with Dependencies: Part I

Writing unit tests on code with dependencies is hard

- Automation is key

## Stub vs. mock

Stub: a dummy piece of code that lets the test run, but you don't care what happens to it. Substitutes for real working code.

Mock: a dummy piece of code that you verify is called correctly as part of the test. Substitutes for real working code.

## Mocks (use selectively)

Object that stands in for a real object

Keeps track of interactions like how many functions calls

- behavior and interaction testing

ALlow you to simulate behavior of the code you are testing but also allows you to simulate ill-behavior for when the code you are depending on fails

## Stub

Useful for testing the state of an object after an interaction

- state testing

## Fake

Class or service that is like the real on for all practiccal purposes but used for testing and not usable in production, for example, like an in-memory database, or fake credit card keys

## Spy

Proxy that stands in front of a real object

## Dependency Inversion Principle

Decoupling tested code from the code it depends on through an interface that easily allows for substituting a mock object in
