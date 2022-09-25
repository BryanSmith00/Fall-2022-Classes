# 4353 Software Design Week 05 Videos Part 4 Testing with Dependencies: Part IV

## To mock or not mock

- mocks should be used sparingly

The purpose of mocking is to make tests easier to write and easier to run

mocks should influence the design??

mock code that you are directly dependent on

do not mock multiple levels at a time

having to mock out a lot of stuff in a single test is a sign of bad design that is too tightly coupled

Don't create one mock per dependency, one per usage

Before inverting or injecting dependencies, try eliminating them

### Working with databases

Run the test "transactions" in roll-back only mode
