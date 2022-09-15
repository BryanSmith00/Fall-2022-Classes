# 4353 Software Design Week 03 Videos Part 3 Modeling Relationships

packages grouping

Package - logical grouping

physical group, where are the libraries stored, jars or assemblies

package modeling, logical group can contain classes and objects within them.

## Relationships between classes

Association - relationship between two classes

Link - an instance of an association

Associations often appear as verbs

bi-directional vs unidirectional relationships

0..1 is a zero or 1 relationship

0..n any number relationship

cardinality

many-to-many 

binary relationship - typical 1:1 n:m etc.

ternary relationship - three different objects involved

n-ary
 - complicated hard to implement

associative relationships are usually between objects on the same level no ownership or domination

Open diamond, one object owns another - aggregation (has-a, part-whole)

Composition - strong, shaded diamond, object owns another object but their lifetimes are tied together