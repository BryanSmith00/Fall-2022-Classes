# 4353 Software Design Week 05 Videos Part 3 Testing with Dependencies: Part III

When a test passes as soon as we write it:

- Examine to ensure the test passed for the right reasons
- It may mean the test is possibly wrong

Try to avoid growing mocks, they should not get complex so they can serve without being a liability

- it is preferable to create multiple simple mocks rather than adding functionality to a single one
- a single mock should have a single purpose

Single level of abstraction principle

- deal with one level of abstraction in your method and leave fine grain detail to another method

Overriding methods on mock tests allows you to avoid or pospone writing interfaces

## Spy

can help selectively replace methods on the object being tested

## Stub

stands in for another object
