How to contribute:

Setup a clean area
```bash
mkdir susynt_dev
cd susynt_dev
git clone git@github.com:gerbaudo/SusyNtuple.git
source scripts/installMinimalSUSYTools.sh
```

Commit changes to your local repository and push them to github:
```bash
cd SusyNtuple
git status # start from a clean state, up-to-date with the origin
git pull   # make sure you are up to date
git checkout -b some-project  # put your work on a branch
git commit some.txt file.txt  # commit files and prompt editor for commit message
git push origin some-project  # push your changes to github
```
Notes:
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

For more details on the workflow, see
[a simple git branching model](https://gist.github.com/jbenet/ee6c9ac48068889b0912)
or
[a detailed description of the feature-branch](https://www.atlassian.com/git/workflows#!workflow-feature-branch)
