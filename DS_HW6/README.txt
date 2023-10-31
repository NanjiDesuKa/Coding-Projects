HOMEWORK 6: INVERSE WORD SEARCH


NAME:  Robert Wu


COLLABORATORS AND OTHER RESOURCES: 
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

I discussed with Ethan Wong on how to approach the problem, and what 
the base cases would be. I spoke with my friend Henry Yeung about how
to approach the logic of checking the grid for unwanted words, helping
me come up with a vector system of ints to indicate direction.
I used geeksforgeeks to look up how to use the reverse function. I used 
geeksforgeeks again but this time for the std::unique algorithm function.

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.

ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  30


ALGORITHM ANALYSIS:
What's the order notation of your algorithm?

parsing and separating the words is O(2(r+f)) sorting is O(rlogr) and O(flogf).
the check_negative function is 8fhw(l-1) because you need to go through the entire
grid and reference each forbidden word times every character except for the first,
alphabet_masher is 26^(2wh+8fhw(l-1)) because it goes through the entire alphabet
and checks for forbidden words each time, recursion_finder is (8whr+8fhw(l-1))^(r+wh)
because it will go in 8 directions for the whole grid for all required words and check 
for forbidden words and if the grid is filled each time until all words are used up, 
and the solution_finder is O(2wh(2l+26^(2wh)(8whr)^(r+wh))+s(s)+2slogs+ s^2 + whs ) 
utilizing all these big O notations. This means the entire program should be 
O(2(r+f)+rlogr+flogf+2wh(2l+26^(2wh+8fhw(l-1))(8whr+8fhw(l-1))^(r+wh))+2slogs+ 2s^2 + whs).
After simplifying, this reduces to 

O(  rlogr  +  flogf   +wh(  l  +  26^(2wh+8fhw(l-1)  ) * (whr + fhwl)^(r+wh))  +  slogs  + s^2 + whs  ).



TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?

puzzle1.txt has a real runtime of 0.002s
puzzle2.txt has a real runtime of 0.002s
puzzle3.txt has a real runtime of 0.003s
puzzle4.txt has a real runtime of 0.005s
puzzle5.txt has a real runtime of 0.013s
puzzle6.txt has a real runtime of 0.007s
puzzle7.txt has a real runtime of 0.066s
puzzle8.txt has a real runtime of 0.747s

I created test cases 1 through 8.

testcase1.txt tests how efficiently the program fills in one space for
a 5 by 4 with the entire alphabet. 
It has a real runtime 0.009s.

testcase2.txt tests how efficient the program is when filling in a 2 by 2
with words with length equal to the bounds. 
Ith as a real runtime of 0.002s.

testcase3.txt is the exact thing except each letter is subdivided into characters.
This test case has a real runtime of 0.218s.

testcase4.txt tests how fast the program can find the solutions of a 3 by 3 with all
gaps filled. 
It has a real runtime of 0.003s.

testcase5.txt tests if the program can recognize an illegal word that is derivative of a 
added word, which it does. It also tests the no solution parameter for this specific scenario.
The real runtime is 0.003s.

testcase6.txt tests how efficient the program is at creating all solutions for a 5 by 5 with no gaps.
The real runtime is 0.104s.

testcase7.txt tests how efficient the program is at finding all solutions for a 5 by 5 with two gaps.
The real runtime is 38.336s.

testcase8.txt tests if the program recognizes that a word to be included exceeds the bounds of 
the grid and returns no solution. It returns no solution. 
It has a real runtime of 0.002s.

testcase9.txt tests the general fill in gaps function with a 2 by 2 with only one letter filled in.
It has a real runtime of 10.106s.

MISC. COMMENTS TO GRADER:  
Optional, please be concise!


