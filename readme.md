# Testing

In this repository you can find your (altered) test script:

I changed the workflow of your script a little bit, because with your script the first merge was already a merge with conflicts.
I also inserted branches, because with my implementation you cannot work on a detached HEAD state.
So if you checkout a commit you can only view this. I hope this is not a big problem that I chose this design.
However you can create branches whenever you want and checkout branches and work with them.


# Design Implementation

* Avoid full copies on commit:
To avoid full copies I decided to store the initial file when it gets committed.
This means I store the first occurrence of the file and after that I used the diff command.
I hope this is still ok.

* Removed Files: For each commit I stored a list of removed files to make sure this files will get deleted if we checkout another branch/commit.


# Known Issues

* After a commit after merge with conflicts you cannot see both parent commits, because I wasnt able to find a way to differentiate between a normal commit or a commit after a merge with conflicts

* Log is missing. Reason: I had a cast for 2 weeks (I was not able to program anything) and I had some time issues to finish everything.
Because of the time issues I also did not have a lot iof time for refactoring the code :(