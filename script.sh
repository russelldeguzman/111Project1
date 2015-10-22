#simple command simple case: PASSED
echo hello 

#and command simple case:PASSED
echo a && echo b

#and command bad first command: PASSED
a && echo b

#and command bad second command: PASSED
echo a && b

#or command simple case: PASSED
echo a || echo b

#or first command bad: PASSED
a || echo b

#both commands bad: PASSED
a || b

#sequence command simple case: PASSED
echo a; echo b

#sequence command bad left: PASSED
 a; echo b

#sequence command bad right: PASSED
echo a; b

#TODO: SINGLE SEQUENCE COMMAND CASE e.g. echo a;

#subshell command simple case: PASSED
(echo x)

#subshell harder case: PASSED 
(echo a && echo b)

#Subshell I/O case:
(wc) < script2.sh

#pipe: Passed
cat whatever.txt | less

#pipe 2:Passed
ls -al | more > wotm8

