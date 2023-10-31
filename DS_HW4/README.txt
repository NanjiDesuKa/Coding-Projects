HOMEWORK 4: TOOL RENTAL LISTS


NAME:  Robert Wu


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

I used geeksforgeeks for linked lists help, as well as iterators.

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  30



ORDER NOTATION:
Use these variables to analyze each phase of computation.  Briefly
justify your answer for each part.


i = # of different inventory items
c = # of different customers 
p = # of pending customers 
q = max quantity of a particular tool owned by the store 
r = max # of tools rented or requested by a customer at one time
p = total # of pending tool rentals
e = # of rental or return events


Load inventory file: O(i)
This just goes through the entire inventory file and parses it directly
into the inventory list, which uses pushback, which is O(1) for a linked
list. 1*i is still i. There are no other significant loops.



Rental event: O(1)
My rental is about three lines of code. I subtract the available quantity
with the request quantity and just pushback the rental request into the 
rent list. Pushback using linked lists has a runtime of O(1).



Return event - no pending customer(s): O(2i + q)
I have to go through the inventory list twice and the rent list once.
The rent list contains all the different rentals that are currently active.
I have to check if the item exists in the inventory list, if it was rented
by the same client in the past, then find the correct iterator for the item
in the inventory by going through the inventory again.



Return event - with pending customer(s): O(2i +2q + p)
I have to do the same thing as above, but this time I have to go through
the rent list again and go once through the wait list. Go through the entire 
wait list and try to fulfill as many requests as possible. Then go through 
the entire rent list to attempt to search for the same person requesting the 
same item to remove the request.



Entire program: O((i)+(p+q)(i+p+q+c)+(p+q)(i+c+e)+i+c+p+q+(c(p+q))+i(q+p)+c(q+p))

This is the unsimplified equation for every single function call in my program.
This is the combination of every single parse, fulfill rent or return, sort,
erase, and write function. If we simplify it down and ignore lesser runtimes,
 we get a big O notation of 

O((p+q)(i+p+q+c) + (p+q)(i+c+e) + 2c(p+q) + i(q+p))

this simplifies to O((p+q)(e+4c+3i+p+q))

this just means the length of the main request list (or the combined length of
active and pending rental requests) multiplied by 3 iterations of inventory list,
4 iterations of customer list, total number of rental or return requests, and the
length of the main request list.


MISC. COMMENTS TO GRADER:
(optional, please be concise!)

