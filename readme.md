In this repository you can find your (altered) test script:

I changed the workflow of your script a little bit, because with your script the first merge was already a merge with conflicts. (If I remember correctly)
I also inserted branches, because with my implementation you cannot work on a detached HEAD state.
So if you checkout a commit you can only view this. I hope this is not a big problem that I chose this design.
However you can create branches whenever you want and checkout branches and work with them.


Below you can see my design of lit

Comment on (Avoid full copies on commit):
To avoid full copies I decided to store the initial file when it gets committed.
This means I store the first occurrence of the file and after that I used the diff command.
I hope this is still ok.

Removed Files:
For each commit I stored a list of removed files to make sure this files will get deleted if we checkout another branch/commit.