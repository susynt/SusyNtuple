#How to contribute

This doc is mainly for developers, hopefully serving as a quick reference for using git.

GitHub repos can be used in numerous ways, but the recommended way is to set up your GitHub account with ssh keys and use the ssh protocol.

###Setup git

On lxplus, git is already installed. We don't yet have a common installation on the UCI T3, but we can use Davide's installation for now. Just add his bin directory to your path:
```bash
export PATH="$PATH:~gerbaudo/opt/bin"
```
For first time use, you must setup your configuration,
[as described here](https://help.github.com/articles/set-up-git#set-up-git). 
However, don't bother with the password caching. 
Instead, [setup your ssh keys](https://help.github.com/articles/generating-ssh-keys).

###Setup a clean development area
```bash
mkdir susynt_dev
cd susynt_dev
git clone git@github.com:gerbaudo/SusyNtuple.git
source SusyNtuple/scripts/installMinimalSUSYTools.sh
```

###Commit changes to your local repository and push them to github:
```bash
cd SusyNtuple
git status # start from a clean state, up-to-date with the origin
git pull   # make sure you are up to date
git checkout -b some-project  # put your work on a branch
git commit some.txt file.txt  # commit files and prompt editor for commit message
git push origin some-project  # push your changes to github
```
Notes:
- feature development should generally always occur on a dedicated
  branch, rather than on the master branch
- try to pick a branch name that is short and describes what you are
  trying to do
- when you have made enough progress, push to github, then issue a
  pull request (green button), and start discussing the proposed
  changes
- recommendations about commit messages. Follow Linus' guidelines: one
  short descriptive line, followed by a longer description of the
  motivation and implementation, if necessary. Limit line length (with
  emacs `M-q` or `M-x fill-paragraph`)
- setting up your git preferences (editor, aliases, etc.): see the
  [Git Configuration](http://git-scm.com/book/en/Customizing-Git-Git-Configuration)
  docs
- report issues and bugs: use the github and open a new issue.
- you can either push to the repo gerbaudo/SusyNtuple (simpler,
  recommended option, but you should ask to add your username as a
  developer), or you can fork that repo and then issue the pull
  request from your fork

###Helpful links
- [Reference of all git commands](http://git-scm.com/docs).
- When you have time, I recommend reading through [the git book](http://git-scm.com/book).
- [Here's a simple guide for using git](http://rogerdudler.github.io/git-guide/), found by Suneet.
- For more details on the workflow, see
[a simple git branching model](https://gist.github.com/jbenet/ee6c9ac48068889b0912)
or
[a detailed description of the feature-branch](https://www.atlassian.com/git/workflows#!workflow-feature-branch).
