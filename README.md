# zapgit

git-based bug zapper. status: fuzzy-edged idea

I've been looking around at bug trackers that integrate with git. I don't like any of
them, so I'm going build to yet another, written in C++.

The basic idea is profoundly simple: capture the history of each bug in its own directory,
monotonically appending to the end of a journal file using a simple markdown-compatible
syntax. Edits to old notes will be permitted, so it will be up to git history to track
such amendments.

In addition to leveraging basic git functionality, the following capabilities may be
needed:

1. Copy a bug onto a branch. If a bug is discovered and documented on one branch, but the
   fix is going to be applied on another, it makes sense to copy the bug across. On the
   from-branch, the bug should be updated with a note advertising the copy. This alerts
   other devs to the fact that the bug is being worked on elsewhere.

1. Rebase a bug to immediately after the last pushed commit and push just the bug. This
   provides a convenient mechanism to advertise a bug without having to also push wip.
   You may, of course, wish to push some amount of wip so others can see the failing
   code, not just the bug report.

zapgit itself will be agnostic to branching strategies and to question of whether bugs
should live on code branches and/or directories or have its own dedicated home.
