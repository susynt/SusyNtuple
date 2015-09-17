#!/bin/bash

# Most of this script is taken from
#
#  http://benlimmer.com/2013/12/26/automatically-publish-javadoc-to-gh-pages-with-travis-ci/
#  and
#  https://github.com/ReadyTalk/swt-bling/blob/master/.utility/push-javadoc-to-gh-pages.sh
#
# davide.gerbaudo@gmail.com
# June 2014

# trigger doxygen build only when the commit message title contains "[build-doxygen]"
LAST_COMMIT_MSG=$(git log  --pretty=oneline  -n 1)
if [[ "${LAST_COMMIT_MSG}" == *\[build-doxygen\]* ]] && [ "$TRAVIS_REPO_SLUG" == "susynt/SusyNtuple" ] && [ "$TRAVIS_PULL_REQUEST" == "false" ]; then

  echo -e "Publishing doxygen...\n"
  doxygen  doc/doxygen.conf
  cp -R doc/doxygen/html $HOME/doxygen-latest

  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "travis-ci"
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/gerbaudo/SusyNtuple gh-pages > /dev/null

  cd gh-pages
  git rm -rf ./doxygen-html
  cp -Rf $HOME/doxygen-latest ./doxygen-html
  git add -f .
  git commit -m "Lastest doc on successful travis build $TRAVIS_BUILD_NUMBER auto-pushed to gh-pages"
  git push -fq origin gh-pages > /dev/null

  echo -e "Published doxygen doc to gh-pages.\n"
else
  echo -e "Skipping doxygen build.\n"
fi
