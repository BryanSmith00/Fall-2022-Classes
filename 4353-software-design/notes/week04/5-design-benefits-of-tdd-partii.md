# 4353 Software Design Week 04 Videos Part 5 Design Benefits of TDD: Part II

Canary test - assert true is true
- Makes sure the environment is correct and there are no errors
- Only need one at each project

Tasks list:
- Jot down tests that come to mind
- Pick the test that is fairly easy and quick to write
- Pick a test that is valuable
- Write a failing test first then write minimum code to make that pass
- Write down a positive, negative, and exception test
- Listen to the whispers of the tests (do the tests indicate that things are too complex?)
- Strive for simplicity, reduce and remove accidental complexity
- Tests should be FAIR
    - F-Fast
    - A-Automated
    - I-Independent
    - R-Repeatable
- Create a setup method which initializes objects and sets data, then call the method before each test to ensure a clean repeatable slate. Can also create teardown if actions need to take place after each test
- When tests pass, take a look at the code and see if it can be simpler (refactor)



<br></br>

Principles
- YAGNI - You Aren't Gonna Need It (yet)
- Last responsible moment - moment beyond which you cannot postpone a decision
- Keep it DRY - Don't Repeat Yourself<br></br>


Positive tests - provides the valid data sets as an input. It checks whether the software application behaves as expected with positive inputs or not

Negative tests - provides invalid or improper data sets as input. It checks whether the software application behaves as expected with the negative or unwanted user inputs. The purpose of negative testing is to ensure that the software application does not crash and remains stable with invalid data inputs

Exception tests - make sure the code throws the correct exception when given an unexpected input<br></br>

Every test should have an assert

Do not put more than 1 assert in a single test