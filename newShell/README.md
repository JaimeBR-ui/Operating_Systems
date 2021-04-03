# Extra Commands Explained
## Implementing repeat
This command simply takes the repeat amount and the rest of the arguments and
calls the parser/runs the command. Its repeated using a loop however many times
it was specified by the user. This uses the start command in front of the arguments
to run the binaries.
#### Example
Let's say we want to open 4 new shells. In this case we would run:
```sh
# repeat 4 /usr/bin/open -a Terminal
```
Which in turn runs the following command 4 times:
```sh
# background /usr/bin/open -a Terminal
```
We run in the background since these new processes may be long living.

## Implementing dalekall
To implement this command, I used a std::vector of type int to store all the
PIDs of the child processes spawned. When its time to call the command, I loop
through the vector containing the child PIDs and kill them one at a time using
kill(). If the process does not exist then it is not stated as exterminated since
it was already killed.

#### Example
Let's say we have processes 4331, 4600, 4710. Running dalekall would kill these.
```sh
# dalekall
Exterminating 3 processe(s):
4331
4600
4710
#
```
If a process such as 4600 and 4710 were unable to be killed, the prompt would return:
```sh
# dalekall
Exterminating 1 processe(s):
4331
#
```
### Jaime Bohorquez
###### For nicer formatting, view this file with markdown and/or change the extension to .md
