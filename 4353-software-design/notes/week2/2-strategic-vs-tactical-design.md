# 4353 Software Design Week 2 Videos Part 2 Strategic Design vs. Tactical Design

Creating a big upfront design is a bad idea: becomes irrelevant fast (waterfall)

Evolutionary design: a design you can iterate on

Emergent design: only designing as it comes up

## Strategic design
high level design about where we would like to go given what we know now
- map of where the software is going
- okay to specify components and classes and their relationships but not much finer grain than that

## Tactical design
low level design, what are the specifics of the classes, what methods will we have, what parameters will the methods have, etc.
- refine the strategic design into an actual implementation
- the devil is in the details (should this method have these parameters, should we rename it to something else, etc)

Last responsible moment (agile) - the moment beyond which you cannot possibly make this decision and before which you dont have to make the decision 

The knowns and the unknowns
- when starting a project there is only a certain amount of detail you can know ahead of time (few knowns and many unknowns)

observer effect - when someone starts using software they come up with a lot of questions and ways to improve it

do enough or minimum up front design rather than big upfront design

You can express strategic designs with UML diagrams or test driven development

Dont be afraid to rewrite the tactical details as you gain more information, prototype